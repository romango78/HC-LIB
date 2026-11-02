# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.2.2611]
### Added
- __EmptySensor__ (__EMPTY_SENSOR_TYPE__, __DeviceCategory::None__). __SensorData{T}__'s default constructor stores that slice so an __Array__ slot can exist before a reading is assigned.

### Changed
- __ISensorReader__ reads one sensor into a caller-owned __SensorData*__, or a fixed collection of sensors into __SensorData*__ slots. __TSensor__ must inherit __ISensor__.
- __IDevice__ and __ISensor__ are trivially copyable. __SensorData{T}__ is trivially copyable when __T__ is.
- __IDevice__ type and category are private. Read them with __getType__ and __getCategory__. Assignment still replaces the whole value, so __Array__ can store devices.
- __SensorData{T}__ sensor and measurement are private. Read them with __getSensor__ and __getData__. Assignment still replaces the whole reading.
- Requires __HC-LIB.Collections__ 1.1.2611.

### Removed
- Public __IDevice__ fields __type__ and __category__.
- Public __SensorData__ fields __sensor__ and __data__.

## [1.1.2609]
### Added
- _DeviceError_ / _DeviceErrors.h_ (_TimerIsNotInitialized_, _SensorIsNotInitialized_).
- _device::is_analog_.
- PlatformIO project at the library root (`platformio.ini`, `src/`, `test/`, `firmware/`).
- Firmware stub and _extra_src_filter.py_ so `pio run` has `setup()` / `loop()` without packing them in `src/`.
- Unity tests use _unity_flash.h_ / _F()_ from HC-LIB.System. Nano test builds shrink Serial buffers and drop Unity float/details to stay under 2 KB SRAM.

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
