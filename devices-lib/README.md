# HC-LIB
## Arduino Devices Library v1.0.2203
This __library__ defines base classes for devices that can be connected to Arduino Boards.

### Dependencies
- HC-LIB.System v1.0.2203
- HC-LIB.IO     v1.0.2203
- HC-LIB.Device.Abstractions v1.0.2201

### Features
- Defines base classes for Sensors and Devices.
- Defines base classes for Device's Controllers.

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.Devices-1.0.2201.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.Devices.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.Devices` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)