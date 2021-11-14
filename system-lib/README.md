# HC-LIB
## Arduino System Library v1.0.2111
This __library__ contains fundamental classes and base classes that define commonly-used data types.

### Dependencies
No Dependencies

### Features
- Defines error type and constants.
- Implemented countdown timer that uses either internal Arduino 16MHz clock or external DS3232 32kHz watch crystal.

### Usage
TBD

### Sample
The __sample application__ is stored in __sample__ folder. 
The `pio run -e nanonew -t upload` command is used for compiling and upload the __sample application__.

See the included examples and tests for further usage examples.

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.SYSTEM-1.0.2111.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.IO.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.SYSTEM` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)