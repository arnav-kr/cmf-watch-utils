import ctypes
from ctypes import POINTER, byref, wintypes
import sdp
from sdp import _ws2, WSAQUERYSET, CSADDR_INFO, SOCKADDR


def dump(addr, uuid):
    restrict = WSAQUERYSET()
    restrict.dwSize = ctypes.sizeof(WSAQUERYSET)
    restrict.dwNameSpace = sdp.NS_BTH
    restrict.lpszContext = ctypes.c_wchar_p(addr)
    g = sdp.uuid_to_guid(uuid)
    restrict.lpServiceClassId = ctypes.pointer(g)
    flags = sdp.LUP_RETURN_NAME | sdp.LUP_RETURN_ADDR | sdp.LUP_RETURN_BLOB | sdp.LUP_FLUSHCACHE
    h = wintypes.HANDLE()
    rc = _ws2.WSALookupServiceBeginW(byref(restrict), flags, byref(h))
    print("begin rc", rc, "err", _ws2.WSAGetLastError())
    while True:
        buf = ctypes.create_string_buffer(0x10000)
        qs = ctypes.cast(buf, POINTER(WSAQUERYSET))
        qs.contents.dwSize = ctypes.sizeof(WSAQUERYSET)
        ln = wintypes.DWORD(0x10000)
        rc = _ws2.WSALookupServiceNextW(h, flags, byref(ln), qs)
        if rc != 0:
            print("next err", _ws2.WSAGetLastError())
            break
        q = qs.contents
        print("name=", q.lpszServiceInstanceName, "ncs=", q.dwNumberOfCsAddrs,
              "nsreg=", q.dwNameSpace, "flags=", hex(q.dwOutputFlags))
        if q.lpServiceClassId:
            b = bytes(q.lpServiceClassId.contents)
            print("  classGUID raw", b.hex())
        csa = ctypes.cast(q.lpcsaBuffer, POINTER(CSADDR_INFO))
        for i in range(q.dwNumberOfCsAddrs):
            ci = csa[i]
            for tag, sa in (("local", ci.LocalAddr), ("remote", ci.RemoteAddr)):
                if sa.lpSockaddr and sa.iSockaddrLength:
                    raw = ctypes.string_at(sa.lpSockaddr, sa.iSockaddrLength)
                    print(f"  {tag} len={sa.iSockaddrLength} raw={raw.hex()}")
    _ws2.WSALookupServiceEnd(h)


sdp.ws_init()
dump("AA:BB:CC:DD:EE:FF", "00001101-0000-1000-8000-00805F9B34FB")
