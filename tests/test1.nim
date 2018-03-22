import assistantpkg/shared/hello
import unittest
import strformat

suite "shared":
    test "hello":
        doAssert helloString == "Hello World", &"got {helloString}"
