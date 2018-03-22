import assistant_shared
import unittest
import strformat

suite "shared":
    test "hello":
        doAssert helloString == "Hello World", &"got {helloString}"
