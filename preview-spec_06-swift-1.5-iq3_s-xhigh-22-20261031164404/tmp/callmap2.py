import json,os,re,sys
base="/home/scott/workspace/AI-Model-benchmark-database/preview-wip/tmp/rea_decompile"
idx=json.load(open(os.path.join(base,"decompile-index.json")))
fns=[f for f in idx["functions"] if f.get("status")=="written"]
names=[l.strip() for l in open("/home/scott/workspace/AI-Model-benchmark-database/preview-wip/tmp/imports_names.txt") if l.strip()]
# build regex: match calls like Name(
esc=sorted(set(names), key=len, reverse=True)
pat=re.compile(r"\b(" + "|".join(re.escape(n) for n in esc) + r")\s*\(")
rows=[]
for f in sorted(fns,key=lambda x:int(x["address"],16)):
    txt=open(os.path.join(base,f["relative_path"])).read()
    body=txt.split("// pseudocode:",1)[-1]
    calls=sorted(set(m.group(1) for m in pat.finditer(body)))
    vaddr=hex(int(f["address"],16)-0x100000)
    rows.append((f["address"],vaddr,f["name"],calls))
want=sys.argv[1] if len(sys.argv)>1 else "."
rx=re.compile(want)
for a,v,n,c in rows:
    if n.startswith("FUN_"):
        hits=[x for x in c if rx.search(x)]
        if hits: print(f"{v} {n} :: " + ", ".join(hits))
