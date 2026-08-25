# HC-LIB
## Arduino Device Abstraction Library v1.0.2404
This __library__ defines abstractions for devices that can be connected to Arduino Boards.

### Dependencies
- HC-LIB.System v1.0.2201

### Features
- Defines __IDevice__ with __DeviceCategory__ (__analog__, __digital__) and device type constants (__RELAY_DEVICE_TYPE__).
- Defines __IDeviceController{State, Device}__. __setState()__ returns __err_t__; __getState()__ returns __Expected{State}__.
- Defines __ISensor__, generic __SensorData{T}__ (sensor and value), and __ISensorReader{SensorData, Sensor}__.
- Read errors should be returned as __Expected{SensorData}__, not stored on __SensorData__.
- Defines device error constants (__TIMER_IS_NOT_INITIALIZED_DEVICE_ERROR__, __SENSOR_IS_NOT_INITIALIZED_DEVICE_ERROR__).

### Usage
Read a sensor through __ISensorReader__. Wrap the result in __Expected{T}__ when the read can fail (for example HC-LIB.ZMPT101B):
```c++
Expected<ZMPT101B_ACVoltage> result = rmsReader->read(*sensor);
if (result.hasValue())
{
    float voltage = result.getValue().data;
}
else
{
    err_t error = result.getError();
}
```

Control a device through __IDeviceController__ (for example HC-LIB.Devices):
```c++
err_t error = controller->setState(device, HIGH);
Expected<uint8_t> state = controller->getState(device);
if (state.hasValue())
{
    uint8_t value = state.getValue();
}
```

To add a custom sensor or device, derive from __ISensor__ / __IDevice__ and implement __ISensorReader__ or __IDeviceController__.

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.Device.Abstractions-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.Device.Abstractions.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.Device.Abstractions` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
