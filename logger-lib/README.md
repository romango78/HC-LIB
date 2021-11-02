# HC-LIB
## Logger Arduino Library v1.0.2110
This __library__ is a tool to help the programmer output log statements to a variety of output targets. 

### Dependencies
* printf.h - (c) Marco Paland (info@paland.com). Tiny printf, sprintf and snprintf implementation, optimized for speed on embedded systems with a very limited resources.

### Features
- Persist the formatted log message via specific ILogPersister abstraction
- Define the log message level for persisting

### Usage
TBD


### Sample
The __sample application__ is stored in __sample__ folder. 
The `pio run -e nanonew -t upload` command is used for compiling and upload the __sample application__.

See the included examples and tests for further usage examples.

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### NuGet
### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.Logger-1.0.2110.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.Logger.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.Logger` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)