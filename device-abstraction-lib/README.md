# HC-LIB
## Arduino Device Abstraction Library v1.0.2111
This __library__ defines abstractions for devices that can be connected to Arduino Boards.

### Dependencies
- HC-LIB.System v1.0.2111
- HC-LIB.IO     v1.0.2111

### Features
- Defines abrstactions for Sensors.
- Defines abstractions for Sensor's Readers.

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.Device.Abstractions-1.0.2111.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.IO.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.Device.Abstractions` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)