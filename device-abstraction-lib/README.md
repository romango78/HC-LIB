# HC-LIB
## Arduino Device Abstraction Library v1.1.2609
This __library__ defines abstractions for devices that can be connected to Arduino Boards.

The PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`).

### Dependencies
- HC-LIB.System v1.1.2609

### Features
- Defines __IDevice__ (type + __DeviceCategory__) and helpers __is_digital__, __is_analog__, __is_relay__.
- Defines __IDeviceController{State, Device}__: __setState__ returns __Error__; __getState__ returns __Expected{State, Error}__.
- Defines __ISensor__, __SensorData{T}__, and __ISensorReader__: __read__ returns __Expected{SensorData, Error}__.
- Defines __DeviceError__ (__TimerIsNotInitialized__, __SensorIsNotInitialized__).

### Environments
Run commands from this folder (`device-abstraction-lib`).

| Environment | Platform | Purpose |
|---|---|---|
| `nano-board` | Arduino Nano (ATmega328, new bootloader) | Firmware: `setup()` / `loop()` from __test/tests_runner.cpp__ |
| `desktop` | native | Unity tests |
| `desktop-debug` | native | Unity tests with debug symbols (`-O0 -ggdb3`) |

__test/tests_runner.cpp__ is the single entry point: a firmware stub when `UNIT_TEST` is off, and the Unity runner when it is on.

### Build
```powershell
pio run -e nano-board
pio run -e nano-board -t upload
pio run -e desktop
```

Firmware is written to `.pio/build/nano-board/firmware.hex`. Default serial settings are `115200` baud on `COM3`.

### Unit tests
```powershell
pio test -e desktop
```

Use `-e desktop-debug` for a debug native run, or `-e nano-board` to run tests on the board. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details.

### Packages
Arduino Library Manager metadata is in [library.properties](library.properties). PlatformIO metadata is in [library.json](library.json).

* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package `HC-LIB.Device.Abstractions-{version}.tar.gz` in the `{local_repo_folder}` folder.
* The `pio package publish {local_repo_folder}/HC-LIB.Device.Abstractions-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.Device.Abstractions.nuspec -outputdirectory {local_repo_folder}` command is used for creating NuGet package and store it in the `{local_repo_folder}` folder.
* The `nuget.exe install HC-LIB.Device.Abstractions` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
