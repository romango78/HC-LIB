# HC-LIB
## Arduino System Library v1.1.2609
This __library__ contains fundamental types for Arduino and native builds: __Expected__, __Error__ / __GenericError__, __ITimer__ / __ArduinoTimer__, __ICloneable__, and Arduino std utility traits.

The PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`).

### Dependencies
No Dependencies

### Features
- Defines __Error__ and __ErrorCategory__, plus __GenericError__ codes (__NoError__, __ArgumentIsNull__, __OutOfRange__, __OutOfMemory__, __NotSupportedOperation__, __InvalidOperation__, __UnknownError__).
- Defines generic __ICloneable{T}__ abstraction.
- Defines __ITimer__ abstraction and __ArduinoTimer__ (Arduino __millis()__, host __steady_clock__ on native). One-shot: __isElapsed()__ latches; __start()__ restarts; __setInterval()__ is ignored while running.
- Implements __Expected{T, E}__ for returning a value or an error via __Unexpected{E}__ and __make_error()__.
- Provides __lib-utility.h__: Arduino __std::move__, __std::forward__, __std::remove_reference__, __std::remove_cv__, and __std::decay__. Native builds include _\<utility\>_ and _\<type_traits\>_.

### Usage
Return a value or an error with __Expected{T, E}__:
```c++
Expected<uint8_t, Error> result = make_error(GenericError::OutOfRange);
if (result.hasValue())
{
    uint8_t value = result.getValue();
}
else
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

### Packages
Arduino Library Manager metadata is in [library.properties](library.properties). PlatformIO metadata is in [library.json](library.json).

* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package `HC-LIB.SYSTEM-{version}.tar.gz` in the `{local_repo_folder}` folder.
* The `pio package publish {local_repo_folder}/HC-LIB.SYSTEM-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.SYSTEM.nuspec -outputdirectory {local_repo_folder}` command is used for creating NuGet package and store it in the `{local_repo_folder}` folder.
* The `nuget.exe install HC-LIB.SYSTEM` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
