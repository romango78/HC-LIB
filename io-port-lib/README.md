# HC-LIB
## IO Port Arduino Library v1.0.2203
This __library__ is a tool to help the programmer to work with I/O Ports on the Arduino Board.

### Dependencies
- HC-LIB.System v1.0.2203
- HC-LIB.IO-Abstraction v1.0.2203

### Features
- Defines generic __IPortAdapter{T}__ and __IPortStream{T}__ for wrapping Arduino port access.
- Defines __AnalogPortAdapter__ / __AnalogStream__ for analog ports (__uint16_t__).
- Defines __DigitalPortAdapter__ / __DigitalStream__ for digital ports (__uint8_t__), including port state.
- Defines __VoltageStream__ to read voltage and set PWM (pulse-width modulation) on a supported pin.

### Usage
Read analog data from a port:
```c++
IPortAdapter<int> *adapter = (IPortAdapter<int> *)new AnalogPortAdapter(A0);
IStream<uint16_t> *stream = (IStream<uint16_t> *)new AnalogStream(adapter);
stream->begin(StreamMode::Read);
uint16_t value = stream->read();
stream->end();
```

Read voltage and set PWM with __VoltageStream__:
```c++
IPortAdapter<int> *adapter = (IPortAdapter<int> *)new AnalogPortAdapter(A0);
IVoltageStream *stream = (IVoltageStream *)new VoltageStream(adapter);
stream->begin(StreamMode::Read);
float voltage = stream->getVoltage();

stream->begin(StreamMode::Write);
stream->setPwm(50);
```

Read or write a digital port:
```c++
IPortAdapter<uint8_t> *adapter = (IPortAdapter<uint8_t> *)new DigitalPortAdapter(3);
DigitalStream *stream = new DigitalStream(adapter);
stream->begin(StreamMode::Write);
stream->write(HIGH);
stream->begin(StreamMode::Read);
uint8_t value = stream->read();
```

Not all pins support PWM.

### Sample
The __sample application__ is stored in __sample__ folder. 
The `pio run -e nanonew -t upload` command is used for compiling and upload the __sample application__.

See the included examples and tests for further usage examples.

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.IO-Port-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.IO-Port.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.IO-Port` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
