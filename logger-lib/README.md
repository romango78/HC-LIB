# HC-LIB
## Logger Arduino Library v1.0.2110
This __library__ is a tool to help the programmer output log statements to a variety of output targets. 

### Dependencies
* printf.h - (c) Marco Paland (info@paland.com). Tiny printf, sprintf and snprintf implementation, optimized for speed on embedded systems with a very limited resources.

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### Sample
The `pio run -e nanonew -t upload` command is used for compiling and upload the __sample application__.

### NuGet
* The `nuget.exe pack Logger.Arduino.Library.nuspec -outputdirectory {local_repo}` command is sued for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install logger.arduino.library` command is used for installing NuGet package.
