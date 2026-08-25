# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2404]
### Added
- Added dependencies on __HC-LIB.Device.Abstractions__ v1.0.2404 and __HC-LIB.System__ v1.0.2404.

### Changed
- _ZMPT101BRmsReader_ and _ZMPT101BTrueRmsReader_ now return __Expected\<ZMPT101B_ACVoltage\>__.

### Removed
- Removed the _ZMPT101B_ACVoltage_ error constructor. Read errors are returned via __Expected{T}__, not stored on __SensorData__.

## [1.0.2203]
### Changed
- Fixed issue with disposing resources.

## [1.0.2201]
### Changed
- Changed dependencies to __HC-LIB.Devices__ and __HC-LIB.IO__.

## [1.0.2111]
### Added
- Added _ZMPT101BSensor_ and _ZMPT101B_ACVoltage_ definitions.
- Added _ZMPT101BRmsReader_.
- Added _ZMPT101BTrueRmsReader_.
