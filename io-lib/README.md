# HC-LIB
## IO Arduino Library v1.0.2110
This __library__ is a tool to help the programmer to work with I/O Ports on the Arduino Board.

### Dependencies
No Dependencies

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### Sample
The __sample application__ is stored in __sample__ folder. 
The `pio run -e nanonew -t upload` command is used for compiling and upload the __sample application__.

### Packages
* The `` command is used for creating PlatformIO package.
* The `` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.IO.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.IO` command is used for installing NuGet package.