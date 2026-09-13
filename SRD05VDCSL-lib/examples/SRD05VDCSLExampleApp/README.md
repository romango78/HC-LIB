# HC-LIB
## SRD05VDCSL Example App
This __application__ shows how a sketch uses __HC-LIB.SRD05VDCSL__: an __SRD05VDCSLDevice__ on a digital pin, driven by __RelayDeviceController__ (__on__ / __off__ / __getState__).

Run commands from this folder (`SRD05VDCSL-lib/examples/SRD05VDCSLExampleApp`).

### Dependencies
- HC-LIB.SRD05VDCSL v1.0.2609 (`file://../../../packages/HC-LIB.SRD05VDCSL-1.0.2609.tar.gz`)
- HC-LIB.Devices v1.1.2609 (and the IO / System packages it needs)

### What it does
`setup()` builds the device and turns the coil __off__. Every 5 s, `loop()` reads __RelayState__, prints it, and toggles.

- Arduino: __DigitalStream__ + __DigitalPortAdapter__ on `SRD05VDCSL_PIN` (D3). Serial at `115200` baud. Status strings use __F()__ so they stay in flash.
- Native: __FakeDigitalStream__ and stdout. There is no relay; the fake stream still accepts __on__ / __off__.

Wiring (VCC / GND / IN on D3, load on COM / NO / NC) is in the [library README](../../README.md). Leave the load disconnected until the coil clicks.

To use another digital pin, change `SRD05VDCSL_PIN` and move the IN wire.

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

If the SRD05VDCSL package was just packed, delete `.pio/libdeps` so PlatformIO unpacks the new tarball.

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
See [SRD05VDCSL-lib/README.md](../../README.md).
