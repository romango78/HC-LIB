# HC-LIB
## Arduino SRD05VDCSL Library v1.0.2609
This __library__ defines __SRD05VDCSLDevice__ for the Songle SRD-05VDC-SL-C relay.

The PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`).

### Dependencies
- HC-LIB.Devices v1.1.2609

### Features
- Defines __SRD05VDCSLDevice__ as a __RelayDevice__ on a digital pin.
- Drive it with __RelayDeviceController__ (__on__ / __off__ / __getState__, __RelayState__ __On__ / __Off__).
- Controllers return __Error__ / __Expected{RelayState, Error}__.

### Device purpose
The Songle __SRD-05VDC-SL-C__ is a 5 V electromagnetic relay. It lets a microcontroller switch a separate load (mains lamp, pump, heater, motor, or another DC circuit) while keeping the MCU and the load electrically isolated.

Use it when a digital pin must control a voltage or current that the AVR GPIO cannot source or sink. __RelayDeviceController__ energizes the coil (__On__) or releases it (__Off__). The load is wired on the contact side (COM / NO / NC), not on the GPIO.

### Device characteristics
Values below are typical for the Songle SRD-05VDC-SL-C (confirm the marking on your part). Most Arduino breakout boards add a transistor driver, flyback diode, and LED; this library talks to that __IN__ pin, not the coil pins.

| Item | Typical value |
|---|---|
| Coil voltage | 5 V DC |
| Coil resistance / power | ~70 Ω / ~0.36 W (~70 mA) |
| Contacts | SPDT (1 Form C): COM, NO, NC |
| Contact rating | up to 10 A @ 250 V AC or 30 V DC (some markings 15 A) |
| Operate / release time | ~10 ms |
| Isolation | coil isolated from contacts |
| Module input | active-low: GPIO LOW = coil on, HIGH = coil off |

__RelayDeviceController__ matches the module: __On__ writes __LOW__, __Off__ writes __HIGH__. Do not drive the bare coil from an ATmega328 pin (absolute max 40 mA). Use a module with a driver, or an external transistor.

### How to connect device to Arduino board
The example uses a 1-channel 5 V relay module on an Arduino Nano (ATmega328, new bootloader). Control wiring:

| Module pin | Arduino Nano | Role |
|---|---|---|
| VCC | 5V | Module and coil supply |
| GND | GND | Common ground |
| IN | D3 | Control signal (`SRD05VDCSL_PIN` in the example) |

Load side (screw terminals, isolated from the MCU):

| Terminal | When the coil is off | When the coil is on |
|---|---|---|
| COM | Common of the switched circuit | Common of the switched circuit |
| NC | Connected to COM | Open |
| NO | Open | Connected to COM |

Typical load: supply → load → NO, and COM → supply return (or the other way around). Keep mains wiring away from the Nano. Switch the live conductor on COM/NO and use a fuse rated for the load.

1. Wire VCC, GND, and IN as in the table. Leave the load disconnected until the coil clicks under software control.
2. Upload __examples/SRD05VDCSLExampleApp__ (`pio run -e nano-board -t upload`). Serial is `115200` on `COM3`.
3. The sketch starts with __off__, then toggles every 5 s. The module LED (if present) follows the coil.

To use another digital pin, change `SRD05VDCSL_PIN` and move the IN wire to that pin.

### Environments
Run commands from this folder (`SRD05VDCSL-lib`).

| Environment | Platform | Purpose |
|---|---|---|
| `nano-board` | Arduino Nano (ATmega328, new bootloader) | Firmware: `setup()` / `loop()` from __test/tests_runner.cpp__ |
| `desktop` | native | Unity tests |
| `desktop-debug` | native | Unity tests with debug symbols (`-O0 -ggdb3`). Default. |

__test/tests_runner.cpp__ is the single entry point: a firmware stub when `UNIT_TEST` is off, and the Unity runner when it is on.

### Build
```powershell
pio run -e nano-board
pio run -e nano-board -t upload
pio run -e desktop-debug
```

Firmware is written to `.pio/build/nano-board/firmware.hex`. Default serial settings are `115200` baud on `COM3`.

### Unit tests
```powershell
pio test -e desktop-debug
```

Use `-e desktop` for a non-debug native run, or `-e nano-board` to run tests on the board. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details.

### Sample
The relay demo is in the __examples/SRD05VDCSLExampleApp__ folder. From that folder:
```powershell
pio run -e nano-board -t upload
```

### Packages
Arduino Library Manager metadata is in [library.properties](library.properties). PlatformIO metadata is in [library.json](library.json).

* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package `HC-LIB.SRD05VDCSL-{version}.tar.gz` in the `{local_repo_folder}` folder.
* The `pio package publish {local_repo_folder}/HC-LIB.SRD05VDCSL-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.SRD05VDCSL.nuspec -outputdirectory {local_repo_folder}` command is used for creating NuGet package and store it in the `{local_repo_folder}` folder.
* The `nuget.exe install HC-LIB.SRD05VDCSL` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
