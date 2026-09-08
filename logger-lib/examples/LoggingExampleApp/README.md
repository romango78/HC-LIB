# HC-LIB
## Logging Example App
This __application__ shows how a sketch uses __HC-LIB.Logger__: a singleton __LogManager__, __F()__ format strings, and a __LogModule__ name.

Run commands from this folder (`logger-lib/examples/LoggingExampleApp`).

### Dependencies
- HC-LIB.Logger v1.0.2609 (`file://../../../packages/HC-LIB.Logger-1.0.2609.tar.gz`)

The library package includes Marco Paland’s tiny __printf__. This app compiles `printf.c` itself and disables float / long-long converters (see `src/app.config/app.config.h`) so the Nano does not pull soft-float into SRAM.

### What it does
Every second it writes five lines (debug, info, warn, error, fatal) with the module name __Logging Example App__.

- Arduino: __SerialLogPersister__ at `115200` baud. __DateTimeProvider__ seeds from compile-time `__DATE__` / `__TIME__` and advances with `millis()` (the Nano has no RTC).
- Native: __StdoutLogPersister__ and the host local clock.

`gLogLevel()` returns __LogLevel::Debug__, so all five levels are printed. Change that function to filter.

__LogModule__ is assigned in `setup()`. On AVR, `F()` / `PSTR()` cannot initialize a global.

### Environments
| Environment | Platform | Purpose |
|---|---|---|
| `nano-board` | Arduino Nano (ATmega328, new bootloader) | Firmware on Serial. Default. |
| `desktop` | native | Prints to stdout |
| `desktop-debug` | native | Same as `desktop` with debug symbols (`-O0 -ggdb3`) |

`[env:nano-board]` shrinks Serial buffers (`16` / `32`) because the demo only prints short lines.

### Build
Arduino:
```powershell
pio run -e nano-board
pio run -e nano-board -t upload
pio device monitor -e nano-board
```

Firmware is written to `.pio/build/nano-board/firmware.hex`. Default serial settings are `115200` baud. `platformio.ini` uses `/dev/ttyUSB0`; on Windows set `upload_port` / `monitor_port` to `COM3` (or the port Device Manager shows). Close the monitor before the next upload.

Native:
```powershell
pio run -e desktop
.pio\build\desktop\program.exe
```

Use `pio run -e desktop-debug` for a debug build. Stop the process with Ctrl+C.

If the Logger package was just packed, delete `.pio/libdeps` so PlatformIO unpacks the new tarball.

### Inspect memory usage
A Nano has 32 KB flash and 2 KB SRAM. **Program** is flash; **Data** is SRAM (`.data` + `.bss`). Native builds have no 2 KB limit.

```powershell
pio run -e nano-board
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -C --mcu=atmega328p .pio\build\nano-board\firmware.elf
```

Largest symbols (`d` / `b` / `B` are RAM; `T` is flash):
```powershell
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-nm.exe" --size-sort --print-size -t d .pio\build\nano-board\firmware.elf
```

Per-section breakdown:
```powershell
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -A .pio\build\nano-board\firmware.elf
```

### Library
See [logger-lib/README.md](../../README.md).
