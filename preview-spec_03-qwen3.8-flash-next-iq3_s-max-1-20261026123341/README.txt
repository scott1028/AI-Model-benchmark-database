Coin Run (Web)

How to play locally
  The game must be served over HTTP; opening index.html directly (file://) does not work.
  1. cd into this folder
  2. python3 -m http.server 8061
  3. Open http://127.0.0.1:8061/ in Chrome, Edge or Firefox (WebGL 2 required)

Upload anywhere that serves static files (itch.io HTML5, GitHub Pages, any web server).
This build has threads disabled, so no special COOP/COEP headers are needed.

Controls
  Arrow keys        move (relative to the camera)
  A / gamepad X     boost (hold)
  B / gamepad B     brake (hold)
  Space / Enter / gamepad A   jump

Goal: collect the 3 coins, then touch the red flag in front of the temple.
