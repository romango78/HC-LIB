# HC-LIB
Arduino Libraries

## HOW TO ARTICLES
### Debug unit tests under __native__ platform
If you would like to debug unit tests under __native__ platform you need to use __GDB__ debugger. For this, you need to perform the following steps:
1) Adds a new configuration for launching __GDB__ debugger from VSCode.
2) Builds yours tests in the "debug" mode.
3) Runs __GDB__ debugger from VSCode.
##### Adding a new configuration for launching GDB debugger from VSCode.
You need to edit the __launch.json__ file, it's located in _.vscode_ folder. The example of added configuration is described below:
```
{
    "type": "cppdbg",
    "request": "launch",
    "name": "native-debug",
    "program": "<full path to the built application, usually it's located under ".pio\build\<environment>" folder and has "program.exe" name>",
    "args": [],
    "stopAtEntry": false,
    "cwd": "${workspaceFolder}",
    "environment": [],
    "externalConsole": false,
    "MIMode": "gdb"            
}
```
___Note:___ The __PlarforIO&reg;__ rewrites the __launch.json__ file over and over again. Pay attention to this.

##### Building unit tests project
Firstly, you need to change __PlatformIO&reg;__ configuration, so that the project will be built in the "__debug__" mode. Opens the __platformio.ini__ file and adds the "_build_type = debug_" option under your environment (see example below).
```
[env:desktop]
...
build_type = debug
...
```
