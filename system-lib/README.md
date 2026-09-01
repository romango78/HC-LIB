# HC-LIB
## Arduino System Library v1.1.2609
This __library__ contains fundamental types for Arduino and native builds: __Expected__, __Error__ / __GenericError__, __ITimer__ / __ArduinoTimer__, __ICloneable__, and Arduino std utility traits.

### Dependencies
No Dependencies

### Features
- Defines __Error__ and __ErrorCategory__, plus __GenericError__ codes (__NO_ERROR__, __ARGUMENT_IS_NULL__, __OUT_OF_RANGE__, __OUT_OF_MEMORY__, __NOT_SUPPORTED_OPERATION__, __INVALID_OPERATION__, __UNKNOWN_ERROR__).
- Defines generic __ICloneable{T}__ abstraction.
- Defines __ITimer__ abstraction and __ArduinoTimer__ (Arduino __millis()__, host __steady_clock__ on native). One-shot: __isElapsed()__ latches; __start()__ restarts; __setInterval()__ is ignored while running.
- Implements __Expected{T, E}__ for returning a value or an error via __Unexpected{E}__ and __make_error()__.
- Provides __lib-utility.h__: Arduino __std::move__, __std::forward__, __std::remove_reference__, __std::remove_cv__, and __std::decay__. Native builds include _\<utility\>_ and _\<type_traits\>_.

### Usage
Return a value or an error with __Expected{T, E}__:
```c++
Expected<uint8_t, Error> result = make_error(GenericError::OUT_OF_RANGE);
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

### Sample
The __sample application__ is stored in the __sample__ folder.

* `pio run -e nano-board` builds Arduino Nano firmware.
* `pio run -e nano-board -t upload` uploads the firmware.
* `pio run -e desktop-debug` builds the native Unity tests with debug symbols.

See the included examples and tests for further usage examples.

### Unit tests
From the __sample__ folder:

```powershell
pio test -e desktop
```


Use `-e desktop` for a non-debug native run, or `-e nano-board` to run tests on the board. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details.

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.SYSTEM-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.SYSTEM.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.SYSTEM` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
