# Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
#
# `pio run` needs firmware/firmware_stub.cpp for setup()/loop().
# `pio test` already compiles test/tests_runner.cpp, so the stub must stay out.

Import("env")

import sys

if "test" not in sys.argv:
    env.Append(SRC_FILTER=["+<../firmware/firmware_stub.cpp>"])
