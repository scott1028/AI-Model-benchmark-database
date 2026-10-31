import gdb, struct, os

ENTRY_OFF = int(os.environ.get("KMINES_ENTRY", "0x10920"), 16)
LOG = open(os.environ.get("KMINES_LOG", "/tmp/probe.log"), "w")

def log(*a):
    LOG.write(" ".join(str(x) for x in a) + "\n")
    LOG.flush()

def mem(addr, n):
    return bytes(gdb.selected_inferior().read_memory(addr, n))

def dqstr(ptr):
    try:
        d = struct.unpack("<Q", mem(ptr, 8))[0]
        if d == 0:
            return "<null>"
        size = struct.unpack("<q", mem(d + 8, 8))[0]
        off = struct.unpack("<q", mem(d + 24, 8))[0]
        if size < 0 or size > 4096:
            return "<badsize %d>" % size
        raw = mem(d + off, size * 2)
        return raw.decode("utf-16-le", "replace")
    except Exception as e:
        return "<err %s>" % e

class HashInsert(gdb.Breakpoint):
    def __init__(self, addr):
        super().__init__("*%d" % addr, internal=True)
        self.hits = 0

    def stop(self):
        f = gdb.selected_frame().read_register
        rdi = int(gdb.selected_frame().read_register("rdi"))
        rsi = int(gdb.selected_frame().read_register("rsi"))
        rdx = int(gdb.selected_frame().read_register("rdx"))
        log("HASHINS hash=0x%x key=%r val=%r" % (rdi, dqstr(rsi), dqstr(rdx)))
        self.hits += 1
        if self.hits > 400:
            log("HASHINS limit reached")
            gdb.execute("detach", to_stdout=True)
            os._exit(0)
        return False

class SpriteKey(gdb.Breakpoint):
    def __init__(self, addr):
        super().__init__("*%d" % addr, internal=True)
        self.hits = 0

    def stop(self):
        rsi = int(gdb.selected_frame().read_register("rsi"))
        log("SPRITEKEY %r" % dqstr(rsi))
        self.hits += 1
        return False

class SpritePixmap(gdb.Breakpoint):
    def __init__(self, addr):
        super().__init__("*%d" % addr, internal=True)
        self.hits = 0

    def stop(self):
        fr = gdb.selected_frame()
        rsi = int(fr.read_register("rsi"))
        rdx = int(fr.read_register("rdx"))
        rcx = int(fr.read_register("rcx"))
        log("SPRITEPIXMAP key=%r size=0x%x(%d,%d) arg3=%d" %
            (dqstr(rsi), rdx & 0xffffffff, rdx & 0xffffffff, (rdx >> 32) & 0xffffffff, rcx))
        self.hits += 1
        if self.hits > 200:
            log("SPRITEPIXMAP limit reached")
            gdb.execute("detach", to_stdout=True)
            os._exit(0)
        return False

def started(event):
    if not isinstance(event, gdb.NewObjFileEvent):
        return
    inf = gdb.selected_inferior()
    maps = gdb.execute("info proc mappings", to_string=True)
    base = None
    for line in maps.splitlines():
        if "/usr/games/kmines" in line:
            base = int(line.split()[0], 16)
            break
    if base is None:
        log("BASE not found")
        os._exit(1)
    log("BASE 0x%x" % base)
    HashInsert(base + 0x16630)
    SpriteKey(base + 0xfa30)
    SpritePixmap(base + 0xf590)
    gdb.execute("continue", to_stdout=True)

gdb.events.new_objfile.connect(started)
gdb.execute("set pagination off")
gdb.execute("set confirm off")
gdb.execute("run")
