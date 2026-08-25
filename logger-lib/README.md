# HC-LIB
## Logger Arduino Library v1.0.2110
This __library__ is a tool to help the programmer output log statements to a variety of output targets.

### Dependencies
- printf.h - (c) Marco Paland (info@paland.com). Tiny printf, sprintf and snprintf implementation, optimized for speed on embedded systems with a very limited resources.

### Features
- Defines __ILogPersister__ abstraction and __SerialLogPersister__ for writing log output to Arduino Serial.
- Defines __IDateTimeProvider__ abstraction and __DateTime__ for optional timestamps.
- Defines __Log__ with levels (__debug__, __info__, __warn__, __error__, __fatal__) and optional __LogModule__ name.
- Formats messages with printf-style placeholders.

### Usage
Provide __gLogLevel()__ in the application, then create a __Log__ with a persister and an optional date/time provider:
```c++
LogLevelEnum gLogLevel()
{
    return LogLevelEnum::debug;
}

ILogPersister *persister = new SerialLogPersister();
IDateTimeProvider *dateTimeProvider = new DateTimeProvider();
Log *logger = new Log(persister, dateTimeProvider);

LogModule module = { "Logging Example App" };
logger->debug(module, "Here can be your debug message.");
logger->info("Here can be your info message.");
logger->warn("Here can be your warning message.");
logger->error("Here can be your error message.");
logger->fatal("Here can be your critical message.");
```

To persist logs to another target, implement __ILogPersister__. To change timestamps, implement __IDateTimeProvider__.

### Sample
The __sample application__ is stored in __sample__ folder. 
The `pio run -e nanonew -t upload` command is used for compiling and upload the __sample application__.

See the included examples and tests for further usage examples.

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.Logger-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.Logger.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.Logger` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
