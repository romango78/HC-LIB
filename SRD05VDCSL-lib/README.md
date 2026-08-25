# HC-LIB
## Arduino SRD05VDCSL Library v1.0.2203
This __library__ defines classes for the SRD05VDCSL relay device that can be connected to Arduino Boards.

### Dependencies
- HC-LIB.Devices v1.0.2203

### Features
- Defines __SRD05VDCSLDevice__ as a __RelayDevice__ for the SRD05VDCSL module.
- Use __RelayDeviceController__ from HC-LIB.Devices to turn the relay __on()__ / __off()__ and read __Expected{RelayState}__.

### Usage
Connect the SRD05VDCSL module to a digital pin, then create the device and control it with __RelayDeviceController__:
```c++
IPortAdapter<uint8_t> *adapter = (IPortAdapter<uint8_t> *)new DigitalPortAdapter(SRD05VDCSL_PIN);
DigitalStream *stream = new DigitalStream(adapter);
SRD05VDCSLDevice device {SRD05VDCSL_PIN, stream};

RelayDeviceController controller;
controller.off(device);

Expected<RelayState> state = controller.getState(device);
if (state.hasValue())
{
    if (state.getValue() == RelayState::off)
    {
        controller.on(device);
    }
    else
    {
        controller.off(device);
    }
}
```

### Sample
The __sample application__ is stored in __sample__ folder. 
The `pio run -e nanonew -t upload` command is used for compiling and upload the __sample application__.

See the included examples and tests for further usage examples.

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.SRD05VDCSL-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.SRD05VDCSL.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.SRD05VDCSL` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
