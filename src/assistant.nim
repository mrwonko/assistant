import assistantpkg/shared/hello
import macros
import strformat
import strutils

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

echo &"// code that will output {helloString}:"
echo frontendCode
