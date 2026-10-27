GODOT ?= /home/scott/tools/bin/godot
PY ?= python3

.PHONY: env verify-input import check layout world-check cam-check bot physics shot web serve serve-https web-smoke

env:
	$(PY) tools/env_check.py

verify-input:
	$(GODOT) --headless --path godot --script res://tools/verify_input.gd

import:
	$(GODOT) --headless --path godot --import

check:
	@test -n "$(ID)" || { echo "usage: make check ID=<id> [ATTEMPT=n]"; exit 2; }
	$(PY) tools/check_asset.py --id $(ID) --attempt $(if $(ATTEMPT),$(ATTEMPT),1)

layout:
	$(PY) tools/gen_layout.py --write
	$(PY) tools/gen_terrain_texture.py

world-check:
	$(GODOT) --headless --path godot --scene res://scenes/main.tscn --quit-after 200 -- --world-check

cam-check:
	$(GODOT) --headless --path godot --scene res://scenes/main.tscn --quit-after 300 -- --cam-check

bot:
	$(PY) tools/bot_test.py

physics:
	$(PY) tools/physics_test.py

shot:
	$(PY) tools/capture.py $(if $(SCENE),--scene $(SCENE),) $(if $(PRESET),--preset $(PRESET),) $(if $(OUT),--out $(OUT),)

web:
	@mkdir -p web
	$(GODOT) --headless --path godot --export-release "Web" "$(CURDIR)/web/index.html"

serve:
	$(PY) tools/serve.py $(if $(PORT),--port $(PORT),) $(if $(HOST),--host $(HOST),) $(if $(DIR),--dir $(DIR),)

serve-https:
	$(PY) tools/serve.py --https $(if $(PORT),--port $(PORT),) $(if $(HOST),--host $(HOST),) $(if $(DIR),--dir $(DIR),)

web-smoke:
	node tools/web_smoke.mjs
