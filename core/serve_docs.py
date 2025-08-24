#!/usr/bin/env python3
import http.server
import socketserver
import os
import sys
from pathlib import Path
import socket

# Bazel runs binaries from the execroot; we expect the docs path to be provided.
# Usage: bazel run //core:serve_docs -- [port]
# It will locate the rustdoc output for //core:docs automatically if not built.

DEFAULT_PORT = 8000

# Relative path from bazel-bin to the docs output directory name produced by rust_doc
DOCS_OUTPUT = "core/docs.rustdoc/"

def find_docs_dir():
    # Determine bazel-bin via environment if available
    bazel_bin = os.environ.get("BUILD_WORKING_DIRECTORY")
    if bazel_bin:
        # BUILD_WORKING_DIRECTORY points to workspace root when using bazel run
        candidate = Path(bazel_bin) / "bazel-bin" / DOCS_OUTPUT
    else:
        candidate = Path("bazel-bin") / DOCS_OUTPUT
    if candidate.is_dir():
        return candidate
    # Fallback: try runfiles structure
    runfiles = Path(sys.argv[0]).parent
    for p in [runfiles / DOCS_OUTPUT, Path.cwd() / DOCS_OUTPUT]:
        if p.is_dir():
            return p
    print("Could not locate docs directory. Please run: bazel build //core:docs", file=sys.stderr)
    sys.exit(1)

def choose_redirect(docs_dir: Path):
    """If docs_dir has no index.html but exactly one crate subdir with index, return its relative index path."""
    if (docs_dir / "index.html").exists():
        return None
    candidates = []
    for p in docs_dir.iterdir():
        if p.is_dir() and (p / "index.html").exists():
            candidates.append(p.name + "/index.html")
    if len(candidates) == 1:
        return candidates[0]
    return None

def main():
    port = int(sys.argv[1]) if len(sys.argv) > 1 else DEFAULT_PORT
    docs_dir = find_docs_dir()
    redirect_target = choose_redirect(docs_dir)

    os.chdir(docs_dir)

    class RedirectingHandler(http.server.SimpleHTTPRequestHandler):
        def do_GET(self):  # noqa: N802
            if redirect_target and (self.path == "/" or self.path == "/index.html"):
                self.send_response(302)
                self.send_header("Location", "/" + redirect_target)
                self.end_headers()
                return
            super().do_GET()

        def log_message(self, format, *args):  # Reduce noise
            sys.stderr.write("[docs] " + (format % args) + "\n")

    class DocTCPServer(socketserver.TCPServer):
        allow_reuse_address = True
        def server_bind(self):
            # Allow immediate rebinding after restart
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            # Try SO_REUSEPORT (ignored if unsupported)
            try:
                self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
            except (AttributeError, OSError):
                pass
            return super().server_bind()

    handler = RedirectingHandler

    # Try chosen port; if busy (Errno 48), increment until free.
    original_port = port
    while True:
        try:
            with DocTCPServer(("", port), handler) as httpd:
                if redirect_target:
                    print(f"Serving docs from {docs_dir} (redirecting / to /{redirect_target}) at http://localhost:{port}/ (Ctrl+C to stop)")
                else:
                    print(f"Serving docs from {docs_dir} at http://localhost:{port}/ (Ctrl+C to stop)")
                try:
                    httpd.serve_forever()
                except KeyboardInterrupt:
                    pass
                finally:
                    print("Stopping server")
                break
        except OSError as e:
            if getattr(e, "errno", None) == 48:  # Address in use
                port += 1
                if port - original_port > 20:
                    print("Could not find a free port after 20 increments.", file=sys.stderr)
                    sys.exit(1)
            else:
                raise

if __name__ == "__main__":
    main()
