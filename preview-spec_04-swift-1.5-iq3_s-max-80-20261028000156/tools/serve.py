#!/usr/bin/env python3
"""Serve the Coin Run WebGL build the way the deliverable is laid out on disk.

  python3 tools/serve.py [--dir ROOT] [--port 8000] [--host 0.0.0.0]

--dir defaults to the repository root (the parent of the deliverable folder), so the URL
http://<hostname>:<port>/<deliverable folder>/index.html works, which is how the build ships.
Prints the exact URLs and the two things that usually break it.
"""
import argparse
import getpass
import io
import os
import socket
import ssl
import stat
import subprocess
import sys
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer

HERE = os.path.dirname(os.path.abspath(__file__))
DELIVERABLE = os.path.dirname(HERE)


def deliverable_folder(root):
    for name in sorted(os.listdir(root)):
        if name.startswith("preview-spec_04-swift") and os.path.isdir(os.path.join(root, name)):
            return name
    return None


def traversal_blockers(root):
    """Dirs that a non-owner server user (www-data, nobody) cannot pass through → 403."""
    blocked, path = [], "/"
    for part in os.path.abspath(root).split(os.sep):
        path = os.path.join(path, part)
        try:
            mode = os.stat(path).st_mode
        except OSError:
            break
        if not mode & stat.S_IXOTH:
            blocked.append(path)
    return blocked


class Handler(SimpleHTTPRequestHandler):
    bot = False  # --bot: serve web/index.html with the engine arg --bot (make web-smoke over any URL)

    def send_head(self):
        if Handler.bot and self.path.split("?")[0].rstrip("/").endswith("/web/index.html"):
            path = self.translate_path(self.path)
            if os.path.isfile(path):
                body = open(path, "rb").read().replace(b'"args":[]', b'"args":["--","--bot"]')
                self.send_response(200)
                self.send_header("Content-Type", "text/html; charset=utf-8")
                self.send_header("Content-Length", str(len(body)))
                self.end_headers()
                return io.BytesIO(body)
        return super().send_head()

    def guess_type(self, path):
        ext = os.path.splitext(path)[1].lower()
        if ext == ".wasm":
            return "application/wasm"  # streaming instantiation needs this
        if ext == ".pck":
            return "application/octet-stream"
        return super().guess_type(path)


def make_selfsigned(cert, key, host):
    cmd = ["openssl", "req", "-x509", "-newkey", "rsa:2048", "-nodes", "-days", "30",
           "-subj", "/CN=%s" % host, "-out", cert, "-keyout", key,
           "-addext", "subjectAltName=DNS:%s,DNS:localhost,IP:127.0.0.1" % host]
    subprocess.run(cmd, check=True, capture_output=True)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dir", default=os.path.dirname(DELIVERABLE))
    ap.add_argument("--port", type=int, default=8000)
    ap.add_argument("--host", default="0.0.0.0")
    ap.add_argument("--https", action="store_true", help="self-signed TLS: browsers treat http://<hostname> as unsafe (Godot needs a secure context)")
    ap.add_argument("--cert", default=None)
    ap.add_argument("--key", default=None)
    ap.add_argument("--bot", action="store_true", help="serve web/index.html with engine arg --bot (make web-smoke proves a win over any URL)")
    args = ap.parse_args()
    Handler.bot = args.bot

    root = os.path.abspath(args.dir)
    folder = deliverable_folder(root)

    if args.port < 1024 and os.geteuid() != 0:
        raise SystemExit(
            "port %d needs root. 跑 sudo python3 tools/serve.py --port %d，"
            "或免 root 用 --port 8000（http://<hostname>:8000/...）。" % (args.port, args.port))

    if folder is None:
        print("警告：在 %s 找不到 preview-spec_04-swift* 目錄。" % root)
    elif not os.path.exists(os.path.join(root, folder, "web", "index.html")):
        print("警告：%s/%s 裡沒有 web/ → 先 cd %s && make web" % (root, folder, folder))

    host = args.host if args.host != "0.0.0.0" else socket.gethostname()
    scheme = "https" if args.https else "http"
    for label, path in (("入口（仓库根）", ""),
                        ("交付目錄入口", "%s/index.html" % folder if folder else ""),
                        ("WebGL 直接", "%s/web/index.html" % folder if folder else "")):
        if path:
            print("  %-14s %s://%s:%d/%s" % (label, scheme, host, args.port, path))
    print("  %-14s http://localhost:%d/" % ("本機", args.port))

    if not args.https:
        print("提示：純 HTTP 的非本機網址不是安全來源，瀏覽器不提供 AudioWorklet → 自訂殼層（godot/web_shell.html）"
              "已放行並把音訊驅動接成靜音；本遊戲無音效，照常玩。")
        print("      要遠端玩請開在 8000 以上的埠，或用 --https（自簽憑證）、ssh -L 8000:localhost:8000 %s@%s"
              % (getpass.getuser(), host))

    for path in traversal_blockers(root):
        print("注意：%s 沒有 other 的 x 權限 → 以 www-data/nobody 跑的 server（nginx、apache）"
              "會回 403。需要時：chmod o+x %s" % (path, path))

    os.chdir(root)
    httpd = ThreadingHTTPServer((args.host, args.port), Handler)
    if args.https:
        cert = args.cert or os.path.join(DELIVERABLE, "web", "selfsigned.pem")
        key = args.key or os.path.join(DELIVERABLE, "web", "selfsigned.key")
        if not (os.path.exists(cert) and os.path.exists(key)):
            make_selfsigned(cert, key, host)
            print("已產生自簽憑證 %s（瀏覽器會警告，選繼續即可）" % cert)
        ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
        ctx.load_cert_chain(cert, key)
        httpd.socket = ctx.wrap_socket(httpd.socket, server_side=True)
    print("serve %s on %s://%s:%d/  (Ctrl-C 停)" % (root, scheme, args.host, args.port))
    sys.stdout.flush()  # the URL list must show up even when piped to a log
    httpd.serve_forever()


if __name__ == "__main__":
    main()
