# Blender × Godot MCP 遊戲 Pipeline — 箱庭版 Playbook v2.2

> v2.2 · 2026-10-28 · 給 coding agent 從空資料夾自動跑完
> v2.1 → v2.2：改成無人值守、交付 Web（WebGL）build、Xbox 手把方向控制、開始與結束引導
> 本檔只描述**工作目標與驗收方式**。所有實作細節（程式、設定檔、規格數值、座標、參數、工具設計、驗收門檻）都由執行的 model 自行推論與決定。
> 執行中**不問使用者、不等回覆**：不確定的事自己推論意圖，記下理由後繼續（見工作原則 9）。

---

## 給人看的：怎麼開始

這份 playbook 設計成無人值守：你啟動一次，agent 自己補環境、接 MCP、建模、組遊戲、寫報告，中間不會停下來問你。

```text
runner ──啟動──► agent 跑一輪 ──結束前寫──► pipeline/progress.md
runner ◄──讀 STATUS── pipeline/progress.md：還不是 DONE / BLOCKED 就再啟動一輪
```

`pipeline/progress.md` 像交班簿：每輪 agent 結束前寫好做到哪，下一輪照著接手。agent 只在需要重啟才能生效時（例如剛裝好 Godot、寫好它的路徑）或 context 快不夠時結束一輪。

1. 建一個空資料夾，把本檔放到 `.spec/PLAYBOOK_v2.2.md`，`PLAYBOOK_BLENDER_GODOT_MCP_SETUP.md` 也放進 `.spec/`。資料夾裡不要放其他版本的 playbook 或前一次的成品
2. 先裝好 agent-switch 與 opencode，runner 靠它們啟動 agent。Phase 0 列的軟體也建議先裝好；沒裝的話 agent 會試著自己裝
3. 在該資料夾跑 runner。第一次跑會建立工作資料夾 `preview-wip/` 並複製一份 `.spec/` 進去；之後每輪都在這個資料夾裡啟動 agent，本輪所有產出都放在裡面，最後由 agent 依 Phase 10 的命名規則改名。它每輪用 agent-switch 啟動 opencode 的非互動模式（`run`），掛上 Godot MCP 與 Blender MCP；`--yolo` 讓所有工具不用詢問就執行（agent-switch 會轉成 opencode 的 `--auto`）。Godot 路徑先讀 agent 寫的 `pipeline/godot-path`，沒有就用 PATH 上的 `godot`：

```bash
ls -d preview-*/ >/dev/null 2>&1 || { mkdir preview-wip && cp -r .spec preview-wip/; }
for i in $(seq 1 30); do
  P="$(ls -d preview-*/ | head -n1)"
  grep -qE '^STATUS: (DONE|BLOCKED)' "${P}pipeline/progress.md" 2>/dev/null && break
  GODOT_PATH="$(cat "${P}pipeline/godot-path" 2>/dev/null || command -v godot)"
  (cd "$P" && agent-switch opencode --yolo \
    --mcp-stdio "godot:npx -y @coding-solo/godot-mcp" \
    --mcp-env "godot:GODOT_PATH=$GODOT_PATH" \
    --mcp-stdio "blender:uvx blender-mcp" \
    --mcp-env "blender:DISABLE_TELEMETRY=true" \
    -- run "讀 .spec/PLAYBOOK_v2.2.md；有 pipeline/progress.md 就一起讀，從未完成的步驟繼續，沒有就從頭開始。依工作原則 9 自動執行，不要問我。")
done
```

要接本地模型時，在 `--yolo` 後面加 `--url` / `--model`（見 `.spec/PLAYBOOK_BLENDER_GODOT_MCP_SETUP.md` 第 3 節）。

4. 跑完看 `preview-*/pipeline/REPORT.md`：
   - `STATUS: DONE`：全部 Phase 都跑過了，沒通過的 DoD 項目會標 `[x]` 並寫原因。成品就是整個 `preview-*/` 資料夾，入口是裡面的 `index.html`：在 `<project>/` 這層啟動 HTTP server，開 `http://<host>/preview-.../index.html`（不能直接雙擊），細節寫在 REPORT
   - `STATUS: BLOCKED`：缺了 agent 自己補不了的東西，REPORT 會寫你要做什麼。做完後把 `preview-*/pipeline/progress.md` 第一行改回 `STATUS: RUNNING`，再跑一次 runner 就會續跑

兩件事一定要記得：
- 權限全開代表 Blender MCP 的 `execute_blender_code` 可以不經核准執行任意 Python。只在可拋棄的環境跑（VM、容器或專用帳號）
- `30` 是最多跑幾輪的保險，避免無限重啟；到上限還沒 DONE，看 `preview-*/pipeline/progress.md` 和 `preview-*/pipeline/log.md` 找原因

---

## 要做什麼

一個 3D 箱庭小遊戲「Coin Run」：玩家操作一台單輪小機器人，在浮空的箱庭地塊上收集 3 枚金幣，收齊後碰到終點旗就勝利。

- **必需**重新做一個新的, 不准參考先前任何其他成品

- 所有 3D 模型（含地面）都由 Blender MCP 產生；Godot MCP 負責組裝與執行
- 模型不符合規格時，要能自動發現並回到 Blender 修正
- 最後交付的是整個工作資料夾（含 Godot 匯出的 Web（WebGL）build），依 Phase 10 的命名規則改名成 `preview-.../`；用瀏覽器開裡面的 `index.html` 就能玩

### 使用者需求（已定案，執行時不要再問）

| 項目 | 需求 |
|---|---|
| 地圖 | 40×40 m 的可活動區域，四周有磚瓦牆，角色走不出去；地塊浮在天空中 |
| 風格 | 箱庭（桌上模型）風格，構圖豐富、畫面不單調；低模、平塗顏色 |
| 角色 | 小機器人，單輪；輪子要有轉動動畫，動畫在 Blender 製作 |
| 場景元素 | 樹、草叢、石頭、磚瓦牆、房子、古代奇幻風格建築、木箱、木桶 |
| 木箱 | 造型要和場景風格搭配，擺放位置要合理 |
| 地面 | 不能單調、看起來不能假：要有草坪、道路等；地面稍微有高低起伏 |
| 鏡頭 | 等角 45° 平滑跟隨；方向鍵與手把的方向輸入都相對鏡頭方向移動 |
| 按鍵 | 支援鍵盤與 Xbox 手把。移動：方向鍵 / 手把左搖桿或十字鍵（左搖桿是類比輸入，推的幅度決定力道，推到底是一般行走速度）；加速：鍵盤 A / 手把 X（按住）；煞車：鍵盤 B / 手把 B（按住）；跳躍：Space / Enter / 手把 A |
| 開始引導 | 遊戲一開始顯示引導畫面：遊戲名稱、目標（收集 3 枚金幣後碰終點旗）、鍵盤與 Xbox 手把的操作說明；按跳躍鍵（Space / Enter / 手把 A）開始玩 |
| 結束引導 | 抵達終點後顯示勝利畫面與重新玩的提示；按跳躍鍵重新玩，整個關卡回到初始狀態（金幣、機器人位置、被推動的木桶） |
| 移動 | 一般行走速度 4 m/s；移動要靠力向量與摩擦力，有慣性 |
| 物理互動 | 加速時機身傾斜；木桶可以推動；撞到樹會搖晃並掉落葉；穿過草叢時草叢會晃 |
| 氣氛 | 陽光與陰影（暖色、柔和陰影、環境光遮蔽）；雲在空中漂移；樹和草叢隨微風擺動；移軸景深（迷你模型感） |

### 完成定義（DoD）

- [ ] 每個 3D 資產都通過自動規格檢查（Blender 端與 Godot 匯入後都符合規格）
- [ ] 自動 bot 測試通過：bot 從起點依序收集 3 枚金幣並抵達終點，輸出能判讀的進度與勝利訊息，且沒有腳本錯誤
- [ ] 自動物理測試通過：「移動」與「物理互動」兩列需求的每一項都有可量測的檢查
- [ ] 透過 Godot MCP 實際執行遊戲沒有錯誤；遊戲畫面截圖依附錄 B 檢查過
- [ ] 完成一次迴圈演練（Phase 9）
- [ ] Web build（Phase 10）：從入口 `index.html` 能導向 Godot 匯出的 WebGL build，在瀏覽器裡載入沒有錯誤，bot 模式能收齊金幣並抵達終點
- [ ] `pipeline/REPORT.md` 已寫

跑完所有 Phase 後照實勾選：通過標 `[v]`，做了沒過標 `[x]` 並寫原因。某一項沒過不是停下來的理由，繼續做完其他 Phase。

---

## 工作原則

1. **資產規格檔是唯一真相**：每個資產都要先寫好規格（外觀描述、目標尺寸、面數上限、是否需要碰撞、需要哪些動畫），建模與檢查都以它為準。規格格式由你設計。
2. **自動檢查迴圈**：每次從 Blender 匯出後，都要匯入 Godot 並和規格自動比對。尺寸在 Blender 端與 Godot 匯入後都要在規格的 ±10%（至少 2 cm）內；另外檢查原點、面數、碰撞、動畫。不通過時要給出具體的修正建議，並分辨是建模問題還是 pipeline 問題。
3. **FAIL 處理**：記進 log（時間、資產、第幾次、檢查結果、判斷、對 Blender 的修正請求、結果）→ 回 Blender 修。同一資產每輪最多 3 次，超過就先判斷原因：
   - 規格本身訂錯 → 改規格，log 寫理由
   - 建模做不到 → 換更簡單的造型或結構（仍要符合使用者需求），重新開始一輪
   - 第二輪仍不過 → 保留最接近規格的版本，progress 標 `[x]`、log 寫原因，繼續下一步，最後列進 REPORT

   不可以只為了通過而放寬規格或檢查。
4. **不要手動修改匯出的模型檔或 Godot 的匯入設定檔**；要改就回 Blender 重新匯出。
5. **工具優先順序**：建模用 Blender MCP；Godot 端優先用 Godot MCP，做不到時改用其他方式，並把原因記進 log 的「MCP 問題」。MCP 連不上時的處理見 Phase 2。
6. **進度與紀錄**：
   - 本輪所有產出都放在工作資料夾（runner 啟動你的位置，`preview-*/`）；本檔的路徑都相對於它。不要在上層資料夾新增檔案
   - `pipeline/progress.md`：第一行是 `STATUS: RUNNING`、`STATUS: DONE` 或 `STATUS: BLOCKED`，runner 靠它決定要不要再跑一輪。下面是本檔各 Phase 的步驟清單，完成就打勾（`[v]` 通過、`[x]` 做了沒過、`[ ]` 未做）。每做完一個步驟就更新，重啟後靠它續跑，不重做已完成的步驟
   - `pipeline/log.md`：設計決策、每次檢查與修正、MCP 問題
   - 過程中可以 `git commit` 當檢查點（例如每個 Phase 完成、結束本輪前）；Phase 11 再把本次執行產生的所有 commit squash 成一個，不動其他歷史。git 沒設定使用者時，只設定這個 repo 的身分，不改全域設定
7. **由你決定的事**：所有數值、座標、程式、設定檔、工具設計、測試門檻都由你推論。重要的決策在 log 的「設計決策」段落寫一兩行理由。
8. **衝突時的優先順序**：DoD > 使用者需求 > 其他美術判斷。
9. **自動執行規則**（取代人工確認）：
   - 不問使用者、不等回覆。不確定的事自己推論：選最符合 DoD 與使用者需求的做法，在 log「設計決策」寫一兩行理由，然後繼續
   - 原本要人做的事都由你做：安裝缺少的軟體、在 Blender 裝好並啟動 MCP server、需要時結束本輪讓 runner 重啟
   - 指令失敗或工具呼叫被拒：換方法再試，記進 log。任何修正迴圈（資產 FAIL、測試修復、截圖重拍）都要有次數上限，沒另外規定就是 3 次，超過就記錄並往下做
   - **結束本輪**：只在需要重啟 agent 才能生效時（例如 `pipeline/godot-path` 剛寫好，這一輪的 Godot MCP 還找不到 Godot），或 context 快不夠時。先更新 progress.md（寫原因與下一步），再正常結束；runner 會重啟並續跑
   - **BLOCKED**：只在沒有任何方法能繼續時（例如缺 Blender 又裝不起來）。progress.md 第一行改成 `STATUS: BLOCKED` 並記下時間，在 `pipeline/REPORT.md` 寫卡在哪、試過什麼、人要做什麼，然後結束。恢復後的第一輪先在 progress.md 記下恢復時間；兩者之間不算進耗時（Phase 10）
   - 做完 Phase 11 才把第一行改成 `STATUS: DONE`

---

## Phase 0：環境檢查（agent 自動處理）

1. 建立 `pipeline/progress.md`（第一行 `STATUS: RUNNING`，並記下開始時間，Phase 10 算耗時用）與 `pipeline/log.md`
2. 確認已安裝：Godot ≥ 4.3（含同版本的 Web 匯出模板）、Blender、Node.js ≥ 18、uv、git、能跑 WebGL 2 的無頭瀏覽器，並找到 Godot 執行檔的位置（PATH 和常見安裝位置都要找）。也確認有可用的顯示環境給 Blender 與截圖用，沒有就準備虛擬顯示（例如 Xvfb）
3. 缺少的軟體自己裝：只從官方來源下載，裝在使用者空間（不用 sudo、不改系統設定），安裝位置與版本記進 log
4. 把 Godot 執行檔的絕對路徑寫進 `pipeline/godot-path`，runner 下一輪會用它設定 Godot MCP 的 `GODOT_PATH`
5. 任一項怎樣都裝不起來 → BLOCKED（工作原則 9），REPORT 列出缺什麼、去哪裡下載

## Phase 1：專案骨架

1. 規劃專案結構：資產規格、pipeline 紀錄、自動化工具、Blender 原始檔、Godot 專案要分開放
2. 寫一份專案規則檔 `AGENTS.md` 給 agent 讀（opencode 會讀它），至少涵蓋：
   - 自動執行規則（工作原則 9 的重點），讓每一輪的 agent 照同一套規則跑
   - 單位與座標約定（規格以哪個軟體的軸向為準、兩邊如何換算、角色正面朝向）
   - Blender 建模與匯出規則（每個資產怎麼組織、原點、材質限制、碰撞怎麼標示、`execute_blender_code` 每次只做一小步、不刪專案外檔案、不下載外部 3D 資產如模型、材質、貼圖；軟體與 MCP addon 可以從官方來源安裝）
   - 每次匯出後必做的檢查，以及 FAIL 迴圈規則
   - Godot 端的檔案位置、輸入規則（只用使用者定案的按鍵）、MCP 使用規則
3. MCP 伺服器（Godot MCP、Blender MCP）與權限由 runner 每輪掛上，專案裡不用另外設定；掛載細節見 `.spec/PLAYBOOK_BLENDER_GODOT_MCP_SETUP.md`。`--yolo` 讓 `execute_blender_code` 也會被自動允許，所以在規則檔限制它的用途（只在 Blender 裡建模與匯出、不碰專案外檔案、不連外網）
4. 建立 Godot 專案、Web 匯出設定與使用者定案的按鍵設定，並用程式確認按鍵真的有綁上。Web 匯出只支援 Compatibility renderer，所以專案從一開始就用它，之後的檢查與截圖才會和成品一致
5. 建立自動化工具（實作方式自訂），至少要能：檢查環境（含 Blender MCP 埠能否連線）、在背景啟動 Blender 並開好 MCP server、讓 Godot 匯入資產、依工作原則第 2 點比對資產規格、跑 bot 測試、跑物理測試、匯出 Web build 並從入口 `index.html` 用瀏覽器驗證
6. 在工作資料夾自己 `git init`（上層資料夾已經是 git repo 也一樣），做第一次 Godot 匯入

## Phase 2：連線 MCP（agent 自動完成）

原本要人在 Blender 裡點的步驟，都改用腳本做。步驟可參考 `.spec/PLAYBOOK_BLENDER_GODOT_MCP_SETUP.md` 第 2 節，但不要依賴 UI 操作。

1. 用 Blender 命令列或啟動腳本安裝並啟用 Blender MCP addon；外部資產庫與生成器保持關閉
2. Blender 5.x 有內建的 Blender Lab「MCP」擴充時，用腳本停用它，否則它會佔用同一個埠
3. 用 Phase 1 的自動化工具在背景啟動 Blender，並自動啟動 MCP server。確認埠是 Blender 開的，而且 MCP 握手成功（只測 TCP 會誤判）。結束本輪時不用關 Blender；之後每輪開始先確認它還在，不在就重新啟動
4. 驗證：環境檢查通過，且 Godot MCP 與 Blender MCP 各成功呼叫一個唯讀工具
   - 這一輪看不到 MCP 工具，或 Godot MCP 找不到 Godot 而 `pipeline/godot-path` 是這一輪才寫的 → 依工作原則 9 結束本輪，下一輪再驗證
   - 有載入但呼叫失敗 → 自己診斷修復（埠、addon、server 狀態），最多 3 次
   - 重啟 2 輪後仍不能用（次數記在 progress.md）→ 改用替代方式繼續（Godot 命令列；直接連 Blender addon 的埠，或用 Blender 命令列執行 Python），記進 log「MCP 問題」，DoD 與 REPORT 照實標記

## Phase 3：冒煙測試

挑一個最簡單的資產，完整走一次：寫規格 → Blender 建模 → 自檢外觀 → 匯出 → Godot 匯入 → 規格檢查 PASS → 放進一個測試場景 → 用 Godot MCP 執行沒有錯誤 → 匯出 Web build，在瀏覽器裡載入沒有錯誤。

## Phase 4：資產

依附錄 A 為每個資產寫規格並建模，每個都走完自檢與自動檢查迴圈。先做角色與道具，再做建築；地面另外在 Phase 5 做。

## Phase 5：地面

依附錄 C 建立整個箱庭地塊（也是 Blender 資產，同樣要通過規格檢查）。其他物件要能正確放在起伏的地面上。

## Phase 6：世界組裝

依附錄 D 在 Godot 組出整個箱庭：地面、外牆、所有場景元素、金幣、終點。版面要可以重現；之後要調整版面時，不需要逐一手動搬動上百個物件。

## Phase 7：遊戲邏輯、物理與氣氛

實作玩法（開始引導、收集、勝利、結束引導與重新玩）、角色操控與移動、物理互動、鏡頭、氣氛效果，全部依「使用者需求」表。bot 模式：用啟動參數切換，忽略玩家輸入，跳過開始引導，自動依序走向 3 枚金幣與終點；勝利後觸發一次重新玩，確認關卡回到初始狀態。Compatibility renderer 不支援的效果（例如環境光遮蔽）要用替代做法做出接近的感覺（例如在 Blender 把 AO 烘進頂點色），log 寫原因。

## Phase 8：測試與畫面檢查

1. bot 測試通過
2. 物理測試通過：「移動」與「物理互動」兩列需求的每一項，都要有你訂的可量測門檻；門檻和理由寫進 log
3. 用 Godot MCP 執行遊戲，沒有錯誤
4. 取得遊戲畫面截圖：包含一般遊玩畫面，以及看得到整個地塊的畫面。依附錄 B 檢查，不合格就判斷是資產問題（回 Blender）還是場景問題（修 Godot），修完重拍；最多 3 輪（工作原則 9）

## Phase 9：迴圈演練

模擬「Godot 端發現資產需要修改」：
1. 在 log 寫下回饋：金幣太大、擋視線，規格改小（約原本的 60%）
2. 修改金幣規格，執行自動檢查 → 應該 FAIL（確認檢查機制有效），並記錄。沒有 FAIL 代表檢查工具有問題，先修工具再重測
3. 依修正建議回 Blender 調整 → 匯出 → 檢查 PASS
4. 同步調整遊戲端與金幣尺寸相關的設定；bot 測試與物理測試仍通過
5. 重新執行遊戲確認畫面

## Phase 10：Web 交付

1. 用 Godot 把最終版本匯出成 Web（WebGL）build 到 `build/web/`；匯出要能用自動化工具一個指令重做
2. 在工作資料夾根目錄建立入口 `index.html`，把使用者導向 `build/web/index.html`。使用者會在上層資料夾（`<project>/`）啟動 HTTP server，最終網址像 `http://<host>/preview-04-claude-opus-5.5-xhigh-187-20261028143005/index.html`；所以要用相對路徑，資料夾改名後也要導得到，需要時調整 base URL
3. 照使用者的方式驗證：在上層資料夾啟動本機 HTTP server，在無頭瀏覽器開 `http://<host>/<工作資料夾名稱>/index.html`（改名前是 `preview-wip`）：要被導向 Web build、console 沒有錯誤，截圖依附錄 B 檢查
4. 在 Web build 裡跑一次 bot 模式（啟動參數怎麼傳由你決定），要能收齊 3 枚金幣並抵達終點
5. 資料夾命名規則（Phase 11 改名時套用）：`preview-${spec_num}-${MODEL_NAME}-${TIME_COST_MINUTES}-${FILE_CREATED_AT_YYYYMMDDHHMMSS}/`
   - `spec_num`：`04`
   - `MODEL_NAME`：執行本次任務的 model 名稱與版本，版本號用點分隔（例如 `claude-opus-5.5`）；有設定 reasoning effort 時接在後面（例如 `-xhigh`），沒有就省略。去掉 provider 前綴（例如 `anthropic/`），`[A-Za-z0-9._-]` 以外的字元換成 `-`
   - `TIME_COST_MINUTES`：從 Phase 0 開始到改名當下的經過時間，扣掉 BLOCKED 等人處理的時間（工作原則 9）；單位分鐘，無條件進位成整數
   - `FILE_CREATED_AT_YYYYMMDDHHMMSS`：改名當下的本機時間
   - 範例：`preview-04-claude-opus-5.5-xhigh-187-20261028143005/index.html`
6. `preview-${spec_num}-${MODEL_NAME}-${TIME_COST_MINUTES}-${FILE_CREATED_AT_YYYYMMDDHHMMSS}/` 都要 commit 進版本控制

## Phase 11：收尾

1. 撰寫 `pipeline/REPORT.md`：DoD 勾選結果、每個資產的迭代次數、MCP 問題與替代做法、物理測試的門檻與實測數字、Web build 的位置與開啟方式、Web 版改用替代做法的效果、主要設計決策與理由、遇到的問題與解法、花最多時間的步驟、建議的下一步
2. 依 Phase 10 的命名規則把工作資料夾改名（例如 `preview-wip/` → `preview-04-...`），在 progress.md 打勾
3. 把 `pipeline/progress.md` 第一行改成 `STATUS: DONE`，commit 後把本次執行產生的所有 commit squash 成一個
4. 輸出 REPORT 摘要，包含資料夾名稱與沒通過的 DoD 項目

---

## 附錄 A：資產目標

尺寸、面數、顏色、結構、命名都由你訂；比例要合理，風格要一致。

| 資產 | 要表達什麼 | 遊戲上的要求 |
|---|---|---|
| 機器人 | 可愛的單輪小機器人 | 輪子要能在遊戲中轉動，且看得出在轉 |
| 金幣 | 金幣 | 可以被收集 |
| 終點旗 | 旗桿加旗子 | 碰到就判定抵達終點 |
| 木箱 | 和場景風格搭配的木箱 | 擺放位置合理 |
| 木桶 | 木桶 | 可以被機器人推動 |
| 樹 | 低模的樹 | 被撞會搖晃、掉落葉；平常隨風擺動 |
| 草叢 | 一叢灌木 | 機器人可以穿過，穿過時會晃；平常隨風擺動 |
| 石頭 | 石頭 | — |
| 磚瓦牆 | 磚瓦牆 | 圍住整個地圖，角色出不去 |
| 房子 | 小屋 | — |
| 古代奇幻風格建築 | 數量與種類由你決定 | — |
| 雲 | 低模雲朵 | 在空中漂移 |
| 地面 | 整個箱庭地塊 | 見附錄 C |

哪些物件會擋住機器人、哪些可以穿過，依遊戲合理性決定（草叢必須可以穿過）。

## 附錄 B：畫面檢查清單

- 一般遊玩畫面：機器人清楚可辨；看得到周圍的場景；陰影柔和；有移軸景深的迷你模型感
- 整體畫面：整個地塊都在畫面內；構圖豐富、沒有大片空白；所有場景元素都看得到；雲在空中，地上看得到雲的影子
- 地面：草坪不單調、不假；道路清楚；看得出高低起伏
- 沒有倒立、躺平、懸空、陷進地面的物件；高大的物件不會擋住玩家常走的區域
- 木箱的造型與擺放和場景搭配

## 附錄 C：地面目標

- 箱庭地塊的樣子：地塊有厚度、側面看得到土層，像放在桌上的模型
- 地表稍微有高低起伏；機器人爬得上去，bot 不會卡住
- 建築、圍牆等物件底下要平，物件放上去不懸空、不陷入
- 草坪有自然的深淺變化，不是單一顏色，也不雜亂
- 有道路連接主要區域
- 加上能讓地面更真實的點綴（種類由你決定）

## 附錄 D：版面目標

- 分區清楚、構圖豐富，同時留有可以跑動的空間
- 鏡頭是斜上方等角視角，高大的物件不要擋住玩家常走的區域
- 木箱、木桶要擺在合理的地方
- bot 的路線（起點 → 3 枚金幣 → 終點）不能被擋住
- 物理測試用到的物件，周圍要有空間讓測試能進行
