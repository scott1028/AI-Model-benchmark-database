# kmines 逆向 playbook

我要逆向 `/usr/games/kmines`。

1. 得到逆向後的原始碼。
2. 根據 #1,把它寫成 WebGL 版本。

## 約束

- 不可弄壞作業系統。
- 完成代表:兩項交付都在,入口 `index.html` 導得到移植版,移植版跑得起來,且工作資料夾已依命名規則改名。

## 檔案輸出規範

- 開始時在目前資料夾建立工作資料夾 `preview-wip/`,並記下開始時間(算 `TIME_COST_MINUTES` 用)。本次所有產出都放在裡面:
  - `report/`:逆向原始碼與報告
  - `webgl/`:WebGL 版,入口是 `webgl/index.html`
  - `index.html`:引導入口(見「引導入口 index.html」)
- 完成後把 `preview-wip/` 改名成 `preview-${spec_num}-${MODEL_NAME}-${TIME_COST_MINUTES}-${FILE_CREATED_AT_YYYYMMDDHHMMSS}/`:
  - `spec_num`:`spec_06`
  - `MODEL_NAME`:執行本次任務的 model 名稱與版本,版本號用點分隔(例如 `claude-opus-5.5`);有設定 reasoning effort 時接在後面(例如 `-xhigh`),沒有就省略。去掉 provider 前綴(例如 `anthropic/`),`[A-Za-z0-9._-]` 以外的字元換成 `-`
  - `TIME_COST_MINUTES`:從開始到改名當下的經過時間,單位分鐘,無條件進位成整數
  - `FILE_CREATED_AT_YYYYMMDDHHMMSS`:改名當下的本機時間
  - 範例:`preview-spec_06-claude-opus-5.5-xhigh-42-20261031153005/`

## 實作限制

- 必需重新做一個新的,不准參考先前任何其他成品(例如其他 `preview-*` 資料夾)。
- 逆向只能依據本機的 `/usr/games/kmines` 與它安裝在系統上的檔案;不能下載或參考 kmines 的上游原始碼、套件原始碼,或其他現成的踩地雷實作。
- 不下載外部素材(圖片、主題、音效)。
- 不修改、不刪除 `/usr/games/kmines` 與其他系統檔案;不用 sudo、不改系統設定。
- 缺少的工具只從官方來源下載,裝在使用者空間;安裝位置與版本寫進報告。
- 除了安裝工具,不在工作資料夾外新增檔案;暫存檔也放在工作資料夾裡。

## 引導入口 index.html

- 工作資料夾根目錄要有 `index.html`,把使用者導向 `webgl/index.html`。
- 使用者會在上層資料夾啟動 HTTP server,網址像 `http://<host>/preview-spec_06-claude-opus-5.5-xhigh-42-20261031153005/index.html`(不能直接雙擊開檔)。所以要用相對路徑,資料夾改名後也要導得到。
- `index.html` 要有 `<title>`;GitHub Pages 列表頁會讀它當標題。
- 照使用者的方式驗證:在上層資料夾啟動本機 HTTP server,用無頭瀏覽器開 `http://<host>/<工作資料夾名稱>/index.html`(改名前是 `preview-wip`),要被導向 WebGL 版、console 沒有錯誤、畫面正常。

## 給 model 的提示詞(直接貼)

```text
我要逆向 /usr/games/kmines。
1. 得到逆向後的原始碼。
2. 根據 #1,把它寫成 WebGL 版本。

約束:
- 不可弄壞作業系統:不修改、不刪除 /usr/games/kmines 與其他系統檔案;不用 sudo、不改系統設定;缺的工具只從官方來源裝在使用者空間。
- 重新做一個新的:逆向只能依據本機的 kmines 與它安裝在系統上的檔案;不准參考其他 preview-* 成品、kmines 上游或套件原始碼、其他現成的踩地雷實作;不下載外部素材。
- 開始時建立 preview-wip/ 並記下開始時間;所有產出與暫存檔都放在裡面:report/(逆向原始碼與報告)、webgl/(WebGL 版)、index.html(引導入口)。
- index.html 用相對路徑導向 webgl/index.html,要有 <title>。在上層資料夾啟動 HTTP server,用無頭瀏覽器開 http://<host>/preview-wip/index.html:要導得到 WebGL 版,console 沒有錯誤。
- 完成後把 preview-wip/ 改名成 preview-spec_06-${MODEL_NAME}-${TIME_COST_MINUTES}-${YYYYMMDDHHMMSS}/
  - MODEL_NAME:你的 model 名稱與版本,版本號用點分隔;有 reasoning effort 就接在後面;去掉 provider 前綴;[A-Za-z0-9._-] 以外的字元換成 -
  - TIME_COST_MINUTES:開始到改名當下的分鐘數,無條件進位
  - YYYYMMDDHHMMSS:改名當下的本機時間
```

做法不在此提供 — 那正是要測的部分。
