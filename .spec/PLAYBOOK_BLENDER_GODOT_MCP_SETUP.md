# MCP 設定 Playbook — agent-switch + opencode

> v1 · 2026-10-26 · 讓 opencode（經由 agent-switch，可接本地模型）跑 `PLAYBOOK_v2.md` 時用得到 Godot MCP 與 Blender MCP
> 本機實測：agent-switch（`--no-launch` dry run）、opencode v2.0.3（`~/.bun/bin/opencode2`）、Godot 4.7.2、Blender 5.2.2、mcp-for-blender addon 1.8

---

## 這份文件解決什麼

PLAYBOOK_v2 的 Phase 1.3 / Phase 2 是照 Claude Code 寫的（`.mcp.json`、`.claude/settings.json`）。換成 opencode 時，MCP 改由 agent-switch 在啟動時掛上去：

```text
agent-switch opencode ──寫出設定──► ~/.agent-switch/agents/opencode-*/opencode.json
        │                                   │
        └──啟動──► opencode2 --standalone ◄──讀（OPENCODE_CONFIG）
                        │
                        ├─ stdio ─► npx -y @coding-solo/godot-mcp ──呼叫──► Godot CLI（GODOT_PATH）
                        └─ stdio ─► uvx blender-mcp ──TCP 9876──► Blender 裡的 MCP addon
```

- agent-switch 不改你自己的 opencode 設定，每次啟動都產生一份新的
- Blender MCP 要先在 Blender 裡啟動 server，opencode 端的 `blender-mcp` 只是轉接器
- Godot MCP 不需要先開 Godot，它每次呼叫都用 `GODOT_PATH` 啟動 headless Godot

---

## 1. 前置軟體

| 軟體 | 用途 | 檢查 |
|---|---|---|
| Godot ≥ 4.3 | godot-mcp 呼叫它 | `<godot> --version` |
| Node.js ≥ 18（含 npx） | 啟動 godot-mcp | `node --version` |
| uv（含 uvx） | 啟動 blender-mcp | `uv --version` |
| Blender 4.x / 5.x | 實際建模 | 開得起來即可 |
| agent-switch | 啟動 opencode 並掛 MCP | `agent-switch opencode --help` |
| opencode | agent | `agent-switch opencode --no-launch` 會印出它用哪一支 binary |

---

## 2. Blender 端（人工）

1. 安裝 Blender MCP addon：`uvx mcp-for-blender install-addon`（目前的 `blender-mcp` 套件就是 mcp-for-blender），或依 blender-mcp README 從 Preferences → Add-ons → Install from Disk 裝 `addon.py`
2. Preferences → Add-ons 勾選「Interface: Blender MCP」
3. **Blender 5.x：停用內建的 Blender Lab「MCP」擴充**。它也聽 9876 埠，協定不同，blender-mcp 會握手逾時，但只測 TCP 的檢查會誤判成功
4. 3D 視窗按 `N` → BlenderMCP 分頁 → 外部資產庫和生成器都不要勾 → Start MCP Server
5. 確認埠是 Blender 開的：

```bash
ss -ltnp | grep 9876      # 應看到 users:(("blender",...))
```

Blender 每次重開都要再按一次 Start MCP Server。

---

## 3. 掛載 MCP

有兩種做法。兩者都能和 `--url` / `--provider`（本地模型）一起用。

| 模式 | MCP 的處理 |
|---|---|
| 不帶 `--url` / `--provider`（native） | 用 opencode 自己的模型與設定，掛上的 MCP **加到**它原本的 MCP |
| 帶 `--url` / `--provider` | 用拋棄式設定，掛上的 MCP **取代**它原本的 MCP |

### 3A. 單次旗標（已實測）

```bash
agent-switch opencode \
  --mcp-stdio "godot:npx -y @coding-solo/godot-mcp" \
  --mcp-env   "godot:GODOT_PATH=/home/scott/tools/bin/godot" \
  --mcp-stdio "blender:uvx blender-mcp" \
  --mcp-env   "blender:DISABLE_TELEMETRY=true"
# 接本地模型時再加：--url <NAME:URL> --model <NAME:ID>
```

- `GODOT_PATH` 換成你的 Godot 路徑
- `DISABLE_TELEMETRY=true` 關掉 blender-mcp 預設的 telemetry，不需要可拿掉

agent-switch 實際寫出的 opencode 設定（native 模式在 `~/.agent-switch/agents/opencode-native/opencode.json`）：

```json
{
  "$schema": "https://opencode.ai/config.json",
  "mcp": {
    "godot": {
      "type": "local",
      "command": ["npx", "-y", "@coding-solo/godot-mcp"],
      "environment": { "GODOT_PATH": "/home/scott/tools/bin/godot" },
      "enabled": true
    },
    "blender": {
      "type": "local",
      "command": ["uvx", "blender-mcp"],
      "environment": { "DISABLE_TELEMETRY": "true" },
      "enabled": true
    }
  }
}
```

### 3B. Registry（常用時省得每次打旗標；依 `--help` 說明，本機尚未實測）

`--help` 的說明：registry 在 `~/.agent-switch/mcp.json`，格式和 `.mcp.json` 相同；`--mcp NAME` 掛其中一個，`--mcp-all` 全掛。

```json
{
  "mcpServers": {
    "godot": {
      "command": "npx",
      "args": ["-y", "@coding-solo/godot-mcp"],
      "env": { "GODOT_PATH": "/home/scott/tools/bin/godot" }
    },
    "blender": {
      "command": "uvx",
      "args": ["blender-mcp"],
      "env": { "DISABLE_TELEMETRY": "true" }
    }
  }
}
```

```bash
agent-switch opencode --mcp godot --mcp blender
```

建好 registry 後，先照第 4 節的 dry run 確認產生的 `opencode.json` 和 3A 一樣，再正式使用。

---

## 4. 驗證

```text
dry run（--no-launch）──► 拿到 OPENCODE_CONFIG 與 binary ──► mcp list 兩個都 connected ──► 進 opencode 呼叫工具
```

1. Dry run：把 3A（或 3B）的指令加上 `--no-launch`，它不會啟動 opencode，只印出：

```text
opencode runs with its own model, login and config.
export OPENCODE_CONFIG=/home/scott/.agent-switch/agents/opencode-native/opencode.json
OPENCODE_CONFIG=/home/scott/.agent-switch/agents/opencode-native/opencode.json /home/scott/.bun/bin/opencode2 --standalone
```

2. 用**同一支 binary** 查連線狀態（變數要 `export`，原因見第 6 節）：

```bash
export OPENCODE_CONFIG=/home/scott/.agent-switch/agents/opencode-native/opencode.json
/home/scott/.bun/bin/opencode2 mcp list
# ✓ blender  connected
# ✓ godot    connected
```

3. 正式啟動（拿掉 `--no-launch`），在 opencode 裡請它依序呼叫 Godot MCP 的 `get_godot_version`、Blender MCP 的 `get_addon_status` 與 `get_scene_info`。三個都成功，就等於 PLAYBOOK_v2 Phase 2 的驗證通過

---

## 5. 套用到 PLAYBOOK_v2

| PLAYBOOK_v2 原本寫的（Claude Code） | 換成 agent-switch + opencode |
|---|---|
| Phase 1.3 `.mcp.json` | opencode 不靠它；MCP 由第 3 節掛載。內容可以直接搬進 3B 的 registry（格式相同） |
| Phase 1.3 `.claude/settings.json` 允許清單 | 不適用。opencode 每次呼叫工具會詢問；`--yolo` 可略過詢問（見下方注意） |
| Phase 1.9 `CLAUDE.md` | opencode 的規則檔是 `AGENTS.md`；放一份內容相同的 `AGENTS.md`，確保它讀得到 |
| Phase 2「退出 agent 再重新啟動」 | 離開 opencode，重跑第 3 節的 agent-switch 指令 |
| Phase 2 驗證 `mcp__godot__get_godot_version` 等 | 用第 4 節步驟 3；工具名稱以 opencode 裡顯示的為準（Claude Code 的 `mcp__<server>__<tool>` 前綴是它自己的命名） |
| 附錄 C-1（9876 被 Blender Lab MCP 佔用） | 一樣適用，見第 2 節第 3 步 |

**`--yolo` 的注意：** Blender MCP 的 `execute_blender_code` 會在 Blender 裡執行任意 Python。PLAYBOOK_v2 刻意不自動允許它；用 `--yolo` 等於全部自動允許，只在可拋棄的環境用。

---

## 6. 常見問題

| 現象 | 原因 | 做法 |
|---|---|---|
| `mcp list` 說 `No MCP servers configured` | `OPENCODE_CONFIG=... opencode mcp list` 這種前綴寫法；本機的 `opencode` 是 shell function（`command opencode --log-level debug "$@"`），實測變數沒傳進去 | 先 `export OPENCODE_CONFIG=...` 再執行，或直接用 dry run 印出的 binary |
| `mcp list` 在 `cli.server-connection.resolve` 出錯 | 本機有兩套 opencode：`~/.bun/bin/opencode(2)` 是 v2.0.3，`~/.opencode/bin/opencode` 是 v2.1.4；直接執行後者會報這個錯，且不接受 `--standalone` | 驗證時用 dry run 印出的那支（目前是 `~/.bun/bin/opencode2`） |
| blender 一直連不上、逾時 | 9876 被 Blender Lab「MCP」擴充佔用，或 Blender 裡還沒 Start MCP Server | 第 2 節；`ss -ltnp \| grep 9876` 確認是 blender 在聽 |
| 第一次啟動很慢或 MCP 逾時 | `npx` / `uvx` 第一次要下載套件 | 等它下載完再重跑一次；之後會用快取 |
| 找不到 `get_viewport_screenshot` | 目前的 blender-mcp（mcp-for-blender）沒有這個工具 | 改用 `look`（PLAYBOOK_v2 已照此寫） |
| godot 工具報找不到 Godot | `GODOT_PATH` 沒設或路徑錯 | `--mcp-env "godot:GODOT_PATH=..."` 指到實際的 Godot 執行檔 |

---

## 參考

- `agent-switch opencode --help`（MCP servers、Native launch 兩段）
- `opencode mcp --help`、`opencode mcp add --help`
- godot-mcp：https://github.com/Coding-Solo/godot-mcp
- blender-mcp：https://github.com/ahujasid/blender-mcp
- 遊戲流程本體：`.spec/PLAYBOOK_v2.md`
