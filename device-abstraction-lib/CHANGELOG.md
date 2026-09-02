# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.1.2609]
### Added
- _DeviceError_ / _DeviceErrors.h_ (_TimerIsNotInitialized_, _SensorIsNotInitialized_).
- _device::is_analog_.
- PlatformIO project at the library root (`platformio.ini`, `src/`, `test/`).

### Changed
- Requires __HC-LIB.System__ 1.1.2609.
- _DeviceCategory_ is an _enum class_ (_Analog_, _Digital_).
- _IDeviceController::setState_ returns _Error_; _getState_ returns _Expected{State, Error}_. Device arguments are const references.
- _ISensorReader::read_ returns _Expected{SensorData, Error}_ and is const.
- _SensorData{T}_ holds only a successful reading. Errors are returned via _Expected_.

### Removed
- _deviceerrdef.h_ macros. Use _DeviceError_ instead.

## [1.0.2201]
### Added
- Added abstraction for Arduino Devices.
- Added abstraction for Arduino Device's controllers.
- Added general definitions for Arduino Devices.

### Removed
- Moved _AnalogSensor_ class to __HC_LIB.Devices__ library.
- Removed dependency on __HC-LIB.IO__ library.

## [1.0.2111]
### Added
- Added abstractions for analog Sensors.
- Added abstractions for Sensor's Readers.
