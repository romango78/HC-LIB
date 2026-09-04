# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2609]
### Changed
- PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`).
- Requires __HC-LIB.Devices__ 1.1.2609.
- _ZMPT101BRmsReader_ and _ZMPT101BTrueRmsReader_ return _Expected{ZMPT101B_ACVoltage, Error}_.
- Sample moved to __examples/ZMPT101BExampleApp__.

## [1.0.2203]
### Changed
- Fixed issue with disposing resources.

## [1.0.2201]
### Changed
- The dependencies was changed.

## [1.0.2111]
### Added
- Added ZMPT101B device definitions.
- Added ZMPT101B RMS reader.
- Added ZMPT101B True RMS reader.
