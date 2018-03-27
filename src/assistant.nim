import assistantpkg/shared/hello
import macros
import strformat
import strutils
import asynchttpserver, asyncdispatch
from posix import onSignal, SIGINT

proc compileFrontend(): string {.compileTime.} =
    hint "[building frontend...]"
    let (log, exitCode) = gorgeEx("nim js assistantpkg/frontend/main")
    if exitCode != 0:
        error &"error code {exitCode} compiling frontend:\n{log}"
    if log != "":
        let indentedLog = indent(log, 4)
        hint &"[frontend build succesful. logs:]\n{indentedLog}"
    hint "[reading compiled frontend...]"
    const code = staticRead("assistantpkg/frontend/nimcache/main.js")
    hint "[frontend successfully compiled and read]"
    return code

const frontendCode = compileFrontend()

echo &"starting server that will serve {helloString}"

var server = newAsyncHttpServer()

onSignal(SIGINT): # FIXME: doesn't quite work yet, probably not necessary for now though
    echo "closing server"
    server.close()

proc serveHttp(req: Request) {.async.} =
    if req.url.path == "/hello.js":
        await req.respond(Http200, frontendCode)
    else:
        await req.respond(Http404, "not found\n")

proc safeServe(server: AsyncHttpServer, port: Port, handler: proc(req: Request): Future[void] {.gcsafe.}) {.async.} =
    try: # FIXME: doesn't quite work yet, probably not necessary for now though
        await server.serve(port, handler)
    except:
        let name = getCurrentException().name
        let message = getCurrentExceptionMsg()
        echo &"serve threw {name} exception: {message}"

try:
    waitFor server.safeServe(Port(8080), serveHttp)
except:
    let name = getCurrentException().name
    let message = getCurrentExceptionMsg()
    echo &"safeServe threw {name} exception: {message}"
echo "server closed"

