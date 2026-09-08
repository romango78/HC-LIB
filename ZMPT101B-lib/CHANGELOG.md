# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2609]
### Added
- Firmware stub and _extra_src_filter.py_ so `pio run` has `setup()` / `loop()` without packing them in `src/`.
- Unity tests use _unity_extensions.h_ / _F()_ from HC-LIB.System. Nano test builds shrink Serial buffers and drop Unity details to stay under 2 KB SRAM. Float asserts stay enabled for RMS / True RMS.

### Changed
- PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`, `firmware/`).
- Requires __HC-LIB.Devices__ 1.1.2609.
- _ZMPT101BRmsReader_ and _ZMPT101BTrueRmsReader_ return _Expected{ZMPT101B_ACVoltage, Error}_.
- Samples live in __examples/ZMPT101B-ReadExampleApp__ and __examples/ZMPT101B-PotentiometerCalibrationApp__.

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
