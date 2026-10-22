import argparse
import ctypes
import struct
import threading
import time
from ctypes import wintypes

AF_BTH = 32
BTHPROTO_RFCOMM = 3
SOCK_STREAM = 1
SOCKET = ctypes.c_uint64
INVALID_SOCKET = (1 << 64) - 1
SOL_SOCKET = 0xFFFF
SO_RCVTIMEO = 0x1006
SO_SNDTIMEO = 0x1005
SO_ERROR = 0x1007
FIONBIO = 0x8004667E
WSAEWOULDBLOCK = 10035

_ws2 = ctypes.WinDLL("ws2_32.dll")
_ws2.socket.restype = SOCKET
_ws2.socket.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
_ws2.connect.restype = ctypes.c_int
_ws2.connect.argtypes = [SOCKET, ctypes.c_void_p, ctypes.c_int]
_ws2.send.restype = ctypes.c_int
_ws2.send.argtypes = [SOCKET, ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
_ws2.recv.restype = ctypes.c_int
_ws2.recv.argtypes = [SOCKET, ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
_ws2.closesocket.restype = ctypes.c_int
_ws2.closesocket.argtypes = [SOCKET]
_ws2.setsockopt.restype = ctypes.c_int
_ws2.WSAGetLastError.restype = ctypes.c_int
_ws2.ioctlsocket.restype = ctypes.c_int
_ws2.ioctlsocket.argtypes = [SOCKET, ctypes.c_long, ctypes.c_void_p]
_ws2.getsockopt.restype = ctypes.c_int
_ws2.getsockopt.argtypes = [SOCKET, ctypes.c_int, ctypes.c_int, ctypes.c_void_p, ctypes.c_void_p]
_ws2.select.restype = ctypes.c_int
_ws2.select.argtypes = [ctypes.c_int, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p]


class fd_set(ctypes.Structure):
    _fields_ = [("fd_count", ctypes.c_uint), ("fd_array", SOCKET * 64)]


class TIMEVAL(ctypes.Structure):
    _fields_ = [("tv_sec", ctypes.c_long), ("tv_usec", ctypes.c_long)]


def _set_nonblocking(h):
    v = ctypes.c_ulong(1)
    _ws2.ioctlsocket(h, FIONBIO, ctypes.byref(v))


def _so_error(h):
    v = ctypes.c_int(0)
    ln = ctypes.c_int(ctypes.sizeof(v))
    _ws2.getsockopt(h, SOL_SOCKET, SO_ERROR, ctypes.byref(v), ctypes.byref(ln))
    return v.value


class WSADATA(ctypes.Structure):
    _fields_ = [("wVersion", wintypes.WORD), ("wHighVersion", wintypes.WORD),
                ("szDescription", ctypes.c_char * 257), ("szSystemStatus", ctypes.c_char * 129),
                ("iMaxSockets", wintypes.WORD), ("iMaxUdpDg", wintypes.WORD),
                ("lpVendorInfo", ctypes.c_char_p)]


def ws_init():
    global _WS_READY
    wd = WSADATA()
    rc = _ws2.WSAStartup(0x0202, ctypes.byref(wd))
    if rc == 0:
        _WS_READY = True
    return rc == 0


_WS_READY = False


def ws_ready():
    return _WS_READY


def lasterr():
    return _ws2.WSAGetLastError()


WSA_ERRORS = {
    10048: "WSAEADDRINUSE", 10049: "WSAEADDRNOTAVAIL", 10050: "WSAENETDOWN",
    10051: "WSAENETUNREACH", 10054: "WSAECONNRESET", 10057: "WSAENOTCONN",
    10060: "WSAETIMEDOUT", 10061: "WSAECONNREFUSED", 10065: "WSAEHOSTUNREACH",
    10035: "WSAEWOULDBLOCK", 10022: "WSAEINVAL",
}


def err_name(e):
    return f"{e}:{WSA_ERRORS.get(e, '?')}"


def mac_to_ulong(mac):
    return int(mac.replace(":", "").replace("-", ""), 16)


def guid_bytes(service_uuid=None):
    if not service_uuid:
        return b"\x00" * 16
    u = service_uuid.replace("-", "")
    b = bytes.fromhex(u)
    return b[0:4][::-1] + b[4:6][::-1] + b[6:8][::-1] + b[8:16]


def sockaddr_bth(mac, channel=0, service_uuid=None, layout="packed"):
    af = AF_BTH
    bt = mac_to_ulong(mac)
    guid = guid_bytes(service_uuid)
    if layout == "packed":
        return struct.pack("<HQI", af, bt, channel) + guid
    if layout == "packed2":
        return struct.pack("<HQ", af, bt) + guid + struct.pack("<I", channel)
    if layout == "natural":
        return struct.pack("<H", af) + b"\x00" * 6 + struct.pack("<Q", bt) + guid + struct.pack("<I", channel)
    raise ValueError(layout)


class BthSocket:
    def __init__(self, handle):
        self.h = handle

    def settimeout(self, ms):
        v = ctypes.c_uint32(int(ms))
        _ws2.setsockopt(self.h, SOL_SOCKET, SO_RCVTIMEO, ctypes.byref(v), ctypes.sizeof(v))
        _ws2.setsockopt(self.h, SOL_SOCKET, SO_SNDTIMEO, ctypes.byref(v), ctypes.sizeof(v))

    def send(self, data):
        buf = ctypes.create_string_buffer(data, len(data))
        n = _ws2.send(self.h, buf, len(data), 0)
        return n

    def sendall(self, data):
        off = 0
        while off < len(data):
            n = self.send(data[off:])
            if n <= 0:
                raise OSError(f"send failed {err_name(lasterr())}")
            off += n
        return off

    def recv(self, n, timeout_ms=None):
        if timeout_ms is not None:
            self.settimeout(timeout_ms)
        buf = ctypes.create_string_buffer(n)
        r = _ws2.recv(self.h, buf, n, 0)
        if r < 0:
            e = lasterr()
            if e in (10060, 10035):
                return b""
            raise OSError(f"recv failed {err_name(e)}")
        return buf.raw[:r]

    def close(self):
        _ws2.closesocket(self.h)


def open_bth():
    h = _ws2.socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM)
    if h == INVALID_SOCKET:
        raise OSError(f"socket failed {err_name(lasterr())}")
    return BthSocket(h)


def connect_bth(mac, channel, layout="packed", service_uuid=None):
    s = open_bth()
    sa = sockaddr_bth(mac, channel, service_uuid, layout)
    buf = ctypes.create_string_buffer(sa, len(sa))
    rc = _ws2.connect(s.h, buf, len(sa))
    if rc != 0:
        e = lasterr()
        s.close()
        raise OSError(err_name(e))
    return s


def connect_bth_timeout(mac, channel, layout="packed", service_uuid=None, timeout=2.0):
    s = open_bth()
    info = {"done": False, "err": None}

    def work():
        try:
            sa = sockaddr_bth(mac, channel, service_uuid, layout)
            buf = ctypes.create_string_buffer(sa, len(sa))
            rc = _ws2.connect(s.h, buf, len(sa))
            info["err"] = 0 if rc == 0 else lasterr()
        except Exception as e:
            info["err"] = str(e)
        finally:
            info["done"] = True

    t = threading.Thread(target=work, daemon=True)
    t.start()
    t.join(timeout)
    if not info["done"]:
        try:
            s.close()
        except Exception:
            pass
        return None, "TIMEOUT"
    if info["err"] == 0:
        return s, None
    try:
        s.close()
    except Exception:
        pass
    e = info["err"]
    return None, (err_name(e) if isinstance(e, int) else e)


def sweep_nb(mac, channels, layout="packed2", service_uuid=None, timeout=1.2):
    """Connect to all channels in parallel without blocking; return (open_chs, errors).

    Never leaks sockets: every handle is either returned (open) or closed here.
    """
    pending = {}
    open_chs = {}
    errors = {}
    for ch in channels:
        s = open_bth()
        _set_nonblocking(s.h)
        sa = sockaddr_bth(mac, ch, service_uuid, layout)
        buf = ctypes.create_string_buffer(sa, len(sa))
        rc = _ws2.connect(s.h, buf, len(sa))
        if rc == 0:
            open_chs[ch] = s
            continue
        e = lasterr()
        if e == WSAEWOULDBLOCK:
            pending[ch] = s
        else:
            s.close()
            errors[ch] = err_name(e)

    if not pending:
        return open_chs, errors

    fds = fd_set(0, (SOCKET * 64)())
    for s in pending.values():
        fds.fd_array[fds.fd_count] = s.h
        fds.fd_count += 1
    tv = TIMEVAL(int(timeout), int((timeout % 1.0) * 1_000_000))
    r = _ws2.select(0, None, ctypes.byref(fds), None, ctypes.byref(tv))

    done = set(fds.fd_array[:fds.fd_count]) if r > 0 else set()
    for ch, s in pending.items():
        if s.h in done:
            e = _so_error(s.h)
            if e == 0:
                _set_nonblocking_off(s.h)
                open_chs[ch] = s
            else:
                s.close()
                errors[ch] = err_name(e)
        else:
            s.close()
            errors[ch] = "TIMEOUT"
    return open_chs, errors


def _set_nonblocking_off(h):
    v = ctypes.c_ulong(0)
    _ws2.ioctlsocket(h, FIONBIO, ctypes.byref(v))


def sweep_loop(address, channels, duration=25.0, interval=0.5, layout="packed2", service_uuid=None,
               connect_timeout=None):
    t0 = time.time()
    last = None
    while time.time() - t0 < duration:
        elapsed = time.time() - t0
        ct = connect_timeout if connect_timeout is not None else min(1.2, interval)
        open_chs, errors = sweep_nb(address, channels, layout, service_uuid, timeout=ct)
        state = tuple(sorted(open_chs))
        if state != last:
            detail = " ".join(f"ch{c}={errors.get(c, 'open').split(':')[0]}" for c in channels)
            print(f"[{elapsed:6.2f}s] open={list(state)}  ({detail})", flush=True)
            last = state
        for s in open_chs.values():
            s.close()
        time.sleep(max(0.0, interval - (time.time() - t0 - elapsed)))


def connect_nb_wait(mac, channel, layout="packed2", service_uuid=None, timeout=2.0):
    """Single non-blocking connect that tolerates paging latency.

    Returns (BthSocket|None, err|None). Never leaks the socket.
    """
    s = open_bth()
    _set_nonblocking(s.h)
    sa = sockaddr_bth(mac, channel, service_uuid, layout)
    buf = ctypes.create_string_buffer(sa, len(sa))
    rc = _ws2.connect(s.h, buf, len(sa))
    if rc != 0:
        e = lasterr()
        if e != WSAEWOULDBLOCK:
            s.close()
            return None, err_name(e)
    deadline = time.time() + timeout
    while True:
        if rc == 0:
            _set_nonblocking_off(s.h)
            return s, None
        remain = deadline - time.time()
        if remain <= 0:
            s.close()
            return None, "TIMEOUT"
        fds = fd_set(1, (SOCKET * 64)(s.h))
        tv = TIMEVAL(int(remain), int((remain % 1.0) * 1_000_000))
        r = _ws2.select(0, None, ctypes.byref(fds), None, ctypes.byref(tv))
        if r > 0:
            e = _so_error(s.h)
            if e == 0:
                _set_nonblocking_off(s.h)
                return s, None
            s.close()
            return None, err_name(e)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", required=True)
    ap.add_argument("--channels", default="11")
    ap.add_argument("--layouts", default="packed")
    ap.add_argument("--uuid", default=None)
    args = ap.parse_args()

    ws_init()
    channels = [int(c, 0) for c in args.channels.split(",") if c.strip()]
    layouts = [x for x in args.layouts.split(",") if x.strip()]
    for layout in layouts:
        for ch in channels:
            t0 = time.time()
            try:
                s = connect_bth(args.address, ch, layout, args.uuid)
                print(f"[{layout:8s} ch={ch:3d}] CONNECTED  {time.time()-t0:.2f}s", flush=True)
                s.close()
            except OSError as e:
                print(f"[{layout:8s} ch={ch:3d}] fail {e} {time.time()-t0:.2f}s", flush=True)


if __name__ == "__main__":
    main()
