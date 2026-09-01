# HC-LIB
Arduino Libraries

## Libraries
The following Arduino libraries are available in this repository:

| Library | Folder | Version | Dependencies |
|---|---|---|---|
| [HC-LIB.System](system-lib/README.md) | `system-lib` | 1.0.2404 | none |
| [HC-LIB.Collections](collections-lib/README.md) | `collections-lib` | 1.0.2203 | HC-LIB.System v1.0.2201 |
| [HC-LIB.Logger](logger-lib/README.md) | `logger-lib` | 1.0.2110 | none |
| [HC-LIB.IO-Abstraction](io-abstraction-lib/README.md) | `io-abstraction-lib` | 1.0.2203 | HC-LIB.System v1.0.2203 |
| [HC-LIB.IO-Port](io-port-lib/README.md) | `io-port-lib` | 1.0.2203 | HC-LIB.System v1.0.2203<br>HC-LIB.IO-Abstraction v1.0.2203 |
| [HC-LIB.Device.Abstractions](device-abstraction-lib/README.md) | `device-abstraction-lib` | 1.0.2404 | HC-LIB.System v1.0.2201 |
| [HC-LIB.Devices](devices-lib/README.md) | `devices-lib` | 1.0.2203 | HC-LIB.Device.Abstractions v1.0.2201<br>HC-LIB.IO-Port v1.0.2203 |
| [HC-LIB.ZMPT101B](ZMPT101B-lib/README.md) | `ZMPT101B-lib` | 1.0.2203 | HC-LIB.System v1.0.2404<br>HC-LIB.Device.Abstractions v1.0.2404<br>HC-LIB.Devices v1.0.2203 |
| [HC-LIB.SRD05VDCSL](SRD05VDCSL-lib/README.md) | `SRD05VDCSL-lib` | 1.0.2203 | HC-LIB.Devices v1.0.2203 |

`io-twi-lib` contains TWI (I2C) stream implementations that depend on HC-LIB.IO-Abstraction. It is not published as a package yet.

Each library lives in its own folder. Firmware, sketches, and unit tests are in that library's `sample` folder (a PlatformIO project).

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

1. Open a PlatformIO terminal and run `pio platform install windows_x86`. This installs MinGW under `{HOME}\.platformio\packages\toolchain-gccmingw32\bin`, where {HOME} is 
where the current user directory is, for standard installations 
of PlatformIO.
2. Add that directory to the user `PATH`, so when the C++ compiler command g++ is 
called the system knows where it exists.
3. Restart the editor, then run `g++ -v` to confirm the compiler is found.

On Windows, if `pio` is not on `PATH`, use:

```powershell
& "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe"
```

Commands below assume you run them from a library `sample` folder, for example `system-lib/sample`.

## Environments
Sample projects typically define these PlatformIO environments in `platformio.ini`:

| Environment | Platform | Purpose |
|---|---|---|
| `nanonew` | Arduino Nano (ATmega328, new bootloader) | Release firmware: build, upload, serial monitor |
| `desktop` | native (host PC) | Unit tests |
| `desktop-debug` | native (host PC) | Unit tests with debug symbols (`-O0 -ggdb3`). Present in `system-lib`. |

`nanonew` is a release build (optimized firmware). `desktop-debug` (or `desktop` with `build_type = debug`) is the debug build for GDB.

Default serial settings for `nanonew` are `115200` baud on `COM3`. Change `upload_port` / `monitor_port` in `platformio.ini` to match the board.

## Build
### Release (Arduino firmware)
```powershell
pio run -e nanonew
```

Firmware is written to `.pio/build/nanonew/firmware.hex`.

### Debug (native unit tests)
```powershell
pio run -e desktop-debug
```

If the sample has no `desktop-debug` environment, use `desktop` (several samples already set `build_type = debug`) or add:

```ini
[env:desktop-debug]
platform = native
build_type = debug
debug_test = *
debug_build_flags = -O0 -ggdb3 -g3
```

`debug_test = *` tells PlatformIO to debug the Unity test binary instead of `src/main.cpp`.

## Upload to an Arduino board
Connect the Nano, then from the library `sample` folder:

```powershell
pio run -e nanonew -t upload
```

This compiles release firmware (if needed) and flashes the board. If upload fails, check the COM port in Device Manager and update `upload_port` in `platformio.ini`.

Open the serial monitor after upload:

```powershell
pio device monitor -e nanonew
```

Close the monitor before the next upload; Windows will not share the COM port.

## Unit tests
Run native tests (no board required):

```powershell
pio test -e desktop
```

Run tests on the board:

```powershell
pio test -e nanonew
```

Tests live under `sample/test`. See [PlatformIO unit testing](https://docs.platformio.org/en/latest/plus/unit-testing.html).

## Debug
Native unit-test debugging is the supported path. On-device debugging of `nanonew` needs a hardware probe and is not configured.

### From the editor (recommended)
1. Open the library `sample` folder as the PlatformIO project (or use `HC-LIB.code-workspace`).
2. Set breakpoints in `sample/test` or in the library headers under `src`.
3. Run and Debug (`Ctrl+Shift+D`) → **PIO Debug** → F5.

**PIO Debug** rebuilds the `desktop-debug` binary, then starts GDB. Use **PIO Debug (skip Pre-Debug)** if the binary is already built.

The workspace launch config `native-debug` starts GDB on:

`sample/.pio/build/desktop-debug/program.exe`

### From the terminal
```powershell
pio debug -e desktop-debug
```

### launch.json
PlatformIO regenerates `sample/.vscode/launch.json` on each debug session. Do not rely on hand-edits there. Put a stable GDB configuration in `HC-LIB.code-workspace` instead:

```json
{
    "type": "cppdbg",
    "request": "launch",
    "name": "native-debug",
    "program": "${workspaceFolder}/sample/.pio/build/desktop-debug/program.exe",
    "args": [],
    "stopAtEntry": false,
    "cwd": "${workspaceFolder}/sample",
    "environment": [],
    "externalConsole": false,
    "MIMode": "gdb"
}
```

`${workspaceFolder}` is the library folder when that folder is the workspace root (as in `HC-LIB.code-workspace`).

## Packages
Packaging and publish commands are documented in each library README (`pio package pack` / `nuget.exe pack`).

## License
See each library's `LICENSE.md`.
