# Blender × Godot MCP 遊戲 Pipeline — PoC Playbook

> v1 · 2026-10-26 · 給 Claude Code 執行

---

## 給人看的：怎麼開始

1. 建一個空資料夾，例如 `mcp-game-poc/`，把本檔放進去
2. 在該資料夾執行 `claude`
3. 輸入：`讀 PLAYBOOK.md，從頭執行。遇到 STOP 就停下來等我。`
4. 中斷或重啟後輸入：`讀 PLAYBOOK.md 和 pipeline/progress.md，從未完成的步驟繼續。`

需要你手動做的只有兩件事：
- Phase 0：安裝軟體
- Phase 2：開 Blender 裝 addon，然後重啟 Claude Code

---

## PoC 目標

做一個 3D 小遊戲「Coin Run」：角色在地面移動，收集 3 枚金幣後碰到旗子就勝利。
所有 3D 模型都由 Blender MCP 產生，Godot MCP 負責組裝與執行；規格不符時回 Blender 調整。

### 完成定義（DoD）

- [ ] 4 個資產（player / coin / crate / goal_flag）的 `check_assets` 全部 PASS
- [ ] Bot 自動測試輸出 `COIN 3/3` 和 `WIN`，且沒有 `SCRIPT ERROR`
- [ ] 透過 Godot MCP `run_project` 執行，`get_debug_output` 無錯誤，截圖已檢查
- [ ] 完成一次「迴圈演練」（規格變更 → FAIL → Blender 調整 → PASS），並留有紀錄
- [ ] `pipeline/REPORT.md` 已寫

---

## Pipeline 總覽

```
assets.json（資產規格，唯一真相）
   │
   ▼
Blender MCP ── 建模 → 截圖自檢 → 標準 export（同時寫 stats）
   │   產出 game/assets/<id>.glb + pipeline/blender_stats/<id>.json
   ▼
node tools/import.mjs          （godot --headless --import）
   ▼
node tools/check_assets.mjs    （Blender stats + Godot 實測 vs 規格）
   ├─ FAIL → 調整請求寫進 pipeline/log.md → 回 Blender（每資產每輪最多 3 次）
   └─ PASS
        ▼
Godot MCP ── create_scene / add_node / save_scene
   ▼
headless bot 測試 → run_project + get_debug_output → 遊戲內截圖檢查
```

---

## 給 Claude Code 的執行規則

1. 每完成一個編號步驟，就在 `pipeline/progress.md` 打勾。重啟後靠它續跑。
2. 看到 **STOP**：清楚告訴使用者要做什麼，結束這一輪，等使用者回覆。
3. 每個 Phase 完成後執行 `git commit`。
4. 本 playbook 寫死的檔案內容要照抄；只給規格的程式由你實作，但必須符合規格。
5. MCP 工具失敗時：先重試 1 次 → 改用替代做法（例如直接編輯檔案）→ 記到 `pipeline/log.md` 的「MCP 問題」段落。
6. `pipeline/progress.md` 的內容 = 本檔各 Phase 的編號步驟清單。

---

## Phase 0：環境檢查（人工安裝，Claude 只檢查）

| 軟體 | 版本 | 備註 |
|---|---|---|
| Godot | ≥ 4.3 | Windows 請用 `*_console.exe`，才抓得到 stdout |
| Blender | 4.x | |
| Node.js | ≥ 18 | godot-mcp 需要 |
| uv | 最新 | blender-mcp 透過 `uvx` 啟動 |
| git | 任意 | |

步驟：
1. 執行 `node --version`、`uv --version`、`git --version`。
2. 依以下順序找 Godot 執行檔：
   - 環境變數 `GODOT_PATH`
   - PATH 裡的 `godot` 或 `godot4`
   - macOS 預設路徑 `/Applications/Godot.app/Contents/MacOS/Godot`
   - 都找不到就問使用者
3. 執行 `<godot> --version`，確認版本 ≥ 4.3。
4. 任一項缺少 → **STOP**，列出缺什麼、去哪裡下載。

---

## Phase 1：專案骨架

目錄結構：
```
mcp-game-poc/
├─ PLAYBOOK.md
├─ CLAUDE.md
├─ .mcp.json
├─ .claude/settings.json
├─ .gitignore
├─ pipeline.config.json
├─ assets.json
├─ pipeline/
│  ├─ progress.md
│  ├─ log.md
│  └─ blender_stats/
├─ tools/
│  ├─ check_env.mjs
│  ├─ import.mjs
│  ├─ check_assets.mjs
│  └─ bot_test.mjs
├─ blender/
│  ├─ addon.py
│  └─ assets.blend        （之後由 Blender 存檔產生）
└─ game/
   ├─ project.godot
   ├─ assets/             （.glb 放這裡）
   ├─ scenes/
   ├─ scripts/
   └─ tools/inspect_assets.gd
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

### 1.2 `assets.json`

軸向一律用 **Godot 座標**：`size_m = [X 寬, Y 高, Z 深]`，單位是公尺。

```json
{
  "assets": {
    "player": {
      "desc": "低模角色：膠囊身體、球形頭、兩個黑色眼睛，平塗顏色，正面朝 Blender -Y",
      "size_m": [0.6, 1.8, 0.6],
      "origin": "bottom_center",
      "max_tris": 3000,
      "collision": "none",
      "anims": []
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
      "desc": "木箱，邊緣有深色框條，平塗顏色",
      "size_m": [1.0, 1.0, 1.0],
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
    }
  }
}
```

`collision` 欄位：
- `"none"`：只有外觀，碰撞體在 Godot 端建立
- `"col"`：Blender 的 mesh 物件名稱加 `-col` 後綴，Godot import 時會自動產生 StaticBody3D

### 1.3 `.mcp.json`

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

Windows 原生（非 WSL）：`command` 改成 `cmd`，`args` 最前面加 `"/c"`，例如 `["/c", "npx", "-y", "@coding-solo/godot-mcp"]`。

### 1.4 `.claude/settings.json`

```json
{
  "permissions": {
    "allow": [
      "mcp__godot",
      "Bash(node tools/check_env.mjs:*)",
      "Bash(node tools/import.mjs:*)",
      "Bash(node tools/check_assets.mjs:*)",
      "Bash(node tools/bot_test.mjs:*)"
    ]
  }
}
```

Blender MCP 刻意不放進自動允許清單：`execute_blender_code` 會執行任意 Python，要不要自動允許，讓使用者在提示出現時自己決定。

### 1.5 `.gitignore`

```
game/.godot/
blender/*.blend1
pipeline/screenshot.png
```

### 1.6 `game/project.godot`

```
config_version=5

[application]
config/name="MCP Game PoC"
run/main_scene="res://scenes/main.tscn"
config/features=PackedStringArray("<主.次版本，例如 4.5>")
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

所有工具都讀 `pipeline.config.json`，cwd 為專案根目錄，並用 `child_process.spawnSync` 呼叫 Godot。

**`tools/check_env.mjs`**
- 確認 `godotPath` 可執行，並印出版本
- 確認 `localhost:<blenderPort>` 的 TCP 連線可建立，印出 OK 或 NG
- 全部 OK → exit 0，否則 exit 1

**`tools/import.mjs`**
- 執行 `<godotPath> --headless --path game --import`，timeout 300 秒
- 印出含 `ERROR` 或 `WARNING` 的行
- 結束碼非 0 → exit 1

**`tools/check_assets.mjs [id ...]`**（不帶參數 = 檢查全部資產）
- 執行 `<godotPath> --headless --path game -s res://tools/inspect_assets.gd -- res://assets/<id>.glb ...`，解析輸出中 `INSPECT_JSON:` 開頭的那一行
- 讀取 `pipeline/blender_stats/<id>.json`
- 每個資產逐項檢查，印出 PASS 或 FAIL：

| 項目 | 規則 |
|---|---|
| 載入 | inspect 結果沒有 `error` |
| Blender 尺寸 | `stats.size_m` 與 spec 的誤差 ≤ max(relative × spec, absoluteMin) |
| Godot 尺寸 | `inspect.aabb_size` 套用同樣規則 |
| 原點 | `bottom_center`：Godot 的 `aabb_min.y`、X/Z 中心的絕對值都 ≤ 0.02 |
| 面數 | `stats.tris` ≤ `max_tris` |
| 碰撞 | `collision` 為 `"col"` 時，`has_collision` 必須是 true |
| 動畫 | `anims` 裡每個名稱都要出現在 `inspect.anims` |

- FAIL 時印出一行具體修正建議，例如：`FIX player: Y 高 3.60m，規格 1.80m → 在 Blender 縮放 0.5 後 apply scale`
- 如果 Blender 尺寸 PASS 但 Godot 尺寸 FAIL → 額外印出 `PIPELINE: export/軸向問題，不是建模問題`
- 最後一行印 `SUMMARY: <n> pass, <m> fail`；有任何 FAIL → exit 1

**`tools/bot_test.mjs`**
- 執行 `<godotPath> --headless --path game --fixed-fps 60 --quit-after 1800 -- --bot`
- PASS 條件：輸出含 `COIN 3/3` 和 `WIN`，且沒有 `SCRIPT ERROR` 或 `ERROR:`
- 印出結果與相關 log 行；FAIL → exit 1

### 1.9 下載 Blender addon

```
curl -L -o blender/addon.py https://raw.githubusercontent.com/ahujasid/blender-mcp/main/addon.py
```

### 1.10 `CLAUDE.md`（照抄）

```
# MCP Game PoC

流程與步驟以 PLAYBOOK.md 為準；進度記在 pipeline/progress.md。

## 座標與單位
- 單位是公尺。assets.json 的 size_m 一律用 Godot 軸向 [X, Y(上), Z]
- Blender 是 Z 朝上：Godot (X, Y, Z) = Blender (X, Z, -Y)
- 角色正面朝 Blender -Y，匯出後在 Godot 是朝 +Z

## Blender 規則
- 每個資產一個 collection，名稱 = asset id，在世界原點建模
- 匯出前該 collection 必須在 view layer 中可見（不可 exclude）
- 原點 bottom_center：底部 z=0、XY 置中
- 低模、平塗：只用 Principled BSDF 的 Base Color，不用圖片貼圖
- collision 為 "col" 的資產，mesh 物件名稱加 -col 後綴
- execute_blender_code 每次只做一小步；不刪專案外的檔案；不下載外部資產
- 匯出只用 PLAYBOOK 附錄 A 的標準 snippet

## 每次匯出後必做
1. node tools/import.mjs
2. node tools/check_assets.mjs <id>

## 迴圈規則
- FAIL → 依 PLAYBOOK 附錄 B 的格式寫進 pipeline/log.md → 回 Blender 修
- 同一資產每輪最多 maxIterations 次，超過就停下來問使用者
- 不要手動修改 .glb 或 .import 檔

## Godot 規則
- 腳本放 game/scripts/，場景放 game/scenes/
- 輸入只用內建的 ui_left / ui_right / ui_up / ui_down / ui_accept
- 優先使用 Godot MCP 工具；做不到再直接編輯 .tscn，並記到 log 的「MCP 問題」
```

### 1.11 初始化

1. 建立 `pipeline/progress.md`（內容為各 Phase 的步驟清單）和 `pipeline/log.md`
2. 執行 `git init`
3. 執行 `node tools/import.mjs`（第一次執行會建立 `game/.godot`）
4. `git commit -m "phase1: scaffold"`

---

## Phase 2：連線 MCP（**STOP**，需要人工）

告訴使用者依序完成：
1. 開 Blender → Edit → Preferences → Add-ons → Install（4.2 以上在右上角 ⌄ 選 Install from Disk）→ 選 `blender/addon.py` → 勾選「Interface: Blender MCP」
2. 在 3D 視窗按 `N` → 切到 BlenderMCP 分頁 → 不要勾 Poly Haven / Hyper3D → 按 Connect 或 Start Server（按鈕名稱依版本而定）
3. 退出 Claude Code，在專案資料夾重新執行 `claude`；出現 `.mcp.json` 伺服器核准提示時選允許
4. 輸入：`讀 PLAYBOOK.md 和 pipeline/progress.md，繼續`

重啟後由 Claude 驗證：
1. `node tools/check_env.mjs` 全部 OK
2. 呼叫 `mcp__godot__get_godot_version` 成功
3. 呼叫 `mcp__blender__get_scene_info` 成功（第一次呼叫失敗很常見，重試一次）
4. 任一項失敗 → **STOP**，回報是哪一項失敗

---

## Phase 3：單一資產冒煙測試（crate）

目的：先用最簡單的資產打通整條鏈。

1. Blender：刪掉預設 Cube，建立 collection `crate`，依 `assets.json` 建模（用多次小步驟的 `execute_blender_code`）
2. 用 `get_viewport_screenshot` 自檢外觀，與 desc 不符就修
3. 執行附錄 A 的標準 export snippet，`ASSET = "crate"`
4. `node tools/import.mjs`
5. `node tools/check_assets.mjs crate` → 必須 PASS（FAIL 就走迴圈規則）
6. Godot MCP：用 `create_scene` 建立 `res://scenes/test_crate.tscn`（root 為 Node3D），把 `crate.glb` instance 進去。若 `add_node` 無法 instance .glb，就直接編輯 .tscn 加入 ext_resource
7. Godot MCP：`run_project` 執行 test_crate 場景（如果不支援指定場景，就暫時把 main_scene 指向它）→ 用 `get_debug_output` 確認無錯誤 → `stop_project`
8. `git commit`

---

## Phase 4：其餘資產

1. 對 player、coin、goal_flag 依序重複 Phase 3 的步驟 1–5（每個資產用自己的 collection）
2. 全部做完後執行 `node tools/check_assets.mjs`，必須全部 PASS
3. `git commit`

---

## Phase 5：Godot 遊戲實作

### 場景 `res://scenes/main.tscn`

```
Main (Node3D)                     script: res://scripts/main.gd
├─ DirectionalLight3D（開陰影）
├─ WorldEnvironment（簡單天空）
├─ Ground (StaticBody3D)
│  ├─ MeshInstance3D：PlaneMesh 20×20
│  └─ CollisionShape3D：BoxShape3D 20×0.2×20，y = -0.1
├─ Player (CharacterBody3D)       script: res://scripts/player.gd
│  ├─ CollisionShape3D：CapsuleShape3D 半徑 0.3、高 1.8，y = 0.9
│  └─ Visual：player.glb instance
├─ Camera3D
├─ Coins (Node3D)
│  └─ Coin ×3 (Area3D)            script: res://scripts/coin.gd
│     ├─ CollisionShape3D：SphereShape3D 半徑 0.5，y = 0.75
│     └─ coin.glb instance，y = 0.5
├─ Crates (Node3D)：crate.glb instance ×3
└─ Goal (Area3D)                  script: res://scripts/goal.gd
   ├─ CollisionShape3D：BoxShape3D 1×2×1，y = 1
   └─ goal_flag.glb instance
```

### 固定配置（bot 走直線不會撞到箱子）

| 物件 | 位置 (x, z) |
|---|---|
| Player | (0, 0) |
| Coin 1 / 2 / 3 | (3, 0) / (3, 4) / (-2, 4) |
| Goal | (-4, -3) |
| Crate ×3 | (6, 2) / (0, -5) / (-6, 1) |

### 腳本規格

**`player.gd`**
- 用 `ui_left` / `ui_right` / `ui_up` / `ui_down` 沿世界軸移動，速度 4 m/s；`ui_accept` 跳躍；重力讀 ProjectSettings
- Visual 面向移動方向：`rotation.y = atan2(dir.x, dir.z)`（glb 的正面是 +Z）
- Bot 模式：當 `"--bot" in OS.get_cmdline_user_args()` 時忽略輸入，依序走向 Coin1 → Coin2 → Coin3 → Goal；距離目標 < 0.3 m 就換下一個

**`coin.gd`**
- 每秒自轉 180°
- `body_entered` 的對象是 Player 時通知 main，然後 `queue_free()`

**`goal.gd`**
- Player 進入時：金幣已收齊 → 由 main 印出 `WIN`；否則印出 `NEED_COINS`

**`main.gd`**
- 每收集一枚金幣印出 `COIN <n>/3`
- 相機跟隨：位置 = player 位置 + (0, 8, 8)，並 `look_at` player
- 非 headless 時（`DisplayServer.get_name() != "headless"`），啟動 3 秒後截圖存到 `ProjectSettings.globalize_path("res://").path_join("../pipeline/screenshot.png")`
- 印出 `WIN` 後，若是 bot 模式就呼叫 `get_tree().quit()`

### 步驟

1. 用 Godot MCP 的 `create_scene` 建立 main.tscn，用 `add_node` 加入內建節點，再 `save_scene` 存檔。glb instance 和 shape 參數如果 MCP 做不到，就直接編輯 .tscn（記錄到 log 的「MCP 問題」）
2. 腳本用檔案工具直接撰寫
3. `node tools/import.mjs`（順便處理 UID；若仍有 UID 警告，用 Godot MCP 的 `update_project_uids`）
4. `node tools/bot_test.mjs` → 必須 PASS。失敗時先修腳本或場景；若判斷是資產問題，就走迴圈規則
5. Godot MCP：`run_project` → 等 5 秒 → `get_debug_output` 確認無錯誤 → `stop_project`
6. 讀取 `pipeline/screenshot.png` 檢查：四種資產都看得到、比例合理、沒有倒立或躺平。不合理時判斷是資產問題（回 Blender）還是場景問題（修 Godot）
7. `git commit`

---

## Phase 6：迴圈演練

模擬「Godot 端發現資產需要修改」：

1. 在 log 寫下：`Godot 回饋：coin 太大、擋視線 → 規格改為 [0.3, 0.3, 0.06]`
2. 修改 `assets.json` 的 `coin.size_m`
3. `node tools/check_assets.mjs coin` → 這時應該 FAIL（確認判斷機制有效）
4. 依 FIX 建議在 Blender 調整 → 標準 export → import → check → PASS
5. 同步把 Coin 的 SphereShape3D 半徑改為 0.3，確認 `node tools/bot_test.mjs` 仍然 PASS
6. 重跑 `run_project` 確認截圖，然後 `git commit`

---

## Phase 7：收尾

1. 撰寫 `pipeline/REPORT.md`，內容包含：
   - DoD 各項的勾選結果
   - 每個資產的迭代次數
   - MCP 問題清單與當時採用的替代做法
   - 卡最久的步驟，以及建議的下一步（例如拆 sub-agent、加 hook 自動 import）
2. `git commit`
3. 告訴使用者 PoC 已完成，並附上 REPORT 摘要

---

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

---

## 附錄 B：調整請求格式（寫入 `pipeline/log.md`）

```
## <時間> <asset> iter <n>
- check: FAIL — <check_assets 印出的 FIX 行>
- 判斷: 建模問題 / pipeline 問題
- 對 Blender 的請求: <具體操作，例如「縮放 0.5 後 apply scale」>
- 結果: PASS / FAIL
```

另開一個段落 `## MCP 問題`，記錄工具失敗的情況與當時採用的替代做法。

---

## 已知限制

- Blender MCP 用 bpy 建模，適合低模、簡單造型；複雜角色不在本 PoC 範圍
- Godot MCP 沒有截圖工具，本 PoC 改由遊戲內截圖存檔，再讓 Claude 讀圖
- 每次 `--import` 都會啟動一次 headless editor，需要幾秒鐘

---

## 參考

- godot-mcp README：https://github.com/Coding-Solo/godot-mcp
- blender-mcp README：https://github.com/ahujasid/blender-mcp
- Godot 命令列參考（`--import`、`--headless`、`--fixed-fps`、`--quit-after`、`-s`）：https://docs.godotengine.org/en/stable/tutorials/editor/command_line_tutorial.html
- `--import` 參數加入的 PR：https://github.com/godotengine/godot/pull/90431
