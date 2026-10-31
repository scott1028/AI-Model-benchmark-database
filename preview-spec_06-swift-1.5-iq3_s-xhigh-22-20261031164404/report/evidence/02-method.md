# 02 — Method & toolchain

All work is user-space, read-only against the system, official sources only.

| Tool | Version | Location | Use |
|---|---|---|---|
| REA MCP server (reverse-engineering harness) | session-connected, provider `rea-workflow` | user-space MCP config | open target, analysis, `decompile_all`, `batch_decompile`, xrefs |
| Ghidra (REA decompiler provider) | 12.1.4 | `/home/scott/tools/ghidra` (official release, user space) | full-binary decompilation → `tmp/rea_decompile/` (830 exported functions; base 0x100000) |
| GNU binutils (objdump/readelf/strings/nm) | 2.46 | distro `/usr/bin` | disassembly `.rodata` hex, PLT tables, dynamic symbols |
| GDB | 17.1 | distro | PLT-stub argument probes (`tmp/gdb_probe*.py`) |
| Python | 3.12 | distro | helpers (`tmp/fidx.py`, `tmp/callmap2.py`, `tmp/callees.py`), http.server for verification |
| Google Chrome (headless, via Playwright) | 156.0.8078.79 | distro | final browser verification (redirect, console errors, render check) |

Address convention: REA/Ghidra function address = objdump vaddr + `0x100000`.

## Pipeline

1. `file`, `readelf -h/-d`, `nm -D` → format, PIE, imports (Qt6/KF6/KDEGames6/KXmlGui).
2. `objdump -d` + `strings` (ASCII, UTF-16) + `.rodata` hex → Qt meta-strings, i18n
   strings, config keys (`kcfg_*`), sprite keys (UTF-16), embedded `kminesui.rc`
   (recovered verbatim at `.rodata 0x23364..0x2354f`).
3. Ghidra full decompile (REA `decompile_all`); targeted `batch_decompile` for gaps.
4. PLT cross-reference map (`tmp/callmap2.py`): every library call site → owner.
5. Dynamic probes (non-destructive, `QT_QPA_PLATFORM=offscreen`, `$HOME` redirected
   to `tmp/fakehome/`): PLT-stub breakpoints recorded live
   `KGameRenderer::spritePixmap(key, QSize)` sizes, `KGameDifficultyLevel(30|40|50,…)`
   ctor args, `KConfigSkeleton::ItemInt(…, default)` values. Logs: `tmp/gdb_run*.log`,
   `tmp/probe*.log`.
6. Companion installed files: `/usr/share/kmines/themes/*.svgz` (layer ids = sprite
   keys), `/usr/share/doc/HTML/en/kmines/index.docbook` (difficulty board sizes),
   observed `~/.config/kminesrc`.

## Non-destructive guarantees

- No `sudo`, no writes to system paths; `/usr/games/kmines` opened read-only.
- Game runs used a sandboxed `$HOME` (`tmp/fakehome/`) so no user/system config touched.
- All temp files inside `preview-wip/tmp/`.
- No upstream/package source, no downloaded assets, no other `preview-*` outputs used.
