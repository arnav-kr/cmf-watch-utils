"""Windows Bluetooth SDP lookup via WSALookupServiceBegin/Next (ws2_32).

Provides sdp_lookup(addr, service_uuid=None) -> list of dicts with
name/class_uuid/channel. Ports the pybluez msbt.py approach.

Run:  python sdp.py --address AA:BB:CC:DD:EE:FF [--uuid 66666666-...]
"""

import argparse
import ctypes
from ctypes import wintypes, POINTER, byref

from rfcomm import ws_init, ws_ready  # noqa

NS_BTH = 16
LUP_FLUSHCACHE = 0x1000
LUP_RETURN_NAME = 0x0010
LUP_RETURN_TYPE = 0x0020
LUP_RETURN_VERSION = 0x0040
LUP_RETURN_COMMENT = 0x0080
LUP_RETURN_ADDR = 0x0100
LUP_RETURN_BLOB = 0x0200
LUP_RETURN_ALIASES = 0x0400
LUP_RETURN_QUERY_STRING = 0x0800
LUP_RETURN_ALL = 0x0FF0
LUP_CONTAINERS = 0x0002
LUP_NOCONTAINERS = 0x0004

WSA_E_NO_MORE = 10110
WSAENOMORE = 10102
WSA_E_CANCELLED = 10111
WSAECANCELLED = 10103


class GUID(ctypes.Structure):
    _fields_ = [("Data1", wintypes.DWORD), ("Data2", wintypes.WORD),
                ("Data3", wintypes.WORD), ("Data4", ctypes.c_ubyte * 8)]


class WSAVERSION(ctypes.Structure):
    _fields_ = [("dwVersion", wintypes.DWORD), ("dwHiVersion", wintypes.DWORD)]


class SOCKADDR(ctypes.Structure):
    _fields_ = [("sa_family", ctypes.c_ushort), ("sa_data", ctypes.c_char * 14)]


class SOCKET_ADDRESS(ctypes.Structure):
    _fields_ = [("lpSockaddr", POINTER(SOCKADDR)), ("iSockaddrLength", ctypes.c_int)]


class CSADDR_INFO(ctypes.Structure):
    _fields_ = [("LocalAddr", SOCKET_ADDRESS), ("RemoteAddr", SOCKET_ADDRESS),
                ("iSocketType", ctypes.c_int), ("iProtocol", ctypes.c_int)]


class AFPROTOCOLS(ctypes.Structure):
    _fields_ = [("iAddressFamily", ctypes.c_int), ("iProtocol", ctypes.c_int)]


class BLOB(ctypes.Structure):
    _fields_ = [("cbSize", wintypes.DWORD), ("pBlobData", POINTER(ctypes.c_ubyte))]


class WSAQUERYSET(ctypes.Structure):
    _fields_ = [("dwSize", wintypes.DWORD),
                ("lpszServiceInstanceName", wintypes.LPWSTR),
                ("lpServiceClassId", POINTER(GUID)),
                ("lpVersion", POINTER(WSAVERSION)),
                ("lpszComment", wintypes.LPWSTR),
                ("dwNameSpace", wintypes.DWORD),
                ("lpNSProviderId", POINTER(GUID)),
                ("lpszContext", wintypes.LPWSTR),
                ("dwNumberOfProtocols", wintypes.DWORD),
                ("lpafpProtocols", POINTER(AFPROTOCOLS)),
                ("lpszQueryString", wintypes.LPWSTR),
                ("dwNumberOfCsAddrs", wintypes.DWORD),
                ("lpcsaBuffer", POINTER(CSADDR_INFO)),
                ("dwOutputFlags", wintypes.DWORD),
                ("lpBlob", POINTER(BLOB))]


class SOCKADDR_BTH(ctypes.Structure):
    _fields_ = [("addressFamily", ctypes.c_ushort),
                ("btAddr", ctypes.c_ulonglong),
                ("serviceClassId", GUID),
                ("port", ctypes.c_ulong)]


_ws2 = ctypes.WinDLL("ws2_32.dll")
_ws2.WSALookupServiceBeginW.restype = ctypes.c_int
_ws2.WSALookupServiceBeginW.argtypes = [POINTER(WSAQUERYSET), wintypes.DWORD, POINTER(wintypes.HANDLE)]
_ws2.WSALookupServiceNextW.restype = ctypes.c_int
_ws2.WSALookupServiceNextW.argtypes = [wintypes.HANDLE, wintypes.DWORD, POINTER(wintypes.DWORD), POINTER(WSAQUERYSET)]
_ws2.WSALookupServiceEnd.restype = ctypes.c_int
_ws2.WSALookupServiceEnd.argtypes = [wintypes.HANDLE]
_ws2.WSAGetLastError.restype = ctypes.c_int


def uuid_to_guid(uuid):
    u = uuid.replace("-", "").lower()
    d1 = int(u[0:8], 16)
    d2 = int(u[8:12], 16)
    d3 = int(u[12:16], 16)
    d4 = bytes.fromhex(u[16:32])
    return GUID(d1, d2, d3, (ctypes.c_ubyte * 8)(*d4))


def guid_to_uuid(g):
    return ("%08X-%04X-%04X-%s" % (
        g.Data1, g.Data2, g.Data3,
        "".join("%02X" % b for b in g.Data4)[0:4] + "-" + "".join("%02X" % b for b in g.Data4)[4:12]))


def _guid_hex(b):
    return (b"".join(bytes([x]) for x in b)).hex()


def _parse(qs):
    out = {"name": qs.lpszServiceInstanceName, "class_uuid": None, "channel": None, "addrs": []}
    if qs.lpServiceClassId:
        out["class_uuid"] = guid_to_uuid(qs.lpServiceClassId.contents)
    n = qs.dwNumberOfCsAddrs
    if qs.lpcsaBuffer and n:
        for i in range(n):
            cs = qs.lpcsaBuffer[i]
            for tag, sa in (("local", cs.LocalAddr), ("remote", cs.RemoteAddr)):
                if sa.lpSockaddr and sa.iSockaddrLength >= 26:
                    raw = ctypes.string_at(sa.lpSockaddr, sa.iSockaddrLength)
                    af = int.from_bytes(raw[0:2], "little")
                    bt = int.from_bytes(raw[2:10], "little")
                    port = int.from_bytes(raw[26:30], "little") if len(raw) >= 30 else None
                    rec = {"tag": tag, "af": af, "bt": ":".join(
                        f"{(bt >> (8 * k)) & 0xFF:02X}" for k in reversed(range(6))), "channel": port}
                    out["addrs"].append(rec)
                    if tag == "remote" and port:
                        out["channel"] = port
    return out


def sdp_lookup(addr, service_uuid=None, flags=None, force_flush=True):
    if not ws_ready():
        ws_init()
    if flags is None:
        flags = LUP_RETURN_NAME | LUP_RETURN_TYPE | LUP_RETURN_ADDR | LUP_RETURN_BLOB | LUP_RETURN_COMMENT
        if force_flush:
            flags |= LUP_FLUSHCACHE

    restrict = WSAQUERYSET()
    restrict.dwSize = ctypes.sizeof(WSAQUERYSET)
    restrict.dwNameSpace = NS_BTH
    ctx = ctypes.c_wchar_p(addr)
    restrict.lpszContext = ctx
    g = None
    if service_uuid:
        g = uuid_to_guid(service_uuid)
        restrict.lpServiceClassId = ctypes.pointer(g)

    h = wintypes.HANDLE()
    rc = _ws2.WSALookupServiceBeginW(byref(restrict), flags, byref(h))
    if rc != 0:
        raise OSError(f"WSALookupServiceBegin {_ws2.WSAGetLastError()}")

    results = []
    bufsz = 0x10000
    try:
        while True:
            buf = ctypes.create_string_buffer(bufsz)
            qs = ctypes.cast(buf, POINTER(WSAQUERYSET))
            qs.contents.dwSize = ctypes.sizeof(WSAQUERYSET)
            ln = wintypes.DWORD(bufsz)
            rc = _ws2.WSALookupServiceNextW(h, flags, byref(ln), qs)
            if rc != 0:
                e = _ws2.WSAGetLastError()
                if e in (WSA_E_NO_MORE, WSAENOMORE, WSA_E_CANCELLED, WSAECANCELLED):
                    break
                raise OSError(f"WSALookupServiceNext {e}")
            results.append(_parse(qs.contents))
    finally:
        _ws2.WSALookupServiceEnd(h)
    return results


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", required=True)
    ap.add_argument("--uuid", default=None)
    args = ap.parse_args()
    res = sdp_lookup(args.address, args.uuid)
    print(f"{len(res)} service(s) on {args.address}:")
    for r in res:
        print(f"  ch={str(r['channel']):>4}  {r['class_uuid']}  {r['name']!r}  {r['addrs']}")


if __name__ == "__main__":
    main()
