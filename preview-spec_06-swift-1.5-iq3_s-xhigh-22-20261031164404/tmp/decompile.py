from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

decomp = DecompInterface()
decomp.openProgram(currentProgram)
monitor = ConsoleTaskMonitor()
funcs = currentProgram.getFunctionManager().getFunctions(True)
out = []
n = 0
for f in funcs:
    res = decomp.decompileFunction(f, 60, monitor)
    df = res.getDecompiledFunction() if res is not None else None
    if df is not None:
        out.append("/* ===== %s @ %s ===== */\n%s" % (f.getName(), f.getEntryPoint(), df.getC()))
        n += 1
    else:
        out.append("/* ===== %s @ %s : DECOMPILE FAILED ===== */" % (f.getName(), f.getEntryPoint()))
open("/home/scott/workspace/AI-Model-benchmark-database/preview-wip/tmp/ghidra_out/decompiled.c", "w").write("\n".join(out))
print("Exported %d functions" % n)
