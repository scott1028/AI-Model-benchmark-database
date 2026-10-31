import json,sys,os,re
base="/home/scott/workspace/AI-Model-benchmark-database/preview-wip/tmp/rea_decompile"
idx=json.load(open(os.path.join(base,"decompile-index.json")))
fns=[f for f in idx["functions"] if f.get("status")=="written"]
BASE=0x100000
def find(addr):
    a=int(addr,16)
    cands=[a, a+BASE]
    best=None
    for target in cands:
        for f in fns:
            for r in (f.get("body") or {}).get("ranges",[]):
                s=int(r["start"],16); e=int(r["end"],16)
                if s<=target<=e:
                    if best is None or s>int(best["address"],16): best=f
    return best
cmd=sys.argv[1]
if cmd=="find":
    f=find(sys.argv[2]);  print(f["address"], f["name"], f["relative_path"] if f else "not found")
elif cmd=="show":
    f=find(sys.argv[2])
    if not f: print("not found"); sys.exit(1)
    print("/* FUNC ghidra=%s vaddr=%s name=%s */"%(f["address"],hex(int(f["address"],16)-BASE),f["name"]))
    print(open(os.path.join(base,f["relative_path"])).read())
elif cmd=="names":
    for f in sorted(fns,key=lambda x:int(x["address"],16)):
        print(f["address"], hex(int(f["address"],16)-BASE), f["name"], f["relative_path"])
elif cmd=="grep":
    pat=re.compile(sys.argv[2])
    for f in sorted(fns,key=lambda x:int(x["address"],16)):
        txt=open(os.path.join(base,f["relative_path"])).read()
        if pat.search(txt): print(f["address"], hex(int(f["address"],16)-BASE), f["name"])
