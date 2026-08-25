# HC-LIB
## IO Abstraction Arduino Library v1.0.2203
This __library__ defines abstractions for I/O streams on the Arduino Board.

### Dependencies
- HC-LIB.System v1.0.2203

### Features
- Defines generic __IStream{T}__ abstraction for reading and writing data.
- Defines __BaseStream{T}__ with stream mode, error tracking and cloning support.
- Defines I/O error constants (__STREAM_NOTCREATED_IO_ERROR__, __STREAM_CLOSED_IO_ERROR__).

### Usage
Use an __IStream{T}__ implementation (for example __AnalogStream__ / __DigitalStream__ from HC-LIB.IO-Port):
```c++
IStream<uint16_t> *stream = (IStream<uint16_t> *)new AnalogStream(adapter);
stream->begin(StreamMode::Read);
if (stream->canRead())
{
    uint16_t value = stream->read();
}
if (stream->hasError())
{
    err_t error = stream->getLastError();
}
stream->end();
```

To add a custom stream, derive from __BaseStream{T}__ and override `read()` / `write()` as needed.

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.IO-Abstraction-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.IO-Abstraction.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.IO-Abstraction` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
