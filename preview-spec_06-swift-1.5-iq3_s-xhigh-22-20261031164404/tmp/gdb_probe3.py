import gdb, struct, os

LOG = open(os.environ.get("KMINES_LOG", "/tmp/probe3.log"), "w")
LIMIT = int(os.environ.get("KMINES_LIMIT", "200"))
hits = [0]

def log(*a):
    LOG.write(" ".join(str(x) for x in a) + "\n"); LOG.flush()

def mem(addr, n):
    return bytes(gdb.selected_inferior().read_memory(addr, n))

def qword(p):
    return struct.unpack("<Q", mem(p, 8))[0]

def dq(ptr, enc="utf-16-le"):
    """Read a Qt6 QString/QByteArray at *ptr* (pointer to the object)."""
    try:
        d = qword(ptr)
        if d == 0:
            return "<null-d>"
        size = struct.unpack("<q", mem(d + 8, 8))[0]
        off = struct.unpack("<q", mem(d + 24, 8))[0]
        if size < 0 or size > 200:
            return "<size=%d>" % size
        raw = mem(d + off, size)
        if enc == "utf-16-le":
            return raw.decode("utf-16-le", "replace")
        return raw.decode("utf-8", "replace")
    except Exception as e:
        return "<err %s>" % e

def regs(fr, names):
    return [int(fr.read_register(n)) for n in names]

class BP(gdb.Breakpoint):
    def __init__(self, addr, handler, label):
        super().__init__("*%d" % addr, internal=True)
        self.handler = handler; self.label = label
    def stop(self):
        try:
            self.handler(gdb.selected_frame())
        except Exception as e:
            log("ERR", self.label, e)
        hits[0] += 1
        if hits[0] >= LIMIT:
            log("LIMIT reached (%s)" % self.label)
            gdb.execute("detach", to_stdout=True); os._exit(0)
        return False

def h_label(fr):
    r = regs(fr, ("rdi", "rsi"))
    log("LABELsetText this=%s text=%r" % (hex(r[0]), dq(r[1])))

def h_button(fr):
    r = regs(fr, ("rdi", "rsi"))
    log("ButtonsetText this=%s text=%r" % (hex(r[0]), dq(r[1])))

def h_diff(fr):
    r = regs(fr, ("rdi", "rsi", "rdx", "rcx", "r8"))
    log("DIFFLevel value=%d name=%r label=%r scoreEnabled=%d" %
        (r[1] & 0xffffffff, dq(r[2], "utf-8"), dq(r[3]), r[4] & 0xff))

def h_addr(fr):
    r = regs(fr, ("rdi", "rsi", "rdx"))
    log("StandardLevelRange begin=%d end=%d" % (r[1] & 0xffffffff, r[2] & 0xffffffff))

def h_itemint(fr):
    r = regs(fr, ("rdi", "rsi", "rdx", "rcx", "r8"))
    log("ItemInt key=%r group=%r default=%d" % (dq(r[1]), dq(r[2]), r[4] & 0xffffffff))

def h_itembool(fr):
    r = regs(fr, ("rdi", "rsi", "rdx", "rcx", "r8"))
    log("ItemBool key=%r group=%r default=%d" % (dq(r[1]), dq(r[2]), r[4] & 0xffffffff))

def h_itemenum(fr):
    r = regs(fr, ("rdi", "rsi", "rdx", "rcx", "r8", "r9"))
    log("ItemEnum key=%r group=%r default=%d" % (dq(r[1]), dq(r[2]), r[5] & 0xffffffff))

def h_min(fr):
    r = regs(fr, ("rdi", "rsi"))
    log("ItemInt setMinValue obj=%s value=%d" % (hex(r[0]), r[1] & 0xffffffff))

def h_max(fr):
    r = regs(fr, ("rdi", "rsi"))
    log("ItemInt setMaxValue obj=%s value=%d" % (hex(r[0]), r[1] & 0xffffffff))

def h_group(fr):
    r = regs(fr, ("rdi", "rsi"))
    log("Config setCurrentGroup group=%r" % dq(r[1]))

def h_rendered(fr):
    r = regs(fr, ("rdi", "rsi", "rdx"))
    log("KGameRenderedItem this=%s key=%r parent=%s" % (hex(r[0]), dq(r[1]), hex(r[2])))

def h_spritekey(fr):
    r = regs(fr, ("rdi", "rsi"))
    log("SpriteKey this=%s key=%r" % (hex(r[0]), dq(r[1])))

def h_popup(fr):
    r = regs(fr, ("rdi", "rsi", "rdx", "rcx"))
    log("Popup showMessage msg=%r position=%d mode=%d" % (dq(r[1]), r[2] & 0xffffffff, r[3] & 0xffffffff))

def h_clock(fr):
    r = regs(fr, ("rdi", "rsi"))
    log("KGameClock this=%s clockType=%d" % (hex(r[0]), r[1] & 0xffffffff))

def h_render_size(fr):
    r = regs(fr, ("rdi", "rsi"))
    log("setRenderSize this=%s w=%d h=%d" % (hex(r[0]), r[1] & 0xffffffff, (r[1] >> 32) & 0xffffffff))

TARGETS = [
    (0xf7b0, h_label, "label"),
    (0xef90, h_button, "button"),
    (0xf6f0, h_diff, "diff"),
    (0xfae0, h_addr, "range"),
    (0xf790, h_itemint, "itemint"),
    (0xfb50, h_itembool, "itembool"),
    (0xeee0, h_itemenum, "itemenum"),
    (0xf0d0, h_min, "min"),
    (0xf430, h_max, "max"),
    (0xf540, h_group, "group"),
    (0xf450, h_rendered, "rendered"),
    (0xfa30, h_spritekey, "spritekey"),
    (0xf3f0, h_popup, "popup"),
    (0xf720, h_clock, "clock"),
]

def started(event):
    maps = gdb.execute("info proc mappings", to_string=True)
    base = None
    for line in maps.splitlines():
        if "/usr/games/kmines" in line:
            base = int(line.split()[0], 16); break
    if base is None:
        return
    if getattr(started, "done", False):
        return
    started.done = True
    log("BASE 0x%x" % base)
    for off, h, lab in TARGETS:
        BP(base + off, h, lab)
    gdb.execute("continue", to_stdout=True)

gdb.events.new_objfile.connect(started)
gdb.execute("set pagination off")
gdb.execute("run")
