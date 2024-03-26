# HC-LIB
Arduino Libraries

## Development Environment
The __Visual Code IDE__ should be installed with the following plugins:
* PlatformIO IDE v2.5.0 or higher
* C/C++ v1.9.2 or higher
* Code Spell Checker v2.2.3 or higher
* Native Debug v0.27.0 or higher
* Makefile Creator v0.0.3 or higher
* vscode-pdf v1.2.2 or higher

The g++ compiler should be installed locally. In case when compiler was not installed, performs the following steps:
1) Open PlatformIO Terminal window and execute `pio platform install windows_x86` command. This will install the required packages to compile C/C++ for host PC (Windows). It will install the minGW binaries in the following directory: `{HOME}\.platformio\packages\toolchain-gccmingw32\bin`, where {HOME} is where the current user directory is, for standard installations of PlatformIO.
2) Now this MinGW binary directory should be added to the environment path, so when the C++ compiler command g++ is called the system knows where it exists.
3) Run `g++ -v` command from Terminal to check if compiler has been installed successfully (restart VS Code before).
  
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
