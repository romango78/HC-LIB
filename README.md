# HC-LIB
Arduino Libraries

## Libraries
The following Arduino libraries are available in this repository:

| Library | Folder | Version | Dependencies |
|---|---|---|---|
| [HC-LIB.System](system-lib/README.md) | `system-lib` | 1.1.2609 | none |
| [HC-LIB.Collections](collections-lib/README.md) | `collections-lib` | 1.0.2609 | HC-LIB.System ≥1.1.2609 |
| [HC-LIB.Logger](logger-lib/README.md) | `logger-lib` | 1.0.2609 | none |
| [HC-LIB.IO-Abstraction](io-abstraction-lib/README.md) | `io-abstraction-lib` | 1.1.2609 | HC-LIB.System ≥1.1.2609 |
| [HC-LIB.IO-Port](io-port-lib/README.md) | `io-port-lib` | 1.1.2609 | HC-LIB.System ≥1.1.2609<br>HC-LIB.IO-Abstraction ≥1.1.2609 |
| [HC-LIB.Device.Abstractions](device-abstraction-lib/README.md) | `device-abstraction-lib` | 1.1.2609 | HC-LIB.System ≥1.1.2609 |
| [HC-LIB.Devices](devices-lib/README.md) | `devices-lib` | 1.1.2609 | HC-LIB.System ≥1.1.2609<br>HC-LIB.Device.Abstractions ≥1.1.2609<br>HC-LIB.IO-Port ≥1.1.2609 |
| [HC-LIB.ZMPT101B](ZMPT101B-lib/README.md) | `ZMPT101B-lib` | 1.0.2609 | HC-LIB.System ≥1.1.2609<br>HC-LIB.IO-Abstraction ≥1.1.2609<br>HC-LIB.IO-Port ≥1.1.2609<br>HC-LIB.Device.Abstractions ≥1.1.2609<br>HC-LIB.Devices ≥1.1.2609 |
| [HC-LIB.SRD05VDCSL](SRD05VDCSL-lib/README.md) | `SRD05VDCSL-lib` | 1.0.2609 | HC-LIB.System ≥1.1.2609<br>HC-LIB.IO-Abstraction ≥1.1.2609<br>HC-LIB.IO-Port ≥1.1.2609<br>HC-LIB.Device.Abstractions ≥1.1.2609<br>HC-LIB.Devices ≥1.1.2609 |

`io-twi-lib` contains TWI (I2C) stream implementations that depend on HC-LIB.IO-Abstraction. It is not published as a package yet.

Each published library is its own PlatformIO project (`platformio.ini`, `src/`, `test/`, `firmware/`). Sketches, when present, live under `examples/` (for example __ZMPT101B-ReadExampleApp__). Do not put `setup()` / `loop()` in packaged `src/` — that would break consumer sketches. `extra_src_filter.py` adds `firmware/firmware_stub.cpp` for `pio run` only.

Local PlatformIO tarballs are under `packages/`. After packing a dependency, delete the consumer’s `.pio/libdeps` so PlatformIO unpacks the new version.

## Development Environment
Install **Visual Studio Code** or **Cursor** with:

* PlatformIO IDE v2.5.0 or higher
* C/C++ v1.9.2 or higher
* C/C++ Debug v0.4.0 or higher
* Code Spell Checker v2.2.3 or higher
* Native Debug v0.27.0 or higher
* Makefile Creator v0.0.3 or higher
* vscode-pdf v1.2.2 or higher

The host `g++` compiler should be installed locally. If it is missing:

1. Open a PlatformIO terminal and run `pio platform install windows_x86`. This installs MinGW under `{HOME}\.platformio\packages\toolchain-gccmingw32\bin`, where {HOME} is the current user directory for a standard PlatformIO install.
2. Add that directory to the user `PATH`, so when the C++ compiler command `g++` is called the system knows where it exists.
3. Restart the editor, then run `g++ -v` to confirm the compiler is found.

On Windows, if `pio` is not on `PATH`, use:

```powershell
& "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe"
```

Commands below assume you run them from a library folder, for example `system-lib` or `ZMPT101B-lib`.

## Environments
Library projects typically define these PlatformIO environments in `platformio.ini`:

| Environment | Platform | Purpose |
|---|---|---|
| `nano-board` | Arduino Nano (ATmega328, new bootloader) | `pio run`: firmware stub. `pio test`: Unity on the board |
| `desktop` | native (host PC) | Unit tests |
| `desktop-debug` | native (host PC) | Unit tests with debug symbols (`-O0 -ggdb3`). Default in most libraries |

`nano-board` is a release build (optimized firmware). `desktop-debug` uses `build_type = test` so `pio run` builds the Unity binary with debug symbols.

Default serial settings for `nano-board` are `115200` baud. Many `platformio.ini` files use `/dev/ttyUSB0`; on Windows set `upload_port` / `monitor_port` / `test_port` to `COM3` (or the port Device Manager shows).

Do not run `pio run` and `pio test` in parallel against the same environment; they share `.pio/build` and can corrupt the SCons cache.

## Build
### Release (Arduino firmware)
```powershell
pio run -e nano-board
```

Firmware is written to `.pio/build/nano-board/firmware.hex`.

### Debug (native unit tests)
```powershell
pio run -e desktop-debug
```

`pio run -e desktop-debug` builds the Unity test binary, not a board image.

## Upload to an Arduino board
Connect the Nano, then from the library folder (or an `examples/...` sketch folder):

```powershell
pio run -e nano-board -t upload
```

This compiles release firmware (if needed) and flashes the board. If upload fails, check the COM port in Device Manager and update `upload_port` in `platformio.ini`.

Open the serial monitor after upload:

```powershell
pio device monitor -e nano-board
```

Close the monitor before the next upload; Windows will not share the COM port.

## Unit tests
Run native tests (no board required):

```powershell
pio test -e desktop-debug
```

Use `-e desktop` for a non-debug native run. Run tests on the board:

```powershell
pio test -e nano-board
```

Tests live under each library’s `test/` folder. Libraries with reader or timer implementations in `src/*.cpp` set `test_build_src = yes` so those symbols link into the test binary. See [PlatformIO unit testing](https://docs.platformio.org/en/latest/plus/unit-testing.html).

Assertion messages may use __F()__ so the text stays in flash on AVR. Include __unity_extensions.h__ after __unity.h__ (from HC-LIB.System, empty unless `UNIT_TEST` is set). Nano test builds shrink Serial buffers and set `UNITY_EXCLUDE_DETAILS` (and often float excludes) to stay under 2 KB SRAM.

## Debug
Native unit-test debugging is the supported path. On-device debugging of `nano-board` needs a hardware probe and is not configured.

Each library is its own PlatformIO project. The Unity debug binary is:

`{library}/.pio/build/desktop-debug/program.exe`

Build it before GDB (`pio run -e desktop-debug` or `pio test -e desktop-debug` from that library folder).

### From the editor
1. Open `HC-LIB.code-workspace`.
2. Set breakpoints in that library’s `test/` or `src/`.
3. Open a source file in that library (for example `collections-lib/src/...` or `test/...`).
4. Run and Debug (`Ctrl+Shift+D`) → **native-debug** → F5.

**native-debug** uses the Native Debug (`gdb`) adapter and MinGW `gdb.exe`. `${workspaceFolder}` in this multi-root workspace is `root`, so the config uses `${fileWorkspaceFolder}` (the library that owns the active file). GDB path: `C:/repos/tools/mingw64/bin/gdb.exe`.

**PIO Debug** (PlatformIO) also works when that library folder is the active project. It rebuilds `desktop-debug`, then starts GDB. Use **PIO Debug (skip Pre-Debug)** if the binary is already built.

### From the terminal
```powershell
pio debug -e desktop-debug
```

Run this from the library folder (for example `collections-lib`), not from the repo root.

## Packages
Packaging and publish commands are documented in each library README (`pio package pack` / `nuget.exe pack`).

From the repo root, pack every library that has a `library.json` into `packages/`:

```powershell
.\pack.ps1
```

To pack a single library, run `pio package pack` from that folder (see the library README).

## License
See each library's `LICENSE.md`.
