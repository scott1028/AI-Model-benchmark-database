# Pipeline log

## MCP 問題

（MCP 工具失敗與替代做法記錄在這裡）

- 2026-10-26 Phase 1：本環境為 opencode + agent-switch，不使用 `.mcp.json` / `.claude/settings.json`（照抄留檔）；規則檔採 `AGENTS.md`（同 CLAUDE.md 內容），依 .spec/PLAYBOOK_MCP_SETUP.md §5。
- 2026-10-26 Phase 1.10：外層 benchmark repo 已 git init，不另建巢狀 repo，phase commit 直接進外層 repo。
- 2026-10-26 Phase 2：人工安裝/啟動已在會話外完成（agent-switch 掛好 godot/blender MCP、Blender server 已在 9876），故跳過 STOP 直接做 agent 端驗證。
- 2026-10-26 Phase 3.6 godot-mcp `create_scene` 不接受 rootNodeName（root 名稱固定 "root"）→ 直接編輯 .tscn 重新命名並加 ext_resource/instance（附錄 C-5 預期內）。
- 2026-10-26 godot-mcp `add_node`：scenePath 帶 `res://` 前綴會報 "Scene file does not exist"，改帶**不帶 res:// 的相對路徑**（如 `scenes/tree.tscn`）即成功。complex 屬性（shape/mesh/resource/script/glb instance）仍走直接編輯 .tscn（附錄 C-5）。
- 2026-10-26 Phase 3.7 godot-mcp `get_debug_output` 在 run_project 後立即呼叫會回空 → 等 5 秒再讀。`save_scene` 對尚未開啟的場景报 "Scene file does not exist"；create_scene 本身已存檔，不需再 save。
- 2026-10-26 Blender `look`：target 要用物件名（如 crate_core-col），collection 名稱不行。view 參數需 3 個數字的視向向量；距離/framing 對薄物件會過近，改「solo 該 collection + view [1,-1,0.4] + distance」方式逐資產自檢。
- 2026-10-26 Blender 5.x：Action 無 `fcurves`（layered action API）；改 `preferences.edit.keyframe_new_interpolation_type='LINEAR'` 後重插 keyframe 達成線性。
- 2026-10-26 Phase 8 直接編輯 .tscn 大坑：`instance=ExtResource(...)` 節點的 `transform=` 寫在 **node header 括号內會被 Godot 靜默忽略**（執行時 transform 全為 identity）→ 必須寫成下一行 `transform = Transform3D(...)` 作為 override。build_world.mjs 已改正；Phase 7 的 route/flat 檢查是文字解析，抓不到這個錯。
- 2026-10-26 Phase 8 直接編輯 .tscn：Curve 的 `_data` 中 tangent/interpolation 欄位必須是 **INT**（`0`），寫 `0.0` 會報 `p_input[i + 3].get_type() != Variant::INT`。
- 2026-10-26 Phase 8 Godot 4.7 GDScript：DOF 屬性為 `dof_blur_near_distance/near_transition/far_distance/far_transition/dof_blur_amount`（**沒有** `dof_blur_far_amount`）；`Particles3D` 基底型別不可用於註解型別（用 Node + `get_class()`）；CPUParticles3D 無 `process_material`/`material`（用 `direction/spread/initial_velocity_*` + `material_override`）；emission shape 列舉在 `ParticleProcessMaterial`（非 GPUParticles3D）。
- 2026-10-26 Phase 8 clouds.gd：`@export var sun: NodePath` 型別 export 在 .tscn 未寫時不會自動解析 → `_ready()` 改 `get_node_or_null("../DirectionalLight3D")` fallback。
- 2026-10-26 Phase 9 直接編輯 .tscn：`Environment.sky` 必須是 **Sky 資源**（`sky_material` 指向 ProceduralSkyMaterial）；直接把 ProceduralSkyMaterial 塞給 `sky=` 會靜默失敗 → 背景全黑。
- 2026-10-26 Phase 9 GDScript 坑：`Input.get_vector(neg_x, pos_x, neg_y, pos_y)` — `ui_up` 要放第 **3** 參數（neg_y），放錯則前後鍵反向（physics_test 場景檢查才抓到，bot 模式走自己的導航不會暴露）。
- 2026-10-26 Phase 9 雲投影：DirectionalLight3D 光線方向 = **-basis.z**；用 +basis.z 且 `maxf(-dir.y, 0.001)` 兜底會讓 `_area_center` 算到天文數字、雲全飛出場景。
- 2026-10-26 Phase 11 補記：Godot 4 `Vector3.FORWARD` = (0,0,-1)；physics_test 曾以 +Z 初速配 FORWARD 方向，check 4 得到反向煞車值 1.873（僥倖 PASS）→ 改明寫 `Vector3(0,0,1)` 後實測 4.000。

## 資產迭代紀錄（附錄 B）

- 2026-10-26 Phase 10 起點：**Godot 回饋：coin 太大、擋視線 → 規格改為 [0.3, 0.3, 0.06]**

## 2026-10-26 12:36 crate iter 1
- check: PASS（Blender/Godot 尺寸 0.80×0.80×0.80、原點、面數 540、碰撞）
- 判斷: 建模過程內修 2 次（板子 rot_z 只轉自身→改三角函數定位；斜撐被板子蓋住→外移到板面外；均屬建模小步自檢，非 check FAIL）
- 對 Blender 的請求: 依附錄 D crate 規格建模
- 結果: PASS

## 2026-10-26 12:44 player iter 1
- check: PASS（0.70×1.20×0.50、原點、684 tris、wheel_spin）
- 判斷: 建模內修（fit 需用 Blender 軸向且先 bake tire 旋轉；Blender 5.x Action 無 fcurves→用 preferences LINEAR 預設重插 key）
- 對 Blender 的請求: 依附錄 D player 規格
- 結果: PASS

## 2026-10-26 12:52 coin/goal_flag/barrel/tree/bush/rock/cloud iter 1
- check: 7 資產全 PASS（coin 0.50×0.50×0.08 內凹面以厚度 0.094 補償；barrel 首版 416 tris 超 400 → 匯出前改鐵箍為無蓋圓筒 368）
- 判斷: pipeline 正常
- 結果: PASS


## 2026-10-26 12:41 wall_brick/house/mage_tower/temple_ruins iter 1
- check: 4 資產全 PASS（尺寸/原點/面數/碰撞）
- 判斷: pipeline 正常
- 結果: PASS

## 2026-10-26 12:42 terrain iter 1
- check: PASS（41.00×2.71×41.00、原點、29,988 tris、碰撞）
- 判斷: 建模內修 2 次（glb 內地表裝飾 z 要含 BASE=1.5；pebbles 密度 16/公尺→4/公尺），皆於匯出前 self-fix
- 對 Blender 的請求: 依附錄 E 生成高度場/地面網格/鋪面/花草/石子
- 結果: PASS

## 2026-10-26 13:05 Phase 8 bot_test iter 1-4
- iter 1: FAIL（3 errors）— clouds.gd sun NodePath 未解析（Nil.global_transform）、main.gd `dof_blur_far_amount` 不存在（4.7 改名）、tree.tscn Curve `_data` 浮點 tangent → 需 INT。逐一修正。
- iter 2: FAIL（25 errors）— tree.gd `extends StaticBody3D` 與 tree.tscn root（Node3D）不符；`Particles3D` 型別不可用；CPUParticles3D 無 `process_material`/`material` → 改 CPU 屬性 + `material_override`；emission shape 列舉在 ParticleProcessMaterial。
- iter 3: FAIL（0 errors，COIN 0/3）— 除錯 print 發現玩家卡在廣場中央：world.tscn 所有 `instance=` 節點的 transform 未生效（寫在 header 內被忽略）→ 建築/圍籬碰撞全部疊在 originals 原點。build_world.mjs 改 `transform =` 獨立行。
- iter 4: PASS（COIN 3/3、WIN、0 errors）
- 結果: PASS（Godot 端場景/腳本修正，未回 Blender）

## 2026-10-26 13:45 coin iter 2（迴圈演練）
- check: FAIL — X/Y 0.50 vs 0.30、Z 深 0.08 vs 0.06（超出容差）；原點/面數 PASS
- 判斷: Godot 回饋（coin 太大、擋視線）导致規格變更，屬資產問題 → 回 Blender
- 對 Blender 的請求: 以原點為基準縮放 coin 群組至 0.30×0.30×0.06（Blender X×0.6、Y×0.75、Z×0.6）後 apply scale
- 結果: PASS（iter 2：匯出尺寸 0.30×0.30×0.06、252 tris；main.tscn Coin SphereShape3D 0.5→0.3；bot_test/physics_test 仍 PASS；截圖重跑硬幣明顯變小）

## 2026-10-26 13:35 Phase 9 測試與截圖 iter 1-2
- physics_test iter 1: #9 FAIL — 玩家往反方向跑（`Input.get_vector` 參數順序錯）；修 player.gd 後 iter 2 全 PASS（1: 3.992 / 3: 0.496<0.678 / 5: 6.998 / 6: 煞車 23 格 vs 滑行 44 格 / 8: 搖曳 0.037 rad / 9: 木桶 0.526 m）。
- 截圖 iter 1（overview）: 背景全黑（Environment.sky 塞了 ProceduralSkyMaterial，需 Sky 資源）+ 雲看不見（clouds.gd 光線方向用 +basis.z，雲群中心算飛掉）→ 皆 Godot 場景問題。
- 截圖 iter 2: 背景淡藍、雲與雲影可見、遠半邊樹影、移軸模糊都到位；跟隨圖機器人正面可辨識。清單全 PASS。
- run_project debug: 僅 2 条 WARNING（basis shadowing / push 參數未用），已修，re-run 後錯誤與警告歸零。
- 結果: PASS（未回 Blender）
