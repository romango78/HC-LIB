# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.1.2609]
### Changed
- PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`).
- Requires __HC-LIB.System__, __HC-LIB.Device.Abstractions__, and __HC-LIB.IO-Port__ 1.1.2609.
- _DigitalDeviceController_ and _RelayDeviceController_ use _Error_ / _Expected{T, Error}_ and _IoError::StreamNotCreated_.
- _RelayState_ is an _enum class_ (_On_, _Off_).

## [1.0.2203]
### Changed
- Fixed issue with disposing resources.

## [1.0.2201]
### Added
- Added _AnalogSensor_ class.
- Added _AnalogDevice_ and _DigitalDevice_ class.
- Added _DigitalDeviceController_ and _RelayDeviceController_ classes.
- Added _Relay_ device type.