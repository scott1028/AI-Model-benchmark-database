# Coin Run — 專案規則（給 agent）

> 本檔是執行 `.spec/PLAYBOOK_v2.1.md` 時的專案級規則。`CLAUDE.md` 內容與本檔相同。
> 衝突優先序：PLAYBOOK 的 DoD > 使用者需求表 > 本檔 > 美術判斷。

## 1. 目錄配置

| 路徑 | 內容 | 可手動編輯 |
|---|---|---|
| `.spec/PLAYBOOK_v2.1.md` | 工作目標（唯讀） | 否 |
| `specs/<id>.json` | 資產規格＝唯一真相 | 是（改規格＝在這裡改） |
| `blender/<id>.blend` | Blender 原始檔（每個資產一份） | 只用 Blender MCP 改 |
| `godot/assets/<id>/<id>.glb` | Blender 匯出產物 | **禁止手動編輯** |
| `godot/assets/<id>/<id>.glb.import` | Godot 匯入設定 | **禁止手動編輯** |
| `godot/` | Godot 專案（scenes/scripts/scenes 等） | 是 |
| `godot/web_shell.html` | 自訂 Web 殼層（`export_presets.cfg` 的 `html/custom_html_shell`）：非安全來源放行 Secure Context（nothreads 版）＋把 `AudioWorklet` 接成靜音，純 HTTP 的 `http://<hostname>/…` 才玩得起來 | 是（改完 `make web` 重新匯出） |
| `tools/` | 自動化工具（Python + GDScript） | 是 |
| `pipeline/progress.md` | 各 Phase 步驟清單，`[v]`/`[x]`/`[ ]` | 是（每完成就更新） |
| `pipeline/log.md` | 設計決策、檢查與修正紀錄、MCP 問題 | 是（只增不刪） |
| `pipeline/checks/<id>.json` | 規格檢查的機器可讀結果 | 由工具產生 |
| `pipeline/screenshots/` | 遊戲截圖 | 由工具產生 |
| `index.html` | 入口頁（本專案層，導向 `web/index.html`）。仓库根另有一份同名入口頁導向 `<本目錄>/web/index.html`，給「從上層開 http server」用 | 是 |
| `web/` | WebGL 版輸出（`make web` 產生，不在 git 內） | 否（要改就改完重新 `make web`） |

## 2. 單位與座標約定

- 1 Blender unit = 1 meter；Godot 端 1 unit = 1 m。匯出不縮放。
- Blender 是 Z-up，Godot 是 Y-up。glTF 匯出用預設 `+Y up`，軸向換算：
  - Blender `(x, y, z)` → Godot `(x, z, -y)`
  - 角色正面：Blender **+Y** → Godot **-Z**（Godot 的「前方」）
- 對稱於世界：Blender 的 +X = Godot 的 +X（地圖向右），Blender 的 +Y = Godot 的 -Z（地圖向後）。版面座標一律以 **Godot 座標**為基準記錄（`specs/layout.json`）。
- 原點規則：**所有資產原點 = 接地面中心**（模型在局部 `Z >= 0`，`X/Y` 依碰撞足跡置中）。懸空物件（金幣、雲）也是底部中心，Godot 端用節點位置抬升。
- 碰撞：**原始體形狀（box / cylinder / capsule / box_pair）在規格宣告參數，由 Godot 端 `WorldBuilder` 建立**，不在 Blender 建模碰撞。
  只有不規則外形（`collision.shape == "mesh"`，例：rock）才在 Blender 建 `<id>_collision`（低面數凸包、自訂屬性 `crcollider=1`、`hide_render=True`）。
  - 碰撞網格**不可**命名為 `<id>_col`：Godot 4.7 的 glTF 匯入器對 `_col` 後綴會自動產生 StaticBody3D **且保留一份白色視覺網格**（2026-10-28 實測），渲染與物理都偏離規格。`_collision` 不觸發該慣例，物理完全由 `WorldBuilder` 依 `collision.mode` 產生 `StaticBody3D`／`RigidBody3D`／`Area3D` 並隱藏碰撞網格。

## 3. 資產規格檔（唯一真相）

`specs/<id>.json`：

```json
{
  "id": "coin",
  "title": "金幣",
  "intent": "可被收集的金幣",
  "dims_m": { "x": 0.7, "y": 0.12, "z": 0.7 },
  "tolerance_pct": 10,
  "tolerance_min_cm": 2,
  "tris_max": 900,
  "origin": "bottom_center",
  "collision": { "mode": "sensor", "shape": "cylinder", "note": "收集判定" },
  "animations": [],
  "materials": [{ "name": "coin_gold", "color": "#E8B92C", "flat": true }],
  "gameplay": { "collectible": true },
  "notes": "直立圓形硬幣，邊緣有齒紋"
}
```

- `collision.mode`：`none`（可穿過，不建碰撞體）/ `body`（擋住機器人）/ `sensor`（觸發判定）/ `rigid`（可被推的動態剛體）
- `collision.shape`：`box`（補 `size_m`）/ `cylinder`、`capsule`（補 `radius_m`、`height_m`，可加 `offset_m`）/ `box_pair`（補 `boxes: [{size_m, offset_m}]`）/ `mesh`（要在 Blender 建 `<id>_collision`）
- `animations`：需要 Blender 製作並匯出的動畫，例：`[{ "name": "wheel_roll", "fps": 30, "frames": 24, "target": "wheel" }]`
- `materials[].name` 要和 Blender 裡的材料名稱一致；平塗顏色（不用貼圖）。
- 面數與尺寸只算視覺網格；`_collision` 不計入。

## 4. Blender 建模與匯出規則

1. 一個資產一個 `.blend`：`blender/<id>.blend`，scene 根下只放該資產的節點。
2. 建模前讀規格；建模後先 `look` 自檢外觀（比例、倒立、破面、法線、原點），再匯出。
3. `execute_blender_code` 每次只做一小步（一個邏輯動作），立刻 `get_scene_info` 或 `look` 確認。
4. 套用全部修改器後匯出，GLB 二進位，`godot/assets/<id>/<id>.glb`。
   **一律用 `tools/blender_export.py`**（經 `runpy` 從 Blender MCP 呼叫），匯出旗標集中在該檔，且會自動跳過此 Blender 版本不支援的旗標：
   `export_format='GLB'`、`export_yup=True`、`export_apply=True`、`export_animations=True`、
   `export_materials='EXPORT'`、`export_extras=True`、`export_cameras=False`、`export_lights=False`、只匯出選取範圍（`use_selection=True`，舊版為 `export_selected`）
5. 只有 `collision.shape == "mesh"` 的資產才建 `<id>_collision`：低面數凸包，自訂屬性 `crcollider=1`，`hide_render=True`；其餘資產不建碰撞網格（由 Godot 依規格參數建立）。
6. 不做的事：不刪除專案外的檔案、不下載外部資產、不使用外部資產庫／生成器、不使用非規格內的顏色。
7. 要改模型 → 只改 `.blend` 並重新匯出；**永不**手動改 `.glb` 或 `.glb.import`。

## 5. 每次匯出後必做檢查

```bash
make check ID=<id>     # tools/check_asset.py：Blender 端審計 → Godot 匯入 → 兩端比對規格
```

檢查項目（兩端都要）：尺寸 ±10%（絕對底限 2 cm）、原點在底部中心、面數 ≤ `tris_max`、
`collision` 所需碰撞網格存在、宣告的動畫存在且被 Godot 收到、材料名稱與顏色。

結果寫入 `pipeline/checks/<id>.json`：`pass` / `fail`、實測值、偏差、`cause`（`modeling` 或 `pipeline`）、具體修正建議。

### FAIL 迴圈

1. 記 log：時間、資產、第幾次、檢查結果、判斷、對 Blender 的修正請求、結果。
2. 回 Blender 修 → 重新匯出 → 重新檢查。
3. 同一資產每輪最多 3 次；第 3 次仍 FAIL → **STOP**，回報使用者。
4. 判斷為 `pipeline`（匯出設定、路徑、匯入器）時先修 pipeline，不改模型。

## 6. Godot 端規則

- 專案：`godot/project.godot`，主場景 `godot/scenes/main.tscn`，測試場景 `godot/scenes/smoke.tscn`。
- 資產放 `godot/assets/<id>/`；版面由 `godot/data/layout.json` 驅動，執行期由 `WorldBuilder` 產生（不逐一手動擺件）。
  - `godot/data/heightmap.json`（由 `tools/blender_terrain.py` 產生）：1 m 網格高度表。`rows[j][i]` 對應 `x = origin[0] + i*step`、`z_godot = origin[1] + j*step`，值是 **Godot y**；`node_y` 是 terrain 節點要擺的 y（ terrain 網格原點在底部中心，所以節點 y = −3.2 讓地表落在它應有的高度）。物件擺放與碰撞都用同一份高度表，地面才不會「看的和踩的不一樣」。
- 輸入用 action，名稱固定（使用者定案，**不得新增按鍵**）：

| action | 鍵盤 | 手把 |
|---|---|---|
| `move_up` / `move_down` / `move_left` / `move_right` | 方向鍵 | dpad |
| `jump` | Space、Enter | A（button 0） |
| `boost` | A 鍵（按住） | X（button 2，按住） |
| `brake` | B 鍵（按住） | B（button 1，按住） |

- bot 模式：啟動參數 `-- --bot`（`OS.get_cmdline_user_args()`），忽略玩家輸入，自動依序走向 3 枚金幣與終點，印出可判讀進度與勝利訊息。
- 測試：`make bot`、`make physics` 用 headless Godot 跑測試腳本，輸出 JSON 到 `pipeline/checks/`。
- 截圖：`make shot SCENE=res://scenes/main.tscn`，輸出 PNG 到 `pipeline/screenshots/`。

## 7. MCP 使用規則

- 建模只用 **Blender MCP**（`execute_blender_code`、`look`、`get_scene_info`、`get_addon_status`）。
- Godot 端優先用 **Godot MCP**：`get_godot_version`、`get_project_info`、`run_project`、`get_debug_output`、`create_scene` / `add_node` / `save_scene`。
  做不到時改用 CLI／檔案，並在 `pipeline/log.md` 的「MCP 問題」寫原因（本版 godot-mcp 無寫腳本／設節點屬性／截圖工具）。
- Blender 的 `execute_blender_code` **不設自動允許**（要在 Blender 裡跑任意 Python）；Godot 工具與檢查工具可自動允許。
- 啟動指令見 `pipeline/log.md`「MCP 設定」。

## 8. 常用指令

```bash
make env          # 環境檢查（含 Blender MCP 9876 埠、Godot MCP 可用性）
make check ID=x   # 資產規格檢查（Blender 端 + Godot 匯入後）
make import       # 讓 Godot 重新匯入全部資產
make layout       # 產生 + 驗證版面（寫 godot/data/layout.json，14 項檢查）
make world-check  # 世界組裝報表（WORLD_BUILD：件數/碰撞/缺資產）
make cam-check    # 鏡頭幾何檢查（CAM_REPORT：pitch/方位/翻滾/鏡頭相對輸入）
make bot          # bot 測試
make physics      # 物理測試（門檻在 specs/physics_thresholds.json）
make shot         # 遊戲截圖
make web          # 匯出 WebGL 版到 web/（Web 用 gl_compatibility，見 project.godot）
make serve        # 用 HTTP 開遊戲（web 根＝仓库根，啟動就印出可用網址），PORT= / HOST= 可指定
make serve-https  # 自簽 HTTPS 版（要音效、或要官方殼層「非安全來源不啟動」的行為時用）
make web-smoke    # 無頭 Chrome 驗證 web/：渲染 + 引擎啟動 +（--bot）bot 通關；--url 可指任意網址
```

## 9. 進度紀律

- 每完成一個步驟就更新 `pipeline/progress.md`；FAIL 用 `[x]`，通過用 `[v]`。
- 每個 Phase 結束 `git commit`（commit 主題格式：`type: summary`，AI 協作加 `Co-authored-by` trailer）。
- 看到 **STOP**：講清楚要使用者做什麼，結束這一輪。
