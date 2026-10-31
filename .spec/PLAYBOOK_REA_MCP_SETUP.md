# REA MCP 設定 Playbook — agent-switch + Claude Code

> v1 · 2026-10-31 · 讓 Claude Code（經由 agent-switch）跑 `spec_06_rea_kmines-PLAYBOOK.md` 這類逆向任務時，用得到 REA MCP（Ghidra 反編譯）與 REA skill
> 本機實測：REA 6.8.0（原始碼建置）、Node.js 24.22.2（nvm）、Ghidra 12.1.4、Eclipse Temurin JDK 21.0.13、Claude Code 2.1.316、Linux x64

---

## 這份文件解決什麼

用一行 agent-switch 指令，替這次啟動的 Claude Code 掛上 REA 的 MCP server 和 skill，不動你原本的 Claude Code 設定。agent-switch 每次啟動都另外產生一份設定，像在影本上加註記，原稿不動。

```text
agent-switch claude-personal
   │
   ├─寫出──► ~/.agent-switch/agents/claude-native/mcp.json   （rea 的啟動指令與 env）
   ├─連結──► ~/.agent-switch/agents/claude-native/agent-switch/.claude/skills/reverse-engineer-anything
   │                                                        └─symlink─► <REA>/skills/reverse-engineer-anything
   └─啟動──► claude-personal --mcp-config=<mcp.json> --add-dir=<…/claude-native/agent-switch>
                 │
                 └─ stdio ─► node <REA>/scripts/rea.mjs mcp ──呼叫──► Ghidra headless（GHIDRA_INSTALL_DIR、JAVA_HOME）
```

- MCP 是工具箱（233 個逆向工具），skill 是使用手冊（告訴 agent 何時、怎麼用這些工具）；兩個都要掛
- 不用先開 Ghidra：REA 第一次查詢時才用 headless 模式啟動它
- 不跑 `rea setup`：setup 會改各 agent 的使用者設定；這份做法只在啟動時掛上，關掉就沒了

### 名詞

| 名詞 | 白話說明 |
|---|---|
| REA | Reverse Engineer Anything，把反編譯器、binary 分析等工具包成一個 MCP server 的開源專案（`morluto/rea`） |
| stdio MCP server | agent 自己啟動的子行程，用 stdin / stdout 和 agent 溝通 |
| provider | REA 背後真正做反編譯的引擎；這份文件只用 Ghidra |
| skill | 一個含 `SKILL.md` 的資料夾，agent 讀它來決定怎麼用 REA |
| native 模式 | agent-switch 不帶 `--url` / `--provider` 時，agent 用自己的模型和登入，掛上的 MCP 加在它原本的 MCP 旁邊 |

---

## 1. 前置軟體

| 軟體 | 版本需求 | 用途 | 檢查 |
|---|---|---|---|
| Node.js（含 npm） | 22.x ≥ 22.23.4、24.x ≥ 24.22.1，或 ≥ 26.12.1 | 建置並執行 REA | `node --version` |
| git | — | 抓 REA 原始碼 | `git --version` |
| Ghidra | 12.x ≥ 12.1 | 反編譯 native binary | `grep application.version <ghidra>/Ghidra/application.properties` |
| JDK | 64-bit 完整 JDK（含 `javac`）；12.1 要 21 以上，12.2 要 25 以上 | 執行 Ghidra | `<jdk>/bin/java -version`、`<jdk>/bin/javac -version` |
| procps 的 `ps` | — | Linux 上 REA 啟動 Ghidra 前，用它確認子行程是自己開的 | `ps --version`（應顯示 procps-ng） |
| agent-switch | — | 啟動 Claude Code 並掛 MCP 與 skill | `agent-switch claude --help` |
| Claude Code | 已有同名 skill 時要 ≥ 2.1.309 才看得到注入的那份 | agent | `claude --version` |
| jq（選用） | — | 第 4 節用來濃縮輸出，不裝就看完整 JSON | `jq --version` |

JDK 的確切下限看 Ghidra 的 `Ghidra/application.properties` 裡 `application.java.min`。

---

## 2. 安裝 REA（從原始碼）

指令裡的 `scripts/rea.mjs` 和 `skills/reverse-engineer-anything` 都在 REA repo 裡，所以從原始碼裝。

```bash
git clone https://github.com/morluto/rea.git ~/workspace/rea   # 用自己的 fork 也可以
cd ~/workspace/rea
nvm install            # 依 .nvmrc 裝 24.22.1；任何符合第 1 節版本需求的 Node 都可以
npm ci
npm run build:cached
ls dist/mcp/main.js    # 有這個檔就是建好了
```

- 建置指令取自 `scripts/rea.mjs` 缺 runtime 時印出的提示；本機 `dist/` 已建好，這步未重跑
- 不需要 `--recurse-submodules`：本機 submodule 都沒 checkout，第 4 節的 Ghidra 流程照樣通過
- 記下這支 node 的絕對路徑，第 5 節要用（例如 `~/.nvm/versions/node/v24.22.2/bin/node`）

---

## 3. 安裝 Ghidra 與 JDK（使用者空間，不用 sudo）

兩個都從官方下載、解壓到 `~/tools`，再用固定名稱的 symlink 指過去，之後升級只要換連結。

```text
官方 zip    ──解壓──► ~/tools/ghidra_12.1.4_PUBLIC ──symlink──► ~/tools/ghidra
官方 tar.gz ──解壓──► ~/tools/jdk-21.0.13+9        ──symlink──► ~/tools/jdk
```

1. Ghidra：從 https://github.com/NationalSecurityAgency/ghidra/releases 下載 `ghidra_12.1.4_PUBLIC_*.zip`
2. JDK：從 https://adoptium.net/temurin/releases/ 下載 JDK 21，選 Linux、x64、`.tar.gz`
3. 解壓並建連結（資料夾名稱以實際解壓出來的為準）：

```bash
mkdir -p ~/tools && cd ~/tools
unzip ~/Downloads/ghidra_12.1.4_PUBLIC_*.zip
ln -sfn ghidra_12.1.4_PUBLIC ghidra
tar xzf ~/Downloads/OpenJDK21U-jdk_x64_linux_hotspot_*.tar.gz
ln -sfn jdk-21.0.13+9 jdk
```

本機的 Ghidra 和 JDK 已經裝好，這節的下載與解壓未重跑；第 4 節的檢查已實測通過。

---

## 4. 先不經 agent，確認 REA 和 Ghidra 能動

先在 shell 裡把三件事各跑一次。這裡過了，之後 agent 裡出問題就只剩掛載設定要查。

```text
doctor（Ghidra 設定對不對）──► mcp doctor（MCP 能不能啟動）──► decompile（真的跑一次 Ghidra）
```

```bash
NODE=$HOME/.nvm/versions/node/v24.22.2/bin/node
REA=$HOME/workspace/rea/scripts/rea.mjs
export GHIDRA_INSTALL_DIR=$HOME/tools/ghidra JAVA_HOME=$HOME/tools/jdk

# 1) Ghidra 設定
"$NODE" "$REA" doctor --provider ghidra --json \
  | jq -c '{healthy, checks: [.scope_checks[] | "\(.name): \(.classification)"]}'

# 2) MCP 啟動
"$NODE" "$REA" mcp doctor --json \
  | jq -c '{healthy, server, tools: (.inventory.tools | {expected, observed})}'

# 3) 反編譯 /usr/bin/true 的 entry（唯讀，本機約 7 秒）
"$NODE" "$REA" decompile /usr/bin/true entry --provider ghidra --json \
  | jq -c '{format: .subject.format, arch: .subject.architecture, provider}'
```

本機實測輸出：

```text
{"healthy":true,"checks":["node: healthy","ghidra: healthy","ghidra-platform: healthy","ghidra-architecture: healthy","ghidra-installation: healthy","ghidra-version: healthy","ghidra-headless: healthy","ghidra-native_decompiler: healthy","ghidra-java: healthy"]}
{"healthy":true,"server":{"name":"rea","version":"6.8.0","protocol_version":"2025-11-25"},"tools":{"expected":233,"observed":233}}
{"format":"elf","arch":"x86_64","provider":{"id":"ghidra","name":"Ghidra","version":"12.1.4"}}
```

`doctor` 的完整輸出還會有 `environment_healthy: false`、`registration:* config_drift`，以及 hopper / ida / jadx / kuna 的 `missing_analysis_engine`。那些是 `rea setup` 管的註冊和其他引擎，這份做法用不到，看 `healthy` 和上面九項就好。

---

## 5. 掛上 MCP 和 skill，啟動 Claude Code

### 5A. 單次旗標（已實測 dry run）

```bash
agent-switch claude-personal \
  --mcp-stdio "rea:$HOME/.nvm/versions/node/v24.22.2/bin/node $HOME/workspace/rea/scripts/rea.mjs mcp" \
  --mcp-env   "rea:GHIDRA_INSTALL_DIR=$HOME/tools/ghidra" \
  --mcp-env   "rea:JAVA_HOME=$HOME/tools/jdk" \
  --skill     "$HOME/workspace/rea/skills/reverse-engineer-anything"
# 接本地模型時再加：--url <NAME:URL> --model <NAME:ID>
```

| 片段 | 作用 | 換成你的 |
|---|---|---|
| `claude-personal` | 要啟動的 agent；這裡是另一個 Claude Code profile（用 `CLAUDE_CONFIG_DIR` 指到獨立設定目錄的 shell function） | 用預設 profile 就寫 `claude` |
| `--mcp-stdio "rea:<node> <rea.mjs> mcp"` | 掛一個名叫 `rea` 的 stdio MCP server | 第 2 節記下的 node 路徑、你的 REA 路徑 |
| `--mcp-env "rea:GHIDRA_INSTALL_DIR=…"` | 只設在 `rea` 子行程的 env：Ghidra 位置 | 第 3 節的 Ghidra 路徑 |
| `--mcp-env "rea:JAVA_HOME=…"` | 只設在 `rea` 子行程的 env：Ghidra 用的 JDK | 第 3 節的 JDK 路徑 |
| `--skill <path>` | 把 REA skill 資料夾 symlink 進 agent-switch 的 skill 目錄 | REA repo 裡的 `skills/reverse-engineer-anything` |

三件事一定要記得：

- **node 寫絕對路徑。** MCP 子行程由 Claude Code 直接啟動，不經過 nvm；PATH 上的 `node` 可能是版本不符的那支，例如 nvm 預設是 22.22.x 時就低於 22.23.4
- **在 shell 裡直接打，不要用單引號包 `$HOME`。** `$HOME` 要在執行當下展開，產生的 `mcp.json` 裡才會是絕對路徑
- **native 模式下 `rea` 是「加上去」的。** 不帶 `--url` / `--provider` 時 Claude Code 用自己的模型和登入，原本的 MCP 照樣在；帶了 `--url` / `--provider`，掛上的 MCP 會取代原本的 MCP

加上 `--no-launch` 就只印出它會怎麼啟動，不真的啟動（家目錄以 `<you>` 代替）：

```text
Injected skills: reverse-engineer-anything. If one shares a name with an existing skill, Claude Code 2.1.309 or newer lists it as agent-switch:<name>; older versions hide it.
claude-personal runs with its own model, login and config.
bash -ic 'claude-personal "$@"' agent-switch --mcp-config=/home/<you>/.agent-switch/agents/claude-native/mcp.json --add-dir=/home/<you>/.agent-switch/agents/claude-native/agent-switch
```

agent-switch 寫出的 `~/.agent-switch/agents/claude-native/mcp.json`：

```json
{
  "mcpServers": {
    "rea": {
      "type": "stdio",
      "command": "/home/<you>/.nvm/versions/node/v24.22.2/bin/node",
      "args": [
        "/home/<you>/workspace/rea/scripts/rea.mjs",
        "mcp"
      ],
      "env": {
        "GHIDRA_INSTALL_DIR": "/home/<you>/tools/ghidra",
        "JAVA_HOME": "/home/<you>/tools/jdk"
      }
    }
  }
}
```

skill 的放法：

```text
~/.agent-switch/agents/claude-native/agent-switch/.claude/skills/
├─ .agent-switch-skills.json     （記錄這次注入了哪些 skill）
└─ reverse-engineer-anything ──symlink──► ~/workspace/rea/skills/reverse-engineer-anything
```

skill 是 symlink，所以在 REA repo 裡 `git pull` 之後，下次啟動就是新版 skill，不用重新複製。

### 5B. Registry（常用時省得每次打旗標；依 `--help` 說明，本機尚未實測）

`--help` 的說明：registry 在 `~/.agent-switch/mcp.json`，格式和 `.mcp.json` 相同；`--mcp NAME` 掛其中一個，`--mcp-all` 全掛。JSON 不會展開 `$HOME`，路徑要寫完整：

```json
{
  "mcpServers": {
    "rea": {
      "command": "/home/<you>/.nvm/versions/node/v24.22.2/bin/node",
      "args": ["/home/<you>/workspace/rea/scripts/rea.mjs", "mcp"],
      "env": {
        "GHIDRA_INSTALL_DIR": "/home/<you>/tools/ghidra",
        "JAVA_HOME": "/home/<you>/tools/jdk"
      }
    }
  }
}
```

registry 只管 MCP，skill 還是要用 `--skill` 帶：

```bash
agent-switch claude-personal --mcp rea \
  --skill "$HOME/workspace/rea/skills/reverse-engineer-anything"
```

建好 registry 後，先用 `--no-launch` 確認產生的 `mcp.json` 和 5A 一樣，再正式使用。

---

## 6. 在 Claude Code 裡驗證

第 4 節已經證明 REA 本身沒問題，這裡只確認 Claude Code 有接上。

```text
dry run 看 mcp.json 與 skill 連結 ──► 啟動後 /mcp 看到 rea connected ──► 請 agent 用 ghidra 反編譯一支小 binary
```

1. Dry run（已實測）：5A 的指令加 `--no-launch`，再確認兩個產物：

```bash
cat ~/.agent-switch/agents/claude-native/mcp.json
ls -l ~/.agent-switch/agents/claude-native/agent-switch/.claude/skills/
```

2. 正式啟動（拿掉 `--no-launch`），在 Claude Code 裡輸入 `/mcp`，應看到 `rea` 是 connected
3. 貼這段提示詞：

```text
用 REA 的 ghidra provider 開 /usr/bin/true，列出它的前 10 個 procedures，再反編譯 entry。告訴我你用了哪些 REA 工具、provider 版本是多少。
```

agent 應該呼叫 `open_binary`、`list_procedures` 等 REA 工具（Claude Code 裡顯示成 `mcp__rea__<tool>`），回報 provider 是 Ghidra 12.1.4。

步驟 2、3 本機尚未在 Claude Code 裡實測；工具名稱以 `/mcp` 裡顯示的為準。

---

## 7. 常見問題

| 現象 | 原因 | 做法 |
|---|---|---|
| `REA's compiled runtime is missing` | REA 還沒建置 | 在 REA repo 跑 `npm ci && npm run build:cached` |
| `/mcp` 裡 `rea` 失敗，或 REA 報 Node 版本不符 | `--mcp-stdio` 寫了 `node`，用到 PATH 上版本不符的那支 | 改成 node 絕對路徑（第 5 節） |
| `doctor` 的 `ghidra-java` 不是 healthy | `JAVA_HOME` 指到 JRE（沒有 `javac`）、32-bit JDK，或版本低於 `application.java.min` | 換成 64-bit 完整 JDK |
| `doctor` 顯示 `environment_healthy: false`、`registration:* config_drift` | 那些是 `rea setup` 管的 agent 註冊 | 這份做法不用 setup，可忽略；`healthy` 是 `true` 就好 |
| 大型 binary 第一次查詢很久或逾時 | Ghidra 第一次查詢時才匯入並自動分析 | 加 `--mcp-env "rea:REA_GHIDRA_STARTUP_TIMEOUT_MS=1800000"`（單位 ms，這裡是 30 分鐘） |
| 看不到 REA skill | 已有同名 skill，Claude Code < 2.1.309 會把注入的那份藏起來 | 升級 Claude Code；新版會列成 `agent-switch:reverse-engineer-anything` |
| Linux 上 Ghidra 起不來，REA 說缺 procps 相容的 `ps` | REA 啟動子行程前要用 `ps` 確認歸屬 | 安裝發行版的 `procps` / `procps-ng`（需要系統管理員權限） |

---

## 參考

- `agent-switch claude-personal --help`（MCP servers、Native launch 兩段）
- REA：https://github.com/morluto/rea
  - `docs/installation.md` 的「Ghidra」與「Linux owned-process prerequisite」兩節
  - `docs/cli.md` 的 `rea mcp doctor --json`
  - `skills/reverse-engineer-anything/SKILL.md`
- Ghidra releases：https://github.com/NationalSecurityAgency/ghidra/releases
- Eclipse Temurin：https://adoptium.net/temurin/releases/
- 同系列：`.spec/PLAYBOOK_BLENDER_GODOT_MCP_SETUP.md`（Godot / Blender MCP + opencode）
- 逆向任務本體：`.spec/spec_06_rea_kmines-PLAYBOOK.md`
