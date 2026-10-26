# Blender × Godot MCP 遊戲 Pipeline — 箱庭版 Playbook v2

> v2 · 2026-10-26 · 給 Claude Code（或其他 coding agent）從空資料夾重跑
> 由 v1（`.spec/PLAYBOOK.md`）加上 Phase 8–10 的使用者回饋整合而成：一路直接做出最終版，不先做舊版再推翻。

---

## 給人看的：怎麼開始

1. 建一個空資料夾，把本檔放到 `.spec/PLAYBOOK_v2.md`
2. 在該資料夾執行 agent，輸入：`讀 .spec/PLAYBOOK_v2.md，從頭執行。遇到 STOP 就停下來等我。`
3. 中斷或重啟後輸入：`讀 .spec/PLAYBOOK_v2.md 和 pipeline/progress.md，從未完成的步驟繼續。`

需要人手動做的只有三件事：
- Phase 0：安裝軟體
- Phase 1.4：如果 agent 被權限擋下，手動建立 `.claude/settings.json`
- Phase 2：開 Blender 裝 addon、啟動 server，然後重啟 agent

---

## 目標

做一個 3D 箱庭小遊戲「Coin Run」：單輪小機器人在 40×40 m、四周有磚牆的浮空地塊上移動，收集 3 枚金幣後碰到神殿前的旗子就勝利。

- 所有 3D 模型（含地形）都由 Blender MCP 產生；Godot MCP 負責組裝與執行；規格不符時回 Blender 調整
- 畫面：等角 45° 跟隨鏡頭、暖色斜陽與柔影、SSAO、移軸景深、漂移的低模雲與雲影、隨風擺動的樹和草叢
- 物理：力向量移動 + 摩擦、加速 / 煞車、機身依加速度傾斜、輪子依速度滾動、撞樹搖晃落葉、可推動的木桶、穿過草叢會晃

以下選擇已經和使用者確認，**執行時不要再問**：

| 項目 | 定案 |
|---|---|
| 地圖 | 40×40 m，四周磚瓦牆；地塊浮在淡藍天空中 |
| 角色 | 單輪小機器人，高 1.2 m；輪子動畫 `wheel_spin` 由 Blender 製作 |
| 鏡頭 | 等角 45° 平滑跟隨；方向鍵相對鏡頭移動（bot 仍走世界座標直線） |
| 奇幻建築 | 法師塔 + 神殿遺跡 |
| 按鍵 | 加速：鍵盤 A / 手把 X；煞車：鍵盤 B / 手把 B；跳躍：Space / Enter / 手把 A |
| 物理加碼 | 可推動木桶、草叢經過會晃、機器人加速傾斜 |
| 氣氛 | 雲漂移 + 雲影、暖色陽光 + 柔影 + SSAO、微風擺動、移軸景深 |
| 地面 | 低模地形，起伏約 −0.35 ~ +0.9 m；草地色塊、石板廣場、通往神殿的石板路、泥土路、草叢花朵與小石子 |

### 完成定義（DoD）

- [ ] 14 個資產的 `check_assets` 全部 PASS（player / coin / crate / goal_flag / tree / bush / rock / wall_brick / house / mage_tower / temple_ruins / barrel / terrain / cloud）
- [ ] `node tools/bot_test.mjs` PASS：輸出 `COIN 3/3` 和 `WIN`，沒有 `SCRIPT ERROR` / `ERROR:`
- [ ] `node tools/physics_test.mjs` PASS（Phase 9 的 9 項檢查）
- [ ] Godot MCP `run_project` 後 `get_debug_output` 無錯誤；跟隨截圖與全景截圖依 Phase 9 清單檢查過
- [ ] 完成一次「迴圈演練」（coin 規格變更 → FAIL → Blender 調整 → PASS），並留有紀錄
- [ ] `pipeline/REPORT.md` 已寫

---

## Pipeline 總覽

```text
assets.json（資產規格）          world_layout.json（版面：物件、道路、廣場、平整區塊）
      │                                  │
      ▼                                  ▼
Blender MCP ── 建模 → look 自檢 → 附錄 A0 + A 匯出 ──► game/assets/<id>.glb + pipeline/blender_stats/<id>.json
      │                                  └─ 地形另外寫出 pipeline/terrain_heights.json
      ▼
node tools/import.mjs → node tools/check_assets.mjs <id>
      ├─ FAIL → pipeline/log.md（附錄 B）→ 回 Blender（每資產每輪最多 3 次）
      └─ PASS
           ▼
node tools/build_world.mjs（world_layout + 高度表 → game/scenes/world.tscn）
           ▼
Godot MCP 建 main.tscn 與道具場景 → bot_test / physics_test → run_project + 截圖檢查
```

---

## 給 agent 的執行規則

1. 每完成一個編號步驟，就在 `pipeline/progress.md` 打勾（`[v]` 通過、`[x]` 做了沒過、`[ ]` 未做）。重啟後靠它續跑。
2. 看到 **STOP**：清楚告訴使用者要做什麼，結束這一輪，等使用者回覆。除了 STOP 和超過 maxIterations，不要停下來問。
3. 每個 Phase 完成後執行 `git commit`。
4. 本檔寫死的檔案內容要照抄；只給規格的程式由你實作，但必須符合規格、常數與驗收標準。
5. MCP 工具失敗時：先重試 1 次 → 改用替代做法（例如直接編輯檔案）→ 記到 `pipeline/log.md` 的「MCP 問題」段落。
6. `pipeline/progress.md` 的內容 = 本檔各 Phase 的編號步驟清單。
7. 附錄 C 是已知的坑，動手前先讀過。

---

## Phase 0：環境檢查（人工安裝，agent 只檢查）

| 軟體 | 版本 | 備註 |
|---|---|---|
| Godot | ≥ 4.3（本次用 4.7.2） | Windows 請用 `*_console.exe`，才抓得到 stdout |
| Blender | 4.x / 5.x（本次用 5.2.2 LTS） | |
| Node.js | ≥ 18 | godot-mcp 需要 |
| uv | 最新 | blender-mcp 透過 `uvx` 啟動 |
| git | 任意 | |

步驟：
1. 執行 `node --version`、`uv --version`、`git --version`。
2. 依序找 Godot 執行檔：環境變數 `GODOT_PATH` → PATH 裡的 `godot` / `godot4` → macOS `/Applications/Godot.app/Contents/MacOS/Godot` → 都找不到就問使用者。
3. 執行 `<godot> --version`，確認版本 ≥ 4.3。
4. 任一項缺少 → **STOP**，列出缺什麼、去哪裡下載。

---

## Phase 1：專案骨架

目錄結構：
```text
./
├─ .spec/PLAYBOOK_v2.md
├─ CLAUDE.md
├─ .mcp.json
├─ .claude/settings.json
├─ .gitignore
├─ pipeline.config.json
├─ assets.json
├─ world_layout.json
├─ pipeline/  progress.md、log.md、blender_stats/
├─ tools/     check_env.mjs、import.mjs、check_assets.mjs、bot_test.mjs、physics_test.mjs、build_world.mjs
├─ blender/   assets.blend（之後由 Blender 存檔產生）
└─ game/
   ├─ project.godot
   ├─ assets/   （.glb）
   ├─ scenes/   main.tscn、world.tscn（產生）、tree.tscn、bush.tscn、barrel.tscn、test_crate.tscn
   ├─ scripts/  main.gd、player.gd、coin.gd、goal.gd、sway.gd、tree.gd、bush.gd、clouds.gd、wind.gd
   └─ tools/    inspect_assets.gd、physics_test.gd
```

### 1.1 `pipeline.config.json`

```json
{
  "projectRoot": "<專案絕對路徑>",
  "godotPath": "<Phase 0 找到的 Godot 路徑>",
  "blenderPort": 9876,
  "maxIterations": 3,
  "sizeTolerance": { "relative": 0.10, "absoluteMin": 0.02 }
}
```

### 1.2 `assets.json`（照抄）

軸向一律用 Godot 座標：`size_m = [X 寬, Y 高, Z 深]`，單位公尺。coin 先用 0.5 m，Phase 10 的迴圈演練才改成 0.3 m。

```json
{
  "assets": {
    "player": {
      "desc": "單輪小機器人：方頭配深色面罩和兩個發光色眼睛、天線、圓柱身體、兩隻短手臂、輪叉加一顆輪子；輪子是獨立物件 robot_tire（不可用 _wheel 結尾），原點在輪心，wheel_spin 動畫 1 秒轉一圈；平塗顏色，正面朝 Blender -Y",
      "size_m": [0.7, 1.2, 0.5],
      "origin": "bottom_center",
      "max_tris": 3000,
      "collision": "none",
      "anims": ["wheel_spin"]
    },
    "coin": {
      "desc": "金色扁圓柱，直立，圓面朝 Blender ±Y",
      "size_m": [0.5, 0.5, 0.08],
      "origin": "bottom_center",
      "max_tris": 500,
      "collision": "none",
      "anims": []
    },
    "crate": {
      "desc": "中世紀木板箱：橫向木板帶板縫、四面斜撐、深色鐵角片，木色與木桶 / 房子一致，平塗顏色",
      "size_m": [0.8, 0.8, 0.8],
      "origin": "bottom_center",
      "max_tris": 1000,
      "collision": "col",
      "anims": []
    },
    "goal_flag": {
      "desc": "灰色旗桿加紅色三角旗",
      "size_m": [0.8, 2.0, 0.1],
      "origin": "bottom_center",
      "max_tris": 800,
      "collision": "none",
      "anims": []
    },
    "tree": {
      "desc": "低模樹：褐色樹幹加 2–3 團深淺綠低面數樹冠；只有樹幹 mesh 名稱加 -col",
      "size_m": [2.0, 3.5, 2.0],
      "origin": "bottom_center",
      "max_tris": 1000,
      "collision": "col",
      "anims": []
    },
    "bush": {
      "desc": "草叢：3–5 個深淺綠低面數扁球擠在一起",
      "size_m": [1.2, 0.7, 1.0],
      "origin": "bottom_center",
      "max_tris": 600,
      "collision": "none",
      "anims": []
    },
    "rock": {
      "desc": "2–3 塊不規則灰色低模石頭組合（固定亂數種子）",
      "size_m": [1.4, 0.9, 1.2],
      "origin": "bottom_center",
      "max_tris": 400,
      "collision": "col",
      "anims": []
    },
    "wall_brick": {
      "desc": "磚瓦牆一段：灰色灰縫底牆、表面交錯排列的紅褐磚塊、頂部深色瓦片壓頂；長邊沿 Blender X",
      "size_m": [4.0, 1.6, 0.5],
      "origin": "bottom_center",
      "max_tris": 2500,
      "collision": "col",
      "anims": []
    },
    "house": {
      "desc": "小屋：米白灰泥牆加深色木框、紅褐瓦片斜屋頂、木門、兩扇窗、石砌煙囪；門朝 Blender -Y",
      "size_m": [4.0, 4.0, 3.5],
      "origin": "bottom_center",
      "max_tris": 2500,
      "collision": "col",
      "anims": []
    },
    "mage_tower": {
      "desc": "古代奇幻法師塔：石砌圓塔身、石基座、拱窗、外凸平台、紫色錐形尖頂加金色尖端",
      "size_m": [3.2, 8.0, 3.2],
      "origin": "bottom_center",
      "max_tris": 3000,
      "collision": "col",
      "anims": []
    },
    "temple_ruins": {
      "desc": "古代神殿遺跡：三層石階基座、6 根石柱（其中 1–2 根斷裂）、殘存一段門楣、地上碎石；正面朝 Blender -Y",
      "size_m": [6.0, 4.0, 4.0],
      "origin": "bottom_center",
      "max_tris": 3000,
      "collision": "col",
      "anims": []
    },
    "barrel": {
      "desc": "木桶：木色桶身中間略鼓，上下兩圈深色鐵箍",
      "size_m": [0.6, 0.8, 0.6],
      "origin": "bottom_center",
      "max_tris": 400,
      "collision": "none",
      "anims": []
    },
    "terrain": {
      "desc": "箱庭地塊 41×41 m：底部 1.5 m 厚的土層側面（分層色），頂面 0.5 m 格網低模起伏（地面最高 0.9 m，含草叢約 1.15 m），草地深淺色塊、泥土路、石板路與石板廣場、草叢花朵與小石子；建築 / 圍牆 / 金幣 / 終點底下壓平；只有地面網格加 -col",
      "size_m": [41.0, 2.6, 41.0],
      "origin": "bottom_center",
      "max_tris": 40000,
      "collision": "col",
      "anims": []
    },
    "cloud": {
      "desc": "低模雲朵：4–6 個白色扁球堆疊，底部較平、帶淡藍灰陰影色",
      "size_m": [5.0, 1.6, 3.0],
      "origin": "bottom_center",
      "max_tris": 800,
      "collision": "none",
      "anims": []
    }
  }
}
```

`collision`：`"none"` 只有外觀；`"col"` 表示 Blender 裡要有碰撞的 mesh 物件名稱加 `-col`，Godot 匯入時會產生 StaticBody3D。

### 1.3 `world_layout.json`（照抄）

版面的唯一來源。Blender 地形依它決定哪裡壓平、哪裡鋪路；`build_world.mjs` 依它擺放物件。座標是 Godot (x, z)：x 朝東、z 朝南；props 格式 `[kind, x, z, yawDeg, scale]`（後兩個可省略，預設 0 與 1）。

```json
{
  "_note": "Godot coords: x = east, z = south. props: [kind, x, z, yawDeg, scale]. bush at (-1, 8.2) sits on the bot path on purpose.",
  "bot_path": [[0, 0], [6, 0], [6, 8], [-4, 8], [-8, -6]],
  "_flat_pads_note": "[x, z, radius] kept level at height 0: player start, coins, goal and the Crates in main.tscn",
  "flat_pads": [[0, 0, 3.5], [6, 0, 1.5], [6, 8, 1.5], [-4, 8, 1.5], [-8, -6, 2.0], [13.2, -5.6, 1.2], [-9.2, 13.6, 1.2], [4.55, 11.05, 1.5]],
  "plaza": {"center": [0, 0], "radius": 3, "surface": "cobble"},
  "roads": [
    {"from": [0, 0], "to": [-8, -11.8], "width": 1.8, "surface": "cobble"},
    {"from": [0, 0], "to": [8, -9.6], "width": 1.8, "surface": "dirt"},
    {"from": [0, 0], "to": [2, 16], "width": 1.6, "surface": "dirt"}
  ],
  "props": {
    "north": [
      ["mage_tower", -15, -15, 20],
      ["temple_ruins", -8, -14],
      ["house", 3, -15],
      ["house", 9.5, -15.5, -10],
      ["house", 15.5, -9, -90],
      ["wall_brick", 6, -10.5],
      ["wall_brick", 10, -10.5],
      ["crate", 12.6, -12.6, 15],
      ["crate", 11.7, -12, -10, 0.85],
      ["crate", 14.1, -5.9, -5, 0.85],
      ["barrel", 8, -7.5],
      ["barrel", 8.7, -8.1],
      ["barrel", 14, -3],
      ["barrel", 2.5, -7]
    ],
    "west_forest": [
      ["tree", -17.5, -9, 0, 1.1],
      ["tree", -14, -7, 40],
      ["tree", -17, -4.5, 75, 0.9],
      ["tree", -13.5, -2, 120],
      ["tree", -17.5, 0.5, 200, 1.15],
      ["tree", -15, 4, 260, 0.95],
      ["tree", -17.5, 7.5, 300],
      ["tree", -13.5, 9.5, 20, 1.1],
      ["tree", -16.5, 12.5, 90],
      ["tree", -13, 15.5, 150, 0.9],
      ["tree", -17.5, 17, 210, 1.05]
    ],
    "edge_trees": [
      ["tree", -11.5, -18, 30, 0.95],
      ["tree", -3, -17.5, 100],
      ["tree", 0, -18, 160, 1.1],
      ["tree", 13.5, -18, 220],
      ["tree", 17.5, -17, 280, 0.9],
      ["tree", 17.5, 3.5, 10],
      ["tree", 17, 9, 130, 1.1],
      ["tree", 18, 14.5, 250],
      ["tree", -6, 17.5, 60, 0.95],
      ["tree", 6, 18, 190],
      ["tree", 12, 17, 310, 1.05]
    ],
    "rocks": [
      ["rock", -11, 6.5, 30],
      ["rock", -10, -9.5, 110, 0.8],
      ["rock", 14, 7, 200],
      ["rock", 16.5, 13, 45, 1.2],
      ["rock", -4, 15, 300, 0.9],
      ["rock", 3.5, 16.5, 150],
      ["rock", 10.5, 12.5, 80, 0.7],
      ["rock", -16, -17.5, 10, 1.1]
    ],
    "garden": [
      ["wall_brick", -8, 10.5],
      ["wall_brick", -10.25, 12.5, 90],
      ["bush", -8, 12.3, 30, 0.9],
      ["bush", -6.8, 13.6, 160, 0.8]
    ],
    "south_yard": [
      ["crate", 4.5, 11, 20],
      ["crate", 5.4, 11.7, -15, 0.85],
      ["barrel", 3.6, 11.8],
      ["rock", 9, 3, 140, 0.8],
      ["tree", -9, 4, 70, 0.9],
      ["tree", 10, -3, 330],
      ["tree", 9.5, 6, 20, 0.85]
    ],
    "bushes": [
      ["bush", -1, 8.2, 0],
      ["bush", -10.5, -1, 60],
      ["bush", -11, 12.5, 120, 1.2],
      ["bush", 3, -4, 200],
      ["bush", 12, 10.5, 30, 0.9],
      ["bush", -2.5, 13, 300],
      ["bush", 8.5, 14.5, 90, 1.1],
      ["bush", 16.5, -13.5, 150],
      ["bush", -6, -9.5, 240, 0.9],
      ["bush", 5, -12, 10],
      ["bush", 18, -4, 330],
      ["bush", -18, -13, 75, 1.1]
    ]
  }
}
```

- `bot_path`：起點 → Coin1 → Coin2 → Coin3 → Goal，bot 走直線
- `flat_pads`：地形在這些圓內壓平到高度 0（起點、3 枚金幣、終點、main.tscn 的 3 個木箱），所以 main.tscn 的遊戲節點不用算高度
- 鏡頭在東南往西北看：高的建築放北、西兩側，南、東只放低矮物件

### 1.4 `.mcp.json` 與 `.claude/settings.json`

```json
{
  "mcpServers": {
    "godot": {
      "command": "npx",
      "args": ["-y", "@coding-solo/godot-mcp"],
      "env": { "GODOT_PATH": "<godotPath>" }
    },
    "blender": {
      "command": "uvx",
      "args": ["blender-mcp"]
    }
  }
}
```

Windows 原生（非 WSL）：`command` 改成 `cmd`，`args` 最前面加 `"/c"`。不想送 telemetry 可在 blender 的 `env` 加 `"DISABLE_TELEMETRY": "true"`。

```json
{
  "permissions": {
    "allow": [
      "mcp__godot",
      "Bash(node tools/check_env.mjs:*)",
      "Bash(node tools/import.mjs:*)",
      "Bash(node tools/check_assets.mjs:*)",
      "Bash(node tools/bot_test.mjs:*)",
      "Bash(node tools/physics_test.mjs:*)",
      "Bash(node tools/build_world.mjs:*)"
    ]
  }
}
```

Blender MCP 刻意不放進自動允許清單：`execute_blender_code` 會執行任意 Python。agent 寫 `.claude/settings.json` 被權限擋下時，請使用者手動建立。

### 1.5 `.gitignore`

```text
game/.godot/
blender/*.blend1
pipeline/screenshot.png
pipeline/screenshot_overview.png
```

### 1.6 `game/project.godot`（照抄，替換版本號）

`[input]` 段落定義 boost / brake，並覆寫 `ui_accept` 補上手把 A（Godot 4.7 預設的 `ui_accept` 只有鍵盤）。

```ini
config_version=5

[application]
config/name="MCP Game PoC"
run/main_scene="res://scenes/main.tscn"
config/features=PackedStringArray("<主.次版本，例如 4.7>")

[input]

ui_accept={
"deadzone": 0.2,
"events": [Object(InputEventKey,"resource_local_to_scene":false,"resource_name":"","device":-1,"window_id":0,"alt_pressed":false,"shift_pressed":false,"ctrl_pressed":false,"meta_pressed":false,"pressed":false,"keycode":4194309,"physical_keycode":0,"key_label":0,"unicode":0,"location":0,"echo":false,"script":null)
, Object(InputEventKey,"resource_local_to_scene":false,"resource_name":"","device":-1,"window_id":0,"alt_pressed":false,"shift_pressed":false,"ctrl_pressed":false,"meta_pressed":false,"pressed":false,"keycode":4194310,"physical_keycode":0,"key_label":0,"unicode":0,"location":0,"echo":false,"script":null)
, Object(InputEventKey,"resource_local_to_scene":false,"resource_name":"","device":-1,"window_id":0,"alt_pressed":false,"shift_pressed":false,"ctrl_pressed":false,"meta_pressed":false,"pressed":false,"keycode":32,"physical_keycode":0,"key_label":0,"unicode":32,"location":0,"echo":false,"script":null)
, Object(InputEventJoypadButton,"resource_local_to_scene":false,"resource_name":"","device":-1,"button_index":0,"pressure":0.0,"pressed":true,"script":null)
]
}
boost={
"deadzone": 0.2,
"events": [Object(InputEventKey,"resource_local_to_scene":false,"resource_name":"","device":-1,"window_id":0,"alt_pressed":false,"shift_pressed":false,"ctrl_pressed":false,"meta_pressed":false,"pressed":false,"keycode":0,"physical_keycode":65,"key_label":0,"unicode":97,"location":0,"echo":false,"script":null)
, Object(InputEventJoypadButton,"resource_local_to_scene":false,"resource_name":"","device":-1,"button_index":2,"pressure":0.0,"pressed":true,"script":null)
]
}
brake={
"deadzone": 0.2,
"events": [Object(InputEventKey,"resource_local_to_scene":false,"resource_name":"","device":-1,"window_id":0,"alt_pressed":false,"shift_pressed":false,"ctrl_pressed":false,"meta_pressed":false,"pressed":false,"keycode":0,"physical_keycode":66,"key_label":0,"unicode":98,"location":0,"echo":false,"script":null)
, Object(InputEventJoypadButton,"resource_local_to_scene":false,"resource_name":"","device":-1,"button_index":1,"pressure":0.0,"pressed":true,"script":null)
]
}
```

### 1.7 `game/tools/inspect_assets.gd`（照抄）

```gdscript
extends SceneTree
# 用法：godot --headless --path game -s res://tools/inspect_assets.gd -- res://assets/a.glb ...

func _init() -> void:
	var result := {}
	for path in OS.get_cmdline_user_args():
		result[path] = _inspect(path)
	print("INSPECT_JSON:" + JSON.stringify(result))
	quit()

func _inspect(path: String) -> Dictionary:
	if not ResourceLoader.exists(path):
		return {"error": "not_found_or_not_imported"}
	var packed := load(path) as PackedScene
	if packed == null:
		return {"error": "load_failed"}
	var root := packed.instantiate()
	var state := {"aabb": AABB(), "first": true, "collision": false, "anims": []}
	_walk(root, Transform3D.IDENTITY, state)
	root.free()
	if state["first"]:
		return {"error": "no_mesh"}
	var box: AABB = state["aabb"]
	return {
		"aabb_min": [box.position.x, box.position.y, box.position.z],
		"aabb_size": [box.size.x, box.size.y, box.size.z],
		"has_collision": state["collision"],
		"anims": state["anims"],
	}

func _walk(node: Node, parent_xf: Transform3D, state: Dictionary) -> void:
	var xf := parent_xf
	if node is Node3D:
		xf = parent_xf * (node as Node3D).transform
	if node is MeshInstance3D and (node as MeshInstance3D).mesh != null:
		var box: AABB = xf * (node as MeshInstance3D).mesh.get_aabb()
		if state["first"]:
			state["aabb"] = box
			state["first"] = false
		else:
			var cur: AABB = state["aabb"]
			state["aabb"] = cur.merge(box)
	if node is CollisionObject3D or node is CollisionShape3D:
		state["collision"] = true
	if node is AnimationPlayer:
		for anim_name in (node as AnimationPlayer).get_animation_list():
			state["anims"].append(String(anim_name))
	for child in node.get_children():
		_walk(child, xf, state)
```

### 1.8 Node 工具（由你實作，規格如下）

所有工具都讀 `pipeline.config.json`，cwd 為專案根目錄，用 `child_process.spawnSync` 呼叫 Godot。

**`tools/check_env.mjs`**
- 確認 `godotPath` 可執行並印出版本；確認 `localhost:<blenderPort>` 的 TCP 連線可建立，印出 OK / NG；全部 OK → exit 0，否則 exit 1
- 注意：只測 TCP，9876 被別的程式佔用也會 OK（附錄 C-1），真正的驗證是 Phase 2.3

**`tools/import.mjs`**
- 執行 `<godotPath> --headless --path game --import`，timeout 300 秒；印出含 `ERROR` / `WARNING` 的行；結束碼非 0 → exit 1
- main.tscn 還沒建立前會印 `Cannot open file 'res://scenes/main.tscn'`，屬預期

**`tools/check_assets.mjs [id ...]`**（不帶參數 = 全部資產）
- 執行 `<godotPath> --headless --path game -s res://tools/inspect_assets.gd -- res://assets/<id>.glb ...`，解析 `INSPECT_JSON:` 那一行；讀 `pipeline/blender_stats/<id>.json`
- 每個資產逐項印 PASS / FAIL：

| 項目 | 規則 |
|---|---|
| 載入 | inspect 結果沒有 `error` |
| Blender 尺寸 | `stats.size_m` 與 spec 誤差 ≤ max(relative × spec, absoluteMin) |
| Godot 尺寸 | `inspect.aabb_size` 套用同樣規則 |
| 原點 | `bottom_center`：Godot 的 `aabb_min.y`、X / Z 中心絕對值都 ≤ 0.02 |
| 面數 | `stats.tris` ≤ `max_tris` |
| 碰撞 | `collision` 為 `"col"` 時 `has_collision` 必須是 true |
| 動畫 | `anims` 每個名稱都要出現在 `inspect.anims` |

- FAIL 時印一行具體修正建議，例如 `FIX coin: X 寬 0.50m，規格 0.30m → 在 Blender 沿 X 軸縮放 0.600 後 apply scale`
- Blender 尺寸 PASS 但 Godot 尺寸 FAIL → 額外印 `PIPELINE: export/軸向問題，不是建模問題`
- 最後一行 `SUMMARY: <n> pass, <m> fail`；有 FAIL → exit 1

**`tools/bot_test.mjs`**
- 執行 `<godotPath> --headless --path game --fixed-fps 60 --quit-after 1800 -- --bot`
- PASS：輸出含 `COIN 3/3` 和 `WIN`，且沒有 `SCRIPT ERROR` 或 `ERROR:`；印結果與相關 log 行；FAIL → exit 1

**`tools/physics_test.mjs`**
- 執行 `<godotPath> --headless --path game --fixed-fps 60 -s res://tools/physics_test.gd`，timeout 300 秒
- PASS：輸出含 `PHYSICS_TEST: PASS`、結束碼 0、沒有 `SCRIPT ERROR` / `ERROR:`；印出 `PASS` / `FAIL` / `TREE_HIT` 行；最後一行 `PHYSICS: PASS|FAIL (...)`

**`tools/build_world.mjs`**（Phase 7 使用）
- 讀 `world_layout.json` 與 `pipeline/terrain_heights.json`，寫出 `game/scenes/world.tscn`
- 種類表：

| kind | 群組節點 | 場景 | 路線檢查半徑 |
|---|---|---|---|
| wall_brick | Walls | `res://assets/wall_brick.glb` | 2.0 |
| tree | Trees | `res://scenes/tree.tscn` | 0.3 |
| bush | Bushes | `res://scenes/bush.tscn` | —（無碰撞） |
| rock | Rocks | `res://assets/rock.glb` | 0.75 |
| barrel | Barrels | `res://scenes/barrel.tscn` | 1.5 |
| crate | Props | `res://assets/crate.glb` | 0.75 |
| house / mage_tower / temple_ruins | Buildings | `res://assets/<kind>.glb` | 2.7 / 1.6 / 3.6 |

- 外牆自動產生：沿 ±20 每 4 m 一段（中心 −18, −14, …, 18），南北兩邊 yaw 0，東西兩邊 yaw 90，共 40 段
- 路線檢查：有半徑的物件到 `bot_path` 各線段的最短距離 − 半徑 < 1.0 → 印出清單並 exit 1（外牆不檢查）
- 平整檢查：`flat_pads` 每個中心的地形高度絕對值 > 0.03 → 印出並 exit 1（要求重匯地形）
- 高度：`y = height(x, z) − SINK`，SINK：tree 0.05、rock 0.1、bush 0.03，其他 0；`height` 用高度表雙線性內插
- 輸出：根節點 `World`（Node3D）；群組依第一次出現的順序；節點名稱 = kind 轉 PascalCase + 該 kind 的兩位數序號（例如 `Tree04`、`WallBrick03`）；transform 只含 yaw 與等比 scale；ext_resource 只寫 path、不寫 uid
- 最後印 `BUILD_WORLD: <總數> props (<擺放數> placed + 40 walls) on terrain → game/scenes/world.tscn`

### 1.9 `CLAUDE.md`（照抄）

```markdown
# MCP Game PoC（箱庭版）

流程與步驟以 .spec/PLAYBOOK_v2.md 為準；進度記在 pipeline/progress.md。

## 座標與單位
- 單位是公尺。assets.json 的 size_m 一律用 Godot 軸向 [X, Y(上), Z]
- Blender 是 Z 朝上：Godot (X, Y, Z) = Blender (X, Z, -Y)
- 角色正面朝 Blender -Y，匯出後在 Godot 是朝 +Z
- world_layout.json 的座標是 Godot (x, z)：x 朝東、z 朝南

## Blender 規則
- 每個資產一個 collection，名稱 = asset id，在世界原點建模
- 匯出前該 collection 必須在 view layer 中可見（不可 exclude）
- 原點 bottom_center：底部 z=0、XY 置中
- 低模、平塗：只用 Principled BSDF 的 Base Color（linear 值），不用圖片貼圖
- collision 為 "col" 的資產，有碰撞的 mesh 物件名稱加 -col 後綴；其他物件名稱結尾不可撞到 Godot 匯入提示字（-noimp、-rigid、-vehicle、-wheel、-navmesh、-convcol、-colonly 等，底線也算）
- execute_blender_code 每次只做一小步；不刪專案外的檔案；不下載外部資產
- 模型匯出只用 PLAYBOOK 附錄 A0（匯出前準備）+ 附錄 A（標準 snippet）

## 每次匯出後必做
1. node tools/import.mjs
2. node tools/check_assets.mjs <id>

## 迴圈規則
- FAIL → 依 PLAYBOOK 附錄 B 的格式寫進 pipeline/log.md → 回 Blender 修
- 同一資產每輪最多 maxIterations 次，超過就停下來問使用者
- 不要手動修改 .glb 或 .import 檔

## Godot 規則
- 腳本放 game/scripts/，場景放 game/scenes/
- 輸入只用內建的 ui_left / ui_right / ui_up / ui_down / ui_accept（另加手把 A 跳躍），以及自訂的 boost（鍵盤 A、手把 X）/ brake（鍵盤 B、手把 B）；input map 定義在 game/project.godot
- 優先使用 Godot MCP 工具；做不到再直接編輯 .tscn，並記到 log 的「MCP 問題」
- game/scenes/world.tscn 只由 node tools/build_world.mjs 產生，不要手改；要改版面就改 world_layout.json
```

### 1.10 初始化

1. 建立 `pipeline/progress.md`（各 Phase 的步驟清單）和 `pipeline/log.md`（含空的 `## MCP 問題` 段落）
2. `git init`
3. `node tools/import.mjs`（第一次執行會建立 `game/.godot`）
4. `git commit`

---

## Phase 2：連線 MCP（**STOP**，需要人工）

告訴使用者依序完成：
1. 依 blender-mcp README 安裝 Blender addon（目前的 `uvx blender-mcp` 是 mcp-for-blender；`uvx mcp-for-blender install-addon` 可安裝 / 更新 addon），在 Preferences → Add-ons 勾選「Interface: Blender MCP」
2. **Blender 5.x 若有內建的 Blender Lab「MCP」擴充，先停用它**：它也聽 9876 埠，協定不同，會讓 blender-mcp 握手逾時（附錄 C-1）
3. 3D 視窗按 `N` → BlenderMCP 分頁 → 不勾外部資產庫 / 生成器 → Start MCP Server
4. 退出 agent，在專案資料夾重新啟動；出現 `.mcp.json` 伺服器核准提示時選允許
5. 輸入：`讀 .spec/PLAYBOOK_v2.md 和 pipeline/progress.md，繼續`

重啟後由 agent 驗證：
1. `node tools/check_env.mjs` 全部 OK
2. `mcp__godot__get_godot_version` 成功
3. `mcp__blender__get_addon_status` 與 `mcp__blender__get_scene_info` 成功（第一次失敗很常見，重試一次）
4. 任一項失敗 → **STOP**，回報是哪一項失敗

---

## Phase 3：單一資產冒煙測試（crate）

1. Blender：刪掉預設 Cube，建 collection `crate`，依附錄 D 建模（多次小步驟 `execute_blender_code`）
2. `mcp__blender__look`（angles 模式、material shading）自檢外觀，與 desc 不符就修（本 MCP 沒有 `get_viewport_screenshot`）
3. 附錄 A0 → 附錄 A，`ASSET = "crate"`
4. `node tools/import.mjs`
5. `node tools/check_assets.mjs crate` → 必須 PASS（FAIL 走迴圈規則）
6. Godot MCP `create_scene` 建 `res://scenes/test_crate.tscn`（root Node3D），把 crate.glb instance 進去（`add_node` 不能 instance .glb → 直接編輯 .tscn 加 ext_resource，記到 MCP 問題）
7. `run_project`（scene 參數指定 test_crate）→ `get_debug_output` 無錯誤 → `stop_project`
8. `git commit`

---

## Phase 4：角色與道具資產

依序做 player、coin、goal_flag、barrel、tree、bush、rock、cloud，每個都走 Phase 3 的步驟 1–5（各自的 collection；建模時可把其他 collection 在 viewport 隱藏，但匯出前一定要跑附錄 A0）。建模細節見附錄 D。

1. 8 個資產各自 check PASS
2. player 額外確認（附錄 C-2、C-3）：用一支暫存的 headless 腳本列出 player.glb 的節點，`robot_tire` 必須是 MeshInstance3D、`AnimationPlayer` 有 `wheel_spin` 且軌道路徑是 `robot_tire`
3. `git commit`

## Phase 5：建築資產

1. 依序做 wall_brick、house、mage_tower、temple_ruins（同 Phase 3 步驟 1–5，細節見附錄 D）
2. `git commit`

## Phase 6：地形（terrain）

地形依 `world_layout.json` 與 `assets.json` 生成，演算法見附錄 E。

1. Blender 計算高度場（印出 min / max 與 8 個 flat_pads 的高度，必須都是 0）
2. 建地面網格 `terrain_ground-col`（含土層側面與底面），依附錄 E 上色
3. 鋪石板 `terrain_cobble`、撒草花 `terrain_flora`、路邊小石子 `terrain_pebbles`
4. `look` 近看（distance 約 16 m）：草地色塊要柔和、不能像碎紙片；石板廣場、石板路、泥土路清楚
5. 寫出 `pipeline/terrain_heights.json`（附錄 E 格式；這是資料檔，不算模型匯出）
6. 附錄 A0 → 附錄 A（`ASSET = "terrain"`）→ import → check PASS
7. `git commit`

---

## Phase 7：Godot 互動道具場景與世界

1. Godot MCP 建 `tree.tscn`、`bush.tscn`、`barrel.tscn` 的節點骨架（`create_scene` + `add_node`，scalar 屬性可用 `properties` 設定），mesh / curve / gradient / shape / script / glb instance 直接編輯 .tscn。規格見附錄 F.1
2. 撰寫 `wind.gd`、`sway.gd`、`tree.gd`、`bush.gd`（附錄 F.2）
3. 實作 `tools/build_world.mjs`（1.8），執行 `node tools/build_world.mjs`：必須通過路線與平整檢查，產生 107 個物件（67 擺放 + 40 外牆）
4. `node tools/import.mjs` 無錯誤
5. `git commit`

## Phase 8：Godot 主場景與腳本

1. Godot MCP `create_scene` 建 `main.tscn`、`add_node` 加入內建節點、`save_scene`；transform / Resource / script / glb instance 直接編輯 .tscn（記 MCP 問題）。結構與屬性見附錄 F.3
2. 撰寫 `main.gd`、`player.gd`、`coin.gd`、`goal.gd`、`clouds.gd`（附錄 F.2）
3. `node tools/import.mjs`（若有 UID 警告，用 Godot MCP `update_project_uids`）
4. `git commit`

---

## Phase 9：測試與畫面檢查

1. 實作 `game/tools/physics_test.gd`（規格如下）與 `tools/physics_test.mjs`（1.8）
2. `node tools/bot_test.mjs` → PASS
3. `node tools/physics_test.mjs` → PASS
4. Godot MCP `run_project` → 等 5–6 秒 → `get_debug_output` 無錯誤 → `stop_project`；讀 `pipeline/screenshot.png`
5. 全景：`<godotPath> --path game --quit-after 300 -- --overview`，讀 `pipeline/screenshot_overview.png`
6. 依下方清單檢查兩張截圖；不合理時判斷是資產問題（回 Blender）還是場景問題（修 Godot）
7. `git commit`

**`physics_test.gd`**（`extends SceneTree`，用法寫在檔頭註解）
- 函式測試（直接呼叫 `player.gd` 的 static `step_velocity`，dt = 1/60）：

| # | 檢查 | 通過條件 |
|---|---|---|
| 1 | 從靜止按住方向 1.5 s | 速度在 (3.8, 4.05) |
| 2 | 放開後 1 s | 速度為 0 |
| 3 | 起步 0.1 s，空中 vs 地面 | 空中 < 地面 × 0.5 |
| 4 | 空中以 4 m/s 按住方向 1 s | ≤ 4.01（跳躍不會變快） |
| 5 | 加速極速，從靜止 2 s | 在 (6.7, 7.05) |
| 6 | 從 7 m/s 煞車（同時按住方向）vs 放開滑行 | 煞車 ≤ 30 格，且 < 滑行格數 × 0.7 |

- 場景測試（載入 main.tscn，非 bot 模式，用 `Input.action_press("ui_up")` 驅動）：
  - 把機器人放在目標「沿鏡頭前方往後 1.6 m」處，y = 目標底部 + 0.5，按住 `ui_up` 最多 120 格
  - 7：離 (−13.5, 0, −2) 最近的樹 → 葉子粒子 `emitting` 變 true
  - 8：同一棵樹的搖晃角度 > 0.005 rad
  - 9：離 (14, 0, −3) 最近的木桶，推 120 格後水平位移 > 0.3 m
- 每項印 `PASS <說明>` 或 `FAIL <說明>`，最後印 `PHYSICS_TEST: PASS|FAIL` 並以 0 / 1 結束

**截圖檢查清單**
- 跟隨（screenshot.png）：機器人在畫面中央、正面或背面可辨識；石板廣場、道路、草叢小花清楚；畫面上下邊緣有移軸模糊；樹影柔和
- 全景（screenshot_overview.png）：整個地塊含最靠近鏡頭的角都在畫面內；背景淡藍；法師塔、神殿、三間房子、圍籬、菜園都看得到；雲飄在地塊上空且地上有雲影；遠半邊的樹也有影子
- 兩張都沒有倒立 / 躺平 / 懸空 / 陷入地面的物件

---

## Phase 10：迴圈演練

模擬「Godot 端發現資產需要修改」：
1. 在 log 寫下：`Godot 回饋：coin 太大、擋視線 → 規格改為 [0.3, 0.3, 0.06]`
2. 修改 `assets.json` 的 `coin.size_m`
3. `node tools/check_assets.mjs coin` → 應該 FAIL（確認判斷機制有效），依附錄 B 記錄
4. 依 FIX 建議在 Blender 以原點為基準縮放 → 附錄 A0 + A → import → check → PASS
5. main.tscn 三個 Coin 的 SphereShape3D 半徑 0.5 → 0.3；`bot_test` 與 `physics_test` 仍 PASS
6. 重跑 `run_project` 看截圖，`git commit`

---

## Phase 11：收尾

1. 撰寫 `pipeline/REPORT.md`：DoD 勾選結果、每個資產的迭代次數、MCP 問題與替代做法、physics_test 的實測數字、遇到的坑（對照附錄 C，標出新出現的）、卡最久的步驟與建議的下一步
2. `git commit`
3. 告訴使用者 PoC 已完成，附 REPORT 摘要

---

## 附錄 A0：匯出前準備（每次跑附錄 A 之前執行）

建模時隱藏的 collection 裡，物件可能還保留上一次匯出的選取狀態；`select_all(DESELECT)` 碰不到隱藏物件，`use_selection=True` 就會把它們一起匯出（附錄 C-4）。

```python
import bpy
for lc in bpy.context.view_layer.layer_collection.children:
    lc.hide_viewport = False
for o in bpy.context.view_layer.objects:
    o.select_set(False)
bpy.context.scene.frame_set(1)
print([o.name for o in bpy.context.selected_objects])   # 必須是 []
```

## 附錄 A：標準 export snippet（透過 `execute_blender_code` 執行）

```python
import bpy, json, os

ASSET = "crate"            # asset id
ROOT = r"<projectRoot>"    # pipeline.config.json 的 projectRoot

if bpy.context.mode != 'OBJECT':
    bpy.ops.object.mode_set(mode='OBJECT')

col = bpy.data.collections[ASSET]
objs = [o for o in col.all_objects if o.type in {'MESH', 'EMPTY'}]
bpy.ops.object.select_all(action='DESELECT')
for o in objs:
    o.hide_set(False)
    o.select_set(True)
bpy.context.view_layer.objects.active = objs[0]
bpy.ops.object.transform_apply(location=False, rotation=True, scale=True)

# 量測：Blender (X, Y, Z-up) → Godot (X, Y-up, Z) = (X, Z, -Y)
deps = bpy.context.evaluated_depsgraph_get()
tris, mn, mx = 0, [1e9] * 3, [-1e9] * 3
for o in objs:
    if o.type != 'MESH':
        continue
    ev = o.evaluated_get(deps)
    me = ev.to_mesh()
    me.calc_loop_triangles()
    tris += len(me.loop_triangles)
    for v in me.vertices:
        w = ev.matrix_world @ v.co
        for i in range(3):
            mn[i] = min(mn[i], w[i])
            mx[i] = max(mx[i], w[i])
    ev.to_mesh_clear()

stats = {
    "tris": tris,
    "size_m": [mx[0] - mn[0], mx[2] - mn[2], mx[1] - mn[1]],
    "origin_offset": [(mx[0] + mn[0]) / 2, mn[2], -(mx[1] + mn[1]) / 2],
}
os.makedirs(os.path.join(ROOT, "pipeline", "blender_stats"), exist_ok=True)
with open(os.path.join(ROOT, "pipeline", "blender_stats", f"{ASSET}.json"), "w") as f:
    json.dump(stats, f, indent=2)

bpy.ops.export_scene.gltf(
    filepath=os.path.join(ROOT, "game", "assets", f"{ASSET}.glb"),
    export_format='GLB',
    use_selection=True,
    export_apply=True,
)
bpy.ops.wm.save_as_mainfile(filepath=os.path.join(ROOT, "blender", "assets.blend"))
print(json.dumps(stats))
```

## 附錄 B：調整請求格式（寫入 `pipeline/log.md`）

```text
## <時間> <asset> iter <n>
- check: FAIL — <check_assets 印出的 FIX 行>
- 判斷: 建模問題 / pipeline 問題
- 對 Blender 的請求: <具體操作，例如「縮放 0.5 後 apply scale」>
- 結果: PASS / FAIL
```

另開 `## MCP 問題` 段落，記錄工具失敗的情況與當時的替代做法。PASS 的資產也用同格式記一筆（iter 數供 REPORT 統計）。

---

## 附錄 C：已知的坑

| # | 現象 | 原因 | 做法 |
|---|---|---|---|
| C-1 | blender MCP 啟動逾時、沒有 `mcp__blender__*` 工具；`check_env` 卻顯示 9876 OK | Blender 5.x 內建 Blender Lab「MCP」擴充佔用 9876，協定不同 | 停用該擴充、改用 blender-mcp addon；`check_env` 只測 TCP，以 `get_scene_info` 成功為準 |
| C-2 | 輪子匯入後變成 VehicleWheel3D、名稱被截斷，check 卻 PASS | Godot 把名稱結尾 `-wheel` / `_wheel` 當匯入提示 | 物件名稱避開提示字；輪子叫 `robot_tire` |
| C-3 | `wheel_spin` 長度 1.04 s，循環時每圈頓一下 | 匯出的取樣範圍多一格 | Godot 端執行時 `anim.length = 最後一個 key 的時間`，`loop_mode = LOOP_LINEAR` |
| C-4 | player.glb 混進別的資產（Godot 尺寸變大、多出碰撞） | 隱藏 collection 的物件保留選取 | 附錄 A0 |
| C-5 | `add_node` 不能 instance .glb，`properties` 設不了 Vector3 / Transform3D / Resource | godot-mcp 只走 `ClassDB.instantiate()` + `Node.set()` | 骨架用 MCP，其餘直接編輯 .tscn |
| C-6 | `add_node` 改到既有場景時，整份 .tscn 被重新序列化（uid 消失、instance 補 type / unique_id） | godot-mcp 用 ResourceSaver 重存 | 接受 Godot 的正規化格式 |
| C-7 | MCP 每次結束都印 `RID allocations ... leaked at exit` | godot-mcp 的 headless 腳本沒 free 場景 | 不影響結果，忽略 |
| C-8 | Blender 調的顏色在 Godot 看起來偏亮 | Blender Base Color 是 linear；Godot StandardMaterial3D 的 albedo 是 sRGB | Blender 端用 linear 值（例如 sRGB 0.35 ≈ linear 0.10） |
| C-9 | bmesh `bevel` 後 ReferenceError | bevel 會刪掉原本的頂點 | 先設 material_index 再 bevel |
| C-10 | `create_icosphere(subdivisions=1)` 只有 20 面 | bmesh 的 subdivisions 從 1 起算 | 需要圓一點用 2（80 面） |
| C-11 | .tscn 加 `#` 註解後載入失敗或被丟掉 | .tscn 不支援 `#` 註解 | 不要在 .tscn 寫註解 |
| C-12 | 子類別覆寫 `_ready()` 後，父類別的初始化沒跑（例如草叢風向沒設定） | GDScript 覆寫不會自動呼叫父類別 | 子類別 `_ready()` 第一行 `super()` |
| C-13 | 按住方向跳躍會越跳越快 | 空中阻力太小 | 空中阻力 = 空中推力 / (m × 極速)，空中極速 = 地面極速 |
| C-14 | 機器人幾乎推不動木桶、木桶被推倒 | CharacterBody 撞上就把自己速度歸零，只有接觸那一格有推力；衝量打在接觸點 | 撞擊瞬間給折合質量 × 接近速度的衝量 + 接觸時給驅動力，作用在質心 |
| C-15 | 看不到雲影 | 影子會順著陽光方向偏移（雲高 15 m、仰角 40° 約偏 18 m），雲以地圖中心分布時影子多落在地圖外 | 雲的分布範圍往陽光反方向挪 |
| C-16 | 全景截圖遠半邊沒有陰影 | 鏡頭距離 64 m 超過 `directional_shadow_max_distance` 60 m | `--overview` 時把陰影距離設為鏡頭距離 × 2 |
| C-17 | 手把 A 不能跳 | Godot 4.7 預設 `ui_accept` 只有鍵盤 | project.godot 覆寫 `ui_accept` 補上 Joypad Button 0 |
| C-18 | 地形 check 尺寸 FAIL | 規格高度沒算進山頂草叢 | 規格 Y 包含附屬物（地形 2.6 m） |
| C-19 | 葉子粒子在鏡頭距離下看不清楚 | 0.14 m 太小 | 葉片 0.22 × 0.16 m、32 片 |

---

## 附錄 D：資產建模要點

共通做法：
- 建議在 Blender session 註冊一組 bmesh 小工具（`bpy.app.driver_namespace["H"]`，不會存進 .blend）：建材質、box（可 bevel、可繞 Z 旋轉）、cylinder、icosphere（可抖動頂點）、`start_asset`（建 / 清空 collection、只顯示它）、`make_obj`、`fit`（各軸分別縮放，讓整個 collection 對齊 size_m，並移到 bottom_center）、`summary`
- 顏色都是 linear Base Color；粗糙度 0.8–0.95
- 有 `-col` 的物件放「簡單外形」，細節放另一個不加 `-col` 的物件，碰撞體保持簡單

| 資產 | 結構 | 主要顏色（linear） |
|---|---|---|
| crate | 牆芯 `crate_core-col` 0.70×0.70×0.78；`crate_detail`：每面 4 條橫板 0.66×0.03×0.17（留板縫）、45° 斜撐、上下橫檔、4 根角柱 0.08、4 片頂板、8 個鐵角片；fit 到 0.8 | 板縫 (0.09,0.045,0.018)、木板 (0.45,0.24,0.09) / (0.36,0.19,0.07)、框 (0.24,0.12,0.045)、鐵 (0.06,0.06,0.07) |
| coin | 圓柱 r 0.25、厚 0.08、32 段，兩個圓面 inset 0.035 / 內凹 0.012，繞 X 轉 90° 讓圓面朝 ±Y，底部 z=0 | 金 (1.0,0.62,0.05) |
| goal_flag | 旗桿 r 0.05 放 x=−0.35（讓 bbox 置中），高 1.95 + 頂端小球；三角旗 x −0.30→0.40、z 1.40–1.90、厚 0.02 | 桿 (0.30,0.30,0.30)、旗 (0.80,0.02,0.02) |
| player | `robot_tire`：輪 r 0.16、寬 0.10 + 輪轂 + 兩側橘色十字（看得出轉動），**物件原點在輪心 (0,0,0.16)**；`robot_body`：輪叉、圓身 r 0.25 z 0.44–0.80、橘色腰帶、胸燈、脖子；`robot_head`：0.44×0.38×0.26 倒角方頭、深色面罩、青色雙眼、耳蓋、天線頂到 z 1.20；`robot_arms`：肩球、手臂、手掌伸到 x ±0.35 | 白 (0.80,0.82,0.85)、橘 (0.90,0.30,0.03)、深 (0.03,0.035,0.045)、眼 (0.05,0.85,1.0)、輪胎 (0.015,…)、金屬 (0.35,0.37,0.40) |
| player 動畫 | `robot_tire` 的 `rotation_euler.x`：第 1 格 0、第 25 格 2π（24 fps = 1 秒一圈），插值 LINEAR，action 改名 `wheel_spin`；+X 旋轉 = 往 −Y（正面）滾 | — |
| barrel | 16 段手建環：(z, r) = (0, 0.26)、(0.1, 0.285)、(0.4, 0.30)、(0.7, 0.285)、(0.8, 0.26)，上下蓋；z 0.16、0.64 兩圈鐵箍 r 0.30 | 木 (0.45,0.24,0.09)、鐵 (0.06,0.06,0.07)、蓋 (0.30,0.15,0.05) |
| tree | `tree_trunk-col`：錐形樹幹 r 0.18→0.10、高 1.9、8 段；`tree_canopy`：4 顆抖動 icosphere（subdiv 1，深淺兩綠）；fit 到 2.0×3.5×2.0 | 樹皮 (0.22,0.11,0.04)、葉 (0.04,0.20,0.035) / (0.10,0.36,0.05) |
| bush | 5 顆壓扁抖動 icosphere（subdiv 2）；fit | (0.03,0.17,0.03) / (0.09,0.32,0.05) |
| rock | `rock-col`：1 大（subdiv 2，頂面朝上的面上青苔色）+ 2 小抖動 icosphere；fit | 灰 (0.30,0.30,0.28)、深 (0.16,…)、苔 (0.10,0.22,0.05) |
| cloud | 6 顆 icosphere（subdiv 2）堆疊，z < 0.25 的頂點壓平，朝下的面用陰影色；fit 到 5.0×1.6×3.0 | 白 (0.95,0.96,1.0)、影 (0.72,0.78,0.88) |
| wall_brick | `wall_core-col`：灰縫牆芯 4.0×0.44×1.40 + 壓頂板 + 16 段交替高度的瓦片三角稜（頂 1.60 / 1.57）；`wall_bricks`：7 排交錯磚（排高 0.20、磚長 0.50、縫 0.025、穿透兩面 0.48 厚），三種磚色隨機 | 灰縫 (0.33,0.31,0.28)、磚 (0.42,0.11,0.05) / (0.30,0.08,0.04) / (0.52,0.18,0.08)、瓦 (0.10,0.05,0.04) |
| house | `house-col`：石基座、灰泥牆 3.4×2.9（z 0.25–2.3）、兩端山牆三角、6 段交替色瓦片坡頂（屋簷 z 2.2、屋脊 z 3.6、外伸到 ±2.0 / ±1.75）、屋脊圓木、石砌煙囪頂到 z 4.0；`house_detail`：角柱、上 / 中橫樑、木門（−Y 面）、4 扇十字窗框；fit | 灰泥 (0.80,0.74,0.60)、石 (0.32,0.31,0.29)、瓦 (0.40,0.09,0.04) / (0.28,0.06,0.03)、木 (0.12,0.06,0.025)、玻璃 (0.25,0.45,0.60) |
| mage_tower | `mage_tower-col`：八角基座 r 1.6、塔身 r 1.15→1.05（z 0.6–5.6）、平台 r 1.5、上層 r 0.95、紫色錐頂到 7.85；`mage_tower_detail`：4 條石環、拱門（−Y）、6 扇發光拱窗、12 個城垛、金色尖頂到 8.0 | 石 (0.45,0.43,0.40)、深石 (0.25,0.24,0.23)、頂 (0.20,0.05,0.38)、金 (0.85,0.60,0.10)、窗 (0.95,0.75,0.25) |
| temple_ruins | `temple_ruins-col`：三層石階 6×4 / 5.6×3.6 / 5.2×3.2（各 0.2 高）、青苔片、6 根柱（x −2/0/2 × y ±1.1，(2,−1.1) 斷在 1.9、(−2,1.1) 斷在 1.2，斷口頂點抖動）、兩段門楣（頂到 4.0）；`temple_ruins_detail`：祭壇 + 浮空八面體藍水晶（z 1.43–2.27）、倒下的柱段、碎石 | 石 (0.58,0.55,0.47)、深石 (0.38,0.36,0.31)、苔 (0.12,0.26,0.06)、水晶 (0.10,0.80,1.0) |

---

## 附錄 E：地形生成（terrain）

座標以 Godot (x, z) 計算；Blender 頂點 = (x, −z, BASE + h)。

**高度場**
- 格網：HALF = 20.5、STEP = 0.5（82×82 格、83×83 頂點）、BASE = 1.5（土層厚度）、MAX_H = 0.9
- 平整距離 `flat_distance(x, z)` = 到下列區域的最短距離（區域內為 0）：
  - `world_layout.props` 中 house / mage_tower / temple_ruins / wall_brick / crate / barrel 的實際佔地（assets.json 的 X、Z 尺寸 × scale，依 yaw 旋轉的矩形；Godot yaw：local x = (cos, 0, −sin)、local z = (sin, 0, cos)）
  - 所有道路（線段距離 − 寬度/2）、廣場（圓）、`flat_pads`（圓）
- 遮罩 `mask = smoothstep(0.6, 3.1, flat_distance) × smoothstep(0, 2, 18.5 − max(|x|, |z|))`
- 丘陵：`hills = 0.30·sin(0.23x + 1.3)·cos(0.19z − 0.4) + 0.2·sin(0.13(x − z) + 2.1) + Σ a·exp(−((x−bx)² + (z−bz)²) / (2s²))`
  - (bx, bz, a, s)：(−14, 9, 0.8, 3.5)、(10, 14, 0.7, 3.2)、(−15, −11, 0.55, 3.0)、(15.5, 6, 0.6, 2.8)、(−3, 15, 0.45, 2.8)、(2.5, 4, 0.35, 2.2)、(−5.5, 1.5, −0.35, 2.4)、(11, 1.5, 0.4, 2.2)、(−12, 1.5, 0.5, 2.6)、(6, −4, 0.3, 2.0)
- `h = hills × mask`，整體縮放到最大值 = MAX_H，再加 `uniform(−0.025, 0.025) × mask` 的抖動（seed 53）。本次結果約 −0.35 ~ +0.92 m，flat_pads 全為 0

**地面網格 `terrain_ground-col`**
- 每格兩個三角形，對角線依 (ix + iz) 奇偶交替；每個三角形依中心點決定顏色：
  - 廣場內或石板路內 → `cobble_bed`
  - 泥土路內 → `dirt`（15% `dirt_dark`）；路邊 0.35 m 內 55% → `dirt_edge`
  - 其他草地：2% `grass_yellow`；否則 `n = sin(0.22x + 1.5·sin(0.15z))·cos(0.2z − 0.08x) + uniform(−0.2, 0.2)`，n > 0.45 → `grass_light`、n < −0.45 → `grass_dark`、其餘 `grass_mid`（隨機成分太大會像碎紙片）
- 側面：邊界環往下分四段 `soil_top`（1.5→1.25）、`soil`（→0.75）、`soil_rock`（→0.6）、`soil`（→0），底面封閉；最後 recalc normals
- 顏色（linear）：grass_mid (0.10,0.30,0.06)、grass_light (0.13,0.35,0.07)、grass_dark (0.08,0.25,0.05)、grass_yellow (0.24,0.36,0.07)、dirt (0.34,0.21,0.08)、dirt_dark (0.24,0.14,0.05)、dirt_edge (0.20,0.24,0.07)、cobble_bed (0.20,0.15,0.10)、soil_top (0.07,0.04,0.02)、soil (0.13,0.07,0.03)、soil_rock (0.20,0.19,0.17)

**細節物件（不加 -col）**
- `terrain_cobble`：石塊 0.07 高、高出地面 0.02，三種灰 (0.30,0.29,0.27) / (0.22,0.21,0.20) / (0.40,0.38,0.34) 隨機（2:1:1），每塊再隨機轉 ±0.15 rad
  - 廣場：中心一塊 0.3；之後每 0.32 m 一圈（從 r 0.48 到 半徑 − 0.12），每圈 `max(6, round(2πr / 0.34))` 塊 0.27×0.24，沿切線排
  - 石板路：沿路每 0.31 m 一排，每排橫向每 0.3 m 一塊 0.28×0.26，奇數排錯開 0.15，廣場內不鋪。本次共 431 塊
- `terrain_flora`（只放在「開放草地」：牆內 19.5 m、離道路 > 0.25、離廣場 > 0.3、不在非樹 / 草叢物件佔地 + 0.15 內、離樹幹 > 0.35×scale）
  - 草叢 1400 撮：每撮 3 片三角形葉（底寬 0.07、高 0.16–0.32、往外傾 0.04–0.12），兩種綠 (0.12,0.36,0.06) / (0.22,0.48,0.08)
  - 小花：14 群，每群 18 朵、高斯分布 σ 1.0 m；每朵一根莖（三角形）+ 頂端 5 瓣扁平五邊形，高 0.12–0.2；每群一色：白 (0.90,0.90,0.85)、黃 (0.95,0.72,0.10)、粉 (0.90,0.35,0.55)、紫 (0.45,0.25,0.80)
- `terrain_pebbles`：每條路每公尺 4 顆（從距起點 3.2 m 開始），放在路邊外 0.02–0.35 m，r 0.035–0.08 壓扁抖動 icosphere，用石板的三種灰
- 本次總面數約 30,600（上限 40,000）

**高度表 `pipeline/terrain_heights.json`**

```json
{"_note": "...", "origin": [-20.5, -20.5], "step": 0.5, "count": 83, "heights": [[...83 個...], ...83 列...]}
```

`heights[iz][ix]` 是 x = origin[0] + ix·step、z = origin[1] + iz·step 的地表高度（Godot y，已含抖動），四捨五入到 4 位小數。

---

## 附錄 F：Godot 場景與腳本規格

### F.1 互動道具場景

**`tree.tscn`**：root `Tree`（Node3D，script tree.gd；`push_gain = 0.25`、`wind_amplitude = 0.025`、`wind_frequency = 0.35`）
- `Model`：tree.glb instance
- `Leaves`：CPUParticles3D，位置 y 2.6；emitting false、one_shot、amount 32、lifetime 3.0、explosiveness 0.85、randomness 0.5、emission_shape box (0.8, 0.4, 0.8)、direction (0,1,0)、spread 180、gravity (0, −1.5, 0)、initial_velocity 0.4–1.4、angular_velocity ±180、damping 0.6–1.2、angle 0–360、scale 0.7–1.3、scale curve (0,1)→(0.8,1)→(1,0)、color_initial_ramp 綠 (0.1,0.4,0.05) → 黃綠 (0.45,0.55,0.1) → 橘 (0.75,0.42,0.05)
- 葉片 mesh：QuadMesh 0.22×0.16；材質 cull disabled、vertex color as albedo、billboard particles、keep scale

**`bush.tscn`**：root `Bush`（Node3D，script bush.gd；stiffness 60、damping 6、push_gain 0.12、wind_amplitude 0.05、wind_frequency 0.7）
- `Model`：bush.glb instance
- `Trigger`：Area3D（monitorable false）+ CylinderShape3D r 0.55、h 0.8，y 0.4

**`barrel.tscn`**：root `Barrel`（RigidBody3D，mass 3、PhysicsMaterial friction 0.5）
- CollisionShape3D：CylinderShape3D r 0.3、h 0.8，y 0.4
- `Model`：barrel.glb instance

### F.2 腳本

**`wind.gd`**（`extends RefCounted`，只有常數，其他腳本用 preload 取用）：DIRECTION = (0.96, 0, 0.28)、CLOUD_SPEED = 1.2 m/s、GUST_WAVELENGTH = 18 m

**`sway.gd`**（Node3D；`$Model` 以底部為支點傾斜）
- export：stiffness 40、damping 5、push_gain 0.15、max_angle 0.35、wind_amplitude 0、wind_frequency 0.4
- `push(direction, strength)`：方向轉成 local XZ，`angular_velocity += 方向 × strength × push_gain`
- `_physics_process`：阻尼彈簧 `ω += (−k·θ − c·ω)·dt`、`θ = (θ + ω·dt)` 限制在 max_angle；接近 0 時歸零
- 風：`gust = 0.6 + 0.4·sin(2π·f·t − phase)`，`phase = 2π·(位置 · DIRECTION) / GUST_WAVELENGTH`（陣風沿風向推進）；傾斜 = 彈簧角 + 風向(local) × wind_amplitude × gust
- 套用：`Model.basis = Basis(UP × tilt 方向, |tilt|)`

**`tree.gd`**（extends sway.gd）：覆寫 push，先 `super.push`；strength ≥ 1.0 且距上次 ≥ 36 個 physics frame 時 `Leaves.restart()` 並印 `TREE_HIT <name> <strength>`

**`bush.gd`**（extends sway.gd）：`_ready` 先 `super()`；Trigger 的 body_entered / body_exited 取對方水平速度（CharacterBody3D 用 velocity、RigidBody3D 用 linear_velocity），≥ 0.1 m/s 就 `push(速度方向, 速率)`

**`coin.gd`**：Area3D；`signal collected`；每秒自轉 180°；body 名稱為 `Player` 時 emit 後 `queue_free()`

**`goal.gd`**：Area3D；`signal reached`；body 名稱為 `Player` 時 emit

**`clouds.gd`**（Node3D；export `sun: DirectionalLight3D`）
- 10 朵 cloud.glb，seed 2026，高度 13–17 m（高於跟隨鏡頭，近看只看到雲影），scale 0.9–1.7、yaw ±0.4
- 分布中心 `area_center = −(陽光方向的水平分量) × 平均高度 / (−陽光方向.y)`（附錄 C-15），在中心 ±34 m 的正方形內
- 每幀沿 `DIRECTION × CLOUD_SPEED` 移動，超出範圍就 wrap 到另一側

**`player.gd`**（CharacterBody3D）
- 常數：MAX_SPEED 4、BOOST_SPEED 7、MASS 2、DRAG 4（1/s）、ROLLING_FRICTION 0.15、BRAKE_FRICTION 0.8、AIR_CONTROL 0.3、JUMP_VELOCITY 4.5、REACH_DISTANCE 0.3、WHEEL_RADIUS 0.16、LEAN_PER_ACCEL 0.04、MAX_LEAN 0.3、LEAN_RESPONSE 10、HIT_MIN_SPEED 0.5；重力讀 ProjectSettings
- `static get_drive_force(top_speed, g) = MASS·(μ·g + DRAG·top_speed)`（地面極速剛好等於 top_speed）
- `static step_velocity(v, dir, on_floor, top_speed, is_braking, g, dt)`：煞車時 dir = 0；地面 drive = F、drag = DRAG；空中 drive = F·AIR_CONTROL、drag = drive / (MASS·top_speed)；`v += (dir·drive/MASS − v·drag)·dt`；地面再扣常數摩擦 `μ·g·dt`（煞車用 BRAKE_FRICTION），不會反向
- 輸入：`Input.get_vector(ui_left, ui_right, ui_up, ui_down)` 轉成相對鏡頭的水平方向；`ui_accept` 在地面時跳躍；按住 boost → top_speed = 7；按住 brake → 煞車
- bot 模式（`"--bot" in OS.get_cmdline_user_args()`）：忽略輸入，`_ready` 先記下 Coin1 → Coin2 → Coin3 → Goal 的位置（金幣會被 free），依序直線前往，水平距離 < 0.3 m 換下一個；不加速、不煞車
- 碰撞（move_and_slide 後）：撞 RigidBody3D → `apply_central_impulse(推方向 × (折合質量 × 接近速度 + 驅動力 × dt))`；其他碰撞的衝擊速度 ≥ 0.5 → 往上找有 `push()` 的祖先（樹）並呼叫 `push(−normal, 衝擊速度)`
- 視覺：面向水平速度方向（> 0.2 m/s 才更新）；依實際水平加速度傾斜（× 0.04 rad，最多 0.3，平滑係數 10）；`Visual.basis = tilt × Basis(UP, yaw)`
- 輪子：`_ready` 把 `wheel_spin` 設為 LOOP_LINEAR、length = 最後一個 key 時間、播放；每幀 `speed_scale = 前進速度 / (2π·0.16) × length`

**`main.gd`**（Node3D）
- 每收集一枚金幣印 `COIN <n>/3`；碰到 Goal 時金幣收齊印 `WIN`（bot 模式接著 `get_tree().quit()`），否則印 `NEED_COINS`
- 鏡頭：yaw 45°（π/4）、pitch −40°（−0.698 rad）、FOV 40；`camera.global_basis = Basis.from_euler(pitch, yaw, 0)`；位置 = 平滑目標 + UP × 0.6 + basis.z × 距離；跟隨距離 18、平滑 `lerp(1 − exp(−4·dt))`
- `--overview`：目標固定 (4, 0, 4)、距離 64，陰影距離設為 128，截圖存 `screenshot_overview.png`
- 移軸景深：`camera.attributes`（CameraAttributesPractical）的 near 結束 = 距離 × 0.89、near transition × 0.17、far 開始 × 1.22、far transition × 0.55
- 非 headless 時啟動 3 秒後截圖到 `res://../pipeline/screenshot.png`（`await RenderingServer.frame_post_draw` 後存），印 `SCREENSHOT <path> (<結果>)`

### F.3 `main.tscn`

```text
Main (Node3D)                               script main.gd
├─ DirectionalLight3D   陽光：西南方、仰角 40°（-Z 指向 (0.542, -0.643, -0.542)），color (1, 0.91, 0.76)、energy 1.35、
│                       angular_distance 1.5、shadow on、shadow_blur 1.5、directional_shadow_max_distance 60
├─ WorldEnvironment     ProceduralSky（ground_bottom (0.55,0.7,0.85)、ground_horizon (0.65,0.78,0.9)）、
│                       ambient = sky 0.9、tonemap filmic、SSAO radius 1.2 / intensity 2.5 / power 1.5
├─ Ground               terrain.glb instance，y = −1.5（地表落在 y = 0）
├─ Player (CharacterBody3D)                 script player.gd，floor_snap_length 0.3
│  ├─ CollisionShape3D  CapsuleShape3D r 0.3、h 1.2，y 0.6
│  └─ Visual            player.glb instance
├─ Camera3D             attributes = CameraAttributesPractical（far / near blur on、amount 0.1）
├─ Coins (Node3D)
│  └─ Coin1–3 (Area3D)  script coin.gd，位置 (6,0,0) / (6,0,8) / (−4,0,8)
│     ├─ CollisionShape3D  SphereShape3D r 0.5（Phase 10 改 0.3），y 0.75（三個共用同一個 shape）
│     └─ coin.glb instance，y 0.5
├─ Crates (Node3D)      crate.glb ×3：(13.2, 0, −5.6) yaw 15°、(−9.2, 0, 13.6) yaw −20°、(4.55, 0.8, 11.05) yaw 35°（疊在南邊木箱上）
├─ Goal (Area3D)        script goal.gd，位置 (−8, 0, −6)
│  ├─ CollisionShape3D  BoxShape3D 1×2×1，y 1
│  └─ goal_flag.glb instance
├─ World                world.tscn instance（build_world.mjs 產生）
└─ Clouds (Node3D)      script clouds.gd，sun = ../DirectionalLight3D
```

---

## 參考

- godot-mcp：https://github.com/Coding-Solo/godot-mcp
- blender-mcp：https://github.com/ahujasid/blender-mcp
- Godot 命令列（`--import`、`--headless`、`--fixed-fps`、`--quit-after`、`-s`）：https://docs.godotengine.org/en/stable/tutorials/editor/command_line_tutorial.html
- Godot 3D 場景匯入名稱提示（`-col`、`-wheel` 等）：https://docs.godotengine.org/en/stable/tutorials/assets_pipeline/importing_3d_scenes/node_type_customization.html
- v1 playbook：`.spec/PLAYBOOK.md`；本次實作紀錄：`pipeline/log.md`、`pipeline/REPORT.md`
