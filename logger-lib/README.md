# HC-LIB
## Logger Arduino Library v1.0.2609
This __library__ writes formatted log statements to a pluggable output target.

The PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`, `firmware/`).

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
| `nano-board` | Arduino Nano (ATmega328, new bootloader) | `pio run`: __firmware/firmware_stub.cpp__. `pio test`: __test/tests_runner.cpp__ |
| `desktop` | native | Unity tests |
| `desktop-debug` | native | Unity tests with debug symbols (`-O0 -ggdb3`). Default. |

`pio run -e nano-board` links the firmware stub (`setup()` / `loop()`). `pio test` links the Unity runner. Do not put `setup()` / `loop()` in packaged `src/` — that would break consumer sketches. `extra_src_filter.py` adds the stub for `pio run` only.

### Build
```powershell
pio run -e nano-board
pio run -e nano-board -t upload
pio run -e desktop-debug
```

Firmware is written to `.pio/build/nano-board/firmware.hex`. Default serial settings are `115200` baud on `COM3`. `pio run -e desktop-debug` builds the Unity test binary (`build_type = test`), not a board image.

Do not run `pio run` and `pio test` in parallel against the same environment; they share `.pio/build` and can corrupt the SCons cache.

### Unit tests
```powershell
pio test -e desktop-debug
```

Use `-e desktop` for a non-debug native run, or `-e nano-board` to run tests on the board. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details.

`pio test` compiles `test/` only. Project `src/*.cpp` is omitted unless `test_build_src = yes`. `build_src_filter` does not change that for a test build.

This library has __Log.cpp__ in `src/`. The tests call those symbols, so every env sets `test_build_src = yes`.

Do not combine `test_build_src = yes` with `build_src_filter = +<*> +<../test/tests_runner.cpp>`. Then __tests_runner.cpp__ is compiled twice (`setup` / `loop` defined twice).

PIO tests depend on HC-LIB.System for __unity_extensions.h__ (included after __Log.h__ so Logger’s native __F()__ overload is not replaced). On AVR it copies each __RUN_TEST__ name from flash into a 96-byte RAM buffer.

Nano test SRAM must stay under 2 KB. `[env:nano-board]` shrinks Serial buffers (`16` / `32`) and sets `UNITY_EXCLUDE_DETAILS`, `UNITY_EXCLUDE_FLOAT`, and `UNITY_EXCLUDE_FLOAT_PRINT`. After packing a new System tarball, delete `.pio/libdeps` so PlatformIO unpacks `unity_extensions.h`.

### Inspect memory usage
A Nano has 32 KB flash and 2 KB SRAM. **Program** is flash; **Data** is SRAM (`.data` + `.bss`). Native builds have no 2 KB limit.

Firmware (`pio run -e nano-board`):
```powershell
pio run -e nano-board
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -C --mcu=atmega328p .pio\build\nano-board\firmware.elf
```

The test image uses the same ELF path (`pio test` overwrites it):
```powershell
pio test -e nano-board --without-uploading --without-testing
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

### Sample
The Serial demo is [examples/LoggingExampleApp](examples/LoggingExampleApp/README.md). From that folder:
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
