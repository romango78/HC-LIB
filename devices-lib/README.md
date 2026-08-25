# HC-LIB
## Arduino Devices Library v1.0.2203
This __library__ defines base classes for devices that can be connected to Arduino Boards.

### Dependencies
- HC-LIB.Device.Abstractions v1.0.2201
- HC-LIB.IO-Port v1.0.2203

### Features
- Defines __AnalogSensor__ with a pin and an __IStream{uint16_t}__.
- Defines __AnalogDevice__, __DigitalDevice__, and __RelayDevice__ with a pin and a port stream.
- Defines __DigitalDeviceController__. __setState()__ returns __err_t__; __getState()__ returns __Expected{uint8_t}__.
- Defines __RelayDeviceController__ with __on()__, __off()__, and __getState()__ returning __Expected{RelayState}__.

### Usage
Create a digital device and set its state:
```c++
IPortAdapter<uint8_t> *adapter = (IPortAdapter<uint8_t> *)new DigitalPortAdapter(pin);
DigitalStream *stream = new DigitalStream(adapter);
DigitalDevice device {UNDEFINED_DEVICE_TYPE, pin, stream};

DigitalDeviceController controller;
err_t error = controller.setState(device, HIGH);
Expected<uint8_t> state = controller.getState(device);
if (state.hasValue())
{
    uint8_t value = state.getValue();
}
```

Control a relay:
```c++
RelayDevice relay {pin, stream};
RelayDeviceController relayController;
relayController.on(relay);
Expected<RelayState> state = relayController.getState(relay);
```

Create an analog sensor (for example for HC-LIB.ZMPT101B):
```c++
IPortAdapter<int> *adapter = (IPortAdapter<int> *)new AnalogPortAdapter(pin);
IStream<uint16_t> *stream = (IStream<uint16_t> *)new AnalogStream(adapter);
AnalogSensor sensor {VOLTAGE_SENSOR_TYPE, pin, stream};
```

### Sample
No Samples

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.Devices-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.Devices.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.Devices` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
