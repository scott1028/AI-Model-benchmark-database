import json,os,re,sys
base="/home/scott/workspace/AI-Model-benchmark-database/preview-wip/tmp/rea_decompile"
idx=json.load(open(os.path.join(base,"decompile-index.json")))
fns=[f for f in idx["functions"] if f.get("status")=="written"]
pat=re.compile(r"\bFUN_[0-9a-f]{4,}")
info={}
for f in fns:
    txt=open(os.path.join(base,f["relative_path"])).read()
    body=txt.split("// pseudocode:",1)[-1]
    calls=sorted(set(m.group(0) for m in pat.finditer(body)))
    vaddr=int(f["address"],16)-0x100000
    info[f["name"]]=(vaddr,f["address"],calls,os.path.join(base,f["relative_path"]))
# reverse graph: who calls X
rev={}
for name,(v,a,c,p) in info.items():
    for callee in c:
        rev.setdefault(callee,[]).append(name)
if sys.argv[1]=="rev":
    for tgt in sys.argv[2:]:
        print(f"== callers of {tgt} ==", [f"{k}@{hex(info[k][0])}" for k in rev.get(tgt,[])])
elif sys.argv[1]=="fwd":
    n="FUN_"+sys.argv[2].replace("0x","")
    print(n, "calls", [(k, hex(info[k][0]) if k in info else "?") for k in info.get(n,(0,0,[],""))[2]])
elif sys.argv[1]=="sizes":
    rows=[]
    for name,(v,a,c,p) in info.items():
        if name.startswith("FUN_"):
            n=sum(1 for _ in open(p))
            rows.append((n,name,hex(v),len(c)))
    for r in sorted(rows,reverse=True)[:int(sys.argv[2])]:
        print(f"{r[0]:5d} lines  {r[1]}  vaddr={r[2]}  callees={r[3]}")
