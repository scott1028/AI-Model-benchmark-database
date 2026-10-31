import sys, threading, http.server, socketserver

PORT = 8141
ROOT = '/home/scott/workspace/AI-Model-benchmark-database'
FOLDER = sys.argv[1] if len(sys.argv) > 1 else 'preview-wip'
URL = f'http://127.0.0.1:{PORT}/{FOLDER}/index.html'


class H(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *a, **kw):
        super().__init__(*a, directory=ROOT, **kw)

    def log_message(self, *a):
        pass


socketserver.TCPServer.allow_reuse_address = True
httpd = socketserver.TCPServer(('127.0.0.1', PORT), H)
threading.Thread(target=httpd.serve_forever, daemon=True).start()

from playwright.sync_api import sync_playwright

errors = []
with sync_playwright() as pw:
    browser = pw.chromium.launch(channel='chrome', args=['--no-sandbox'])
    page = browser.new_page(viewport={'width': 1280, 'height': 1000})
    page.on('console', lambda m: errors.append(f'{m.type}: {m.text}')
            if m.type == 'error' else None)
    page.on('pageerror', lambda e: errors.append('pageerror: ' + str(e)))
    page.goto(URL)
    page.wait_for_timeout(900)

    print('final_url:', page.url)
    print('title:', page.title())
    print('app_started:', page.evaluate("!!(window.kminesApp && window.kminesApp.renderer)"))
    print('gl2_active:', page.evaluate(
        "window.kminesApp ? window.kminesApp.renderer.gl instanceof WebGL2RenderingContext : false"))

    box = page.locator('#board').bounding_box()
    page.mouse.click(box['x'] + box['width'] / 2, box['y'] + box['height'] / 2)
    page.wait_for_timeout(250)
    page.mouse.click(box['x'] + box['width'] * 0.95, box['y'] + 10, button='right')
    page.wait_for_timeout(250)

    print('hud_time:', page.evaluate("document.getElementById('timeLabel').textContent"))
    print('hud_mines:', page.evaluate("document.getElementById('mineLabel').textContent"))

    sample = page.evaluate("""() => {
      const c = document.getElementById('board');
      const t = document.createElement('canvas');
      t.width = 240; t.height = 140;
      const g = t.getContext('2d');
      g.drawImage(c, 0, 0, 240, 140);
      const d = g.getImageData(0, 0, 240, 140).data;
      const seen = new Set();
      for (let i = 0; i < d.length; i += 16)
        seen.add((d[i] >> 3) + ',' + (d[i+1] >> 3) + ',' + (d[i+2] >> 3));
      return seen.size;
    }""")
    print('distinct_colors:', sample)

    shot = f'/home/scott/workspace/AI-Model-benchmark-database/{FOLDER}/tmp/verify_screenshot.png'
    page.screenshot(path=shot)
    browser.close()

print('console_errors:', errors if errors else 'NONE')
sys.exit(0 if (not errors and sample > 8) else 1)
