# HC-LIB
## Arduino System Library v1.1.2609
This __library__ contains fundamental types for Arduino and native builds: __Expected__, __Error__ / __GenericError__, __ITimer__ / __ArduinoTimer__, __ICloneable__, and Arduino std utility traits.

The PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`, `firmware/`).

### Dependencies
No Dependencies

### Features
- Defines __Error__ and __ErrorCategory__, plus __GenericError__ codes (__NoError__, __ArgumentIsNull__, __OutOfRange__, __OutOfMemory__, __NotSupportedOperation__, __InvalidOperation__).
- Defines generic __ICloneable{T}__ abstraction.
- Defines __ITimer__ abstraction and __ArduinoTimer__ (Arduino __millis()__, host __steady_clock__ on native). One-shot: __isElapsed()__ latches; __start()__ restarts; __setInterval()__ is ignored while running.
- Implements __Expected{T, E}__ for returning a value or an error via __Unexpected{E}__ and __make_error()__. Call __hasValue()__ before __getValue()__.
- Provides __lib-utility.h__: Arduino __std::move__, __std::forward__, __std::remove_reference__, __std::remove_cv__, and __std::decay__. Native builds include `<utility>` and `<type_traits>`.
- Provides __flash.h__: __flash_c_str__ copies an __F()__ string into a RAM buffer for C APIs. On native builds __F()__ is a pass-through.

### Usage
Return a value or an error with __Expected{T, E}__:
```c++
Expected<uint8_t, Error> result = make_error(GenericError::OutOfRange);
if (result.hasValue())
{
    uint8_t value = result.getValue();
}
else if (result.getError() == GenericError::OutOfRange)
{
    Error error = result.getError();
}
```

Use __ArduinoTimer__ as an interval countdown:
```c++
ITimer *timer = new ArduinoTimer();
timer->setInterval(1000);
timer->start();
if (timer->isElapsed())
{
    timer->stop();
}
```

### Environments
Run commands from this folder (`system-lib`).

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

This library has implementations in its own `src/` (__ArduinoTimer.cpp__, __GenericErrors.cpp__). The tests call those symbols, so every env sets `test_build_src = yes`. Header-only HC-LIB projects (for example Collections) do not need that flag: their `src/` is templates, and this library’s `.cpp` files arrive through `lib_deps`, which PlatformIO always compiles.

Do not combine `test_build_src = yes` with `build_src_filter = +<*> +<../test/tests_runner.cpp>`. Then __tests_runner.cpp__ is compiled twice (`setup` / `loop` defined twice).

Assertion messages may use __F()__ so the text stays in flash on AVR. Include __unity_flash.h__ after __unity.h__ (it ships in this package, empty unless `UNIT_TEST` is set). It converts __FlashStringHelper*__ via __flash_c_str__. On AVR it also copies each __RUN_TEST__ name from flash into a 96-byte RAM buffer. Other HC-LIB test runners include the same header after they depend on HC-LIB.System.

Nano test SRAM must stay under 2 KB. `[env:nano-board]` shrinks Serial buffers (`16` / `32`) and sets `UNITY_EXCLUDE_DETAILS`, `UNITY_EXCLUDE_FLOAT`, and `UNITY_EXCLUDE_FLOAT_PRINT`.

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

### Packages
Arduino Library Manager metadata is in [library.properties](library.properties). PlatformIO metadata is in [library.json](library.json).

* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package `HC-LIB.System-{version}.tar.gz` in the `{local_repo_folder}` folder.
* The `pio package publish {local_repo_folder}/HC-LIB.System-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.SYSTEM.nuspec -outputdirectory {local_repo_folder}` command is used for creating NuGet package and store it in the `{local_repo_folder}` folder.
* The `nuget.exe install HC-LIB.SYSTEM` command is used for installing NuGet package.

After packing System, dependents that use `file://../packages/HC-LIB.System-{version}.tar.gz` must delete `.pio/libdeps` so PlatformIO unpacks the new tarball.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
