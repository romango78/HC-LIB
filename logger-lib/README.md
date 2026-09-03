# HC-LIB
## Logger Arduino Library v1.0.2609
This __library__ writes formatted log statements to a pluggable output target.

The PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`).

### Dependencies
- printf.h - (c) Marco Paland (info@paland.com). Tiny printf, sprintf and snprintf implementation, optimized for speed on embedded systems with a very limited resources.

### Features
- Defines __Log__ with __debug__ / __info__ / __warn__ / __error__ / __fatal__.
- Defines __LogLevel__ (__Off__, __Critical__, __Error__, __Warn__, __Info__, __Debug__).
- Persists output through __ILogPersister__ (__SerialLogPersister__ on Arduino).
- Optional timestamps from __IDateTimeProvider__ (__getLocalDatetime__ returns __DateTime__ by value).
- The application supplies __gLogLevel()__ as the requested minimum level.
- Prefer __F("...")__ for format strings and __LogModule(F("..."))__ so AVR keeps those literals in flash.

### Environments
Run commands from this folder (`logger-lib`).

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
The Serial demo is in the __examples/LoggingExampleApp__ folder. From that folder:
```powershell
pio run -e nano-board -t upload
```

### Packages
Arduino Library Manager metadata is in [library.properties](library.properties). PlatformIO metadata is in [library.json](library.json).

* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package `HC-LIB.Logger-{version}.tar.gz` in the `{local_repo_folder}` folder.
* The `pio package publish {local_repo_folder}/HC-LIB.Logger-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.Logger.nuspec -outputdirectory {local_repo_folder}` command is used for creating NuGet package and store it in the `{local_repo_folder}` folder.
* The `nuget.exe install HC-LIB.Logger` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
