import gdb, struct, os

LOG = open(os.environ.get("KMINES_LOG", "/tmp/probe4.log"), "w")
LIMIT = int(os.environ.get("KMINES_LIMIT", "40"))
hits = [0]

def log(*a):
    LOG.write(" ".join(str(x) for x in a) + "\n"); LOG.flush()

def mem(addr, n):
    return bytes(gdb.selected_inferior().read_memory(addr, n))

def qword(p):
    return struct.unpack("<Q", mem(p, 8))[0]

def printable(s):
    return len(s) > 1 and all(32 <= ord(c) < 0x2100 for c in s)

def try_decode(d):
    """Try plausible Qt5/Qt6 QArrayData layouts at header address d."""
    out = []
    try:
        raw32 = mem(d, 32)
    except Exception:
        return out
    words = struct.unpack("<8q", raw32)
    out.append("hdr=" + ",".join(str(w) for w in words[:6]))
    for size_off in (4, 8, 12, 16):
        for off_off in (8, 12, 16, 20, 24, 28):
            try:
                size = struct.unpack_from("<q", raw32, size_off)[0]
                off = struct.unpack_from("<q", raw32, off_off)[0]
            except Exception:
                continue
            if 1 <= size <= 64 and 0 <= off <= 128:
                for unit, enc in ((2, "utf-16-le"), (1, "utf-8")):
                    try:
                        payload = mem(d + off, size * unit)
                    except Exception:
                        continue
                    s = payload.decode(enc, "replace")
                    if printable(s):
                        out.append("size@%d off@%d enc=%s -> %r" % (size_off, off_off, enc, s))
    return out

class BP(gdb.Breakpoint):
    def __init__(self, addr, label):
        super().__init__("*%d" % addr, internal=True); self.label = label
    def stop(self):
        fr = gdb.selected_frame()
        rsi = int(fr.read_register("rsi"))
        log("== %s rsi=%s" % (self.label, hex(rsi)))
        try:
            raw = [hex(qword(rsi + i * 8)) for i in range(4)]
        except Exception as e:
            raw = "err %s" % e
        log("  obj qwords:", raw)
        d = qword(rsi)
        log("  d=", hex(d))
        for line in try_decode(d):
            log("   [d]", line)
        for line in try_decode(rsi):
            log("   [obj]", line)
        hits[0] += 1
        if hits[0] >= LIMIT:
            log("LIMIT")
            gdb.execute("detach", to_stdout=True); os._exit(0)
        return False

def started(event):
    if getattr(started, "done", False):
        return
    maps = gdb.execute("info proc mappings", to_string=True)
    base = None
    for line in maps.splitlines():
        if "/usr/games/kmines" in line:
            base = int(line.split()[0], 16); break
    if base is None:
        return
    started.done = True
    log("BASE 0x%x" % base)
    BP(base + 0xf7b0, "QLabel::setText")
    BP(base + 0xf6f0, "KGameDifficultyLevel")
    BP(base + 0xf450, "KGameRenderedItem")
    gdb.execute("continue", to_stdout=True)

gdb.events.new_objfile.connect(started)
gdb.execute("set pagination off")
gdb.execute("run")
