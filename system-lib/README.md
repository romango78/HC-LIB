# HC-LIB
## Arduino System Library v1.0.2404
This __library__ contains fundamental classes and base classes that define commonly-used data types.

### Dependencies
No Dependencies

### Features
- Defines error type __err_t__ and constants (__NO_ERROR__, __ARGUMENT_IS_NULL_ERROR__, __OUT_OF_RANGE_ERROR__, __OUT_OF_MEMORY_ERROR__, __NOT_SUPPORTED_OPERATION_ERROR__, __INVALID_OPERATION_ERROR__).
- Defines generic __ICloneable{T}__ abstraction.
- Defines __ITimer__ abstraction and __ArduinoTimer__ implementation based on Arduino __millis()__.
- Implements __Expected{T}__ idiom for returning a value or an error.

### Usage
Return a value or an error with __Expected{T}__:
```c++
Expected<uint8_t> result = Expected<uint8_t>::fromError(OUT_OF_RANGE_ERROR);
if (result.hasValue())
{
    uint8_t value = result.getValue();
}
else
{
    err_t error = result.getError();
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
The __sample application__ is stored in __sample__ folder. 
The `pio run -e nanonew -t upload` command is used for compiling and upload the __sample application__.

See the included examples and tests for further usage examples.

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.SYSTEM-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.SYSTEM.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.SYSTEM` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
