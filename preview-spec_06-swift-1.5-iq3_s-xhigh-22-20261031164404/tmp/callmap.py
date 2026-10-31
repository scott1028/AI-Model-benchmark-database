import json,os,re,sys
base="/home/scott/workspace/AI-Model-benchmark-database/preview-wip/tmp/rea_decompile"
idx=json.load(open(os.path.join(base,"decompile-index.json")))
fns=[f for f in idx["functions"] if f.get("status")=="written"]
pat=re.compile(r"<EXTERNAL>::([A-Za-z_][A-Za-z0-9_:~]*)")
rows=[]
for f in sorted(fns,key=lambda x:int(x["address"],16)):
    txt=open(os.path.join(base,f["relative_path"])).read()
    calls=sorted(set(m.group(1) for m in pat.finditer(txt)))
    vaddr=hex(int(f["address"],16)-0x100000)
    rows.append((f["address"],vaddr,f["name"],calls,txt))
want=sys.argv[1] if len(sys.argv)>1 else None
rx=re.compile(want) if want else None
for a,v,n,c,txt in rows:
    hits=[x for x in c if rx.search(x)] if rx else None
    if hits:
        print(f"{a} {v} {n} :: {', '.join(hits)}")
