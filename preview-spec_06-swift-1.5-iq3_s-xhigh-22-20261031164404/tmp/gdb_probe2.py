import gdb, struct, os

LOG = open(os.environ.get("KMINES_LOG", "/tmp/probe2.log"), "w")

def log(*a):
    LOG.write(" ".join(str(x) for x in a) + "\n"); LOG.flush()

def mem(addr, n):
    return bytes(gdb.selected_inferior().read_memory(addr, n))

def qword(p):
    return struct.unpack("<Q", mem(p, 8))[0]

def dqstr(ptr, label):
    try:
        d = qword(ptr)
        out = []
        for base in (d, ptr):
            try:
                words = [qword(base + i * 8) for i in range(5)]
                out.append((hex(base), [hex(w) for w in words]))
            except Exception as e:
                out.append((hex(base), "err"))
        log(label, "ptr=%s raw=%s" % (hex(ptr), out))
    except Exception as e:
        log(label, "outer err", e)

class SP(gdb.Breakpoint):
    def __init__(self, addr):
        super().__init__("*%d" % addr, internal=True); self.hits = 0
    def stop(self):
        fr = gdb.selected_frame()
        r = [int(fr.read_register(n)) for n in ("rdi","rsi","rdx","rcx","r8","r9")]
        log("SPRITEPIXMAP regs", [hex(x) for x in r])
        dqstr(r[1], "  arg1(rsi)")
        dqstr(r[1] & ~0xf, "  arg1-aligned")
        self.hits += 1
        if self.hits >= 4:
            gdb.execute("detach", to_stdout=True); os._exit(0)
        return False

def started(event):
    maps = gdb.execute("info proc mappings", to_string=True)
    base = None
    for line in maps.splitlines():
        if "/usr/games/kmines" in line:
            base = int(line.split()[0], 16); break
    if base is None: return
    log("BASE 0x%x" % base)
    SP(base + 0xf590)
    gdb.execute("continue", to_stdout=True)

gdb.events.new_objfile.connect(started)
gdb.execute("set pagination off")
gdb.execute("run")
