# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2609]
### Added
- Firmware stub and _extra_src_filter.py_ so `pio run` has `setup()` / `loop()` without packing them in `src/`.
- Unity tests use _unity_extensions.h_ / _F()_ from HC-LIB.System. Nano test builds shrink Serial buffers and drop Unity float/details to stay under 2 KB SRAM.

### Changed
- PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`, `firmware/`).
- Requires __HC-LIB.Devices__ 1.1.2609.
- Example uses __RelayDeviceController__ with __Error__ / __Expected__ and __RelayState__ (__On__, __Off__).
- Sample moved to __examples/SRD05VDCSLExampleApp__.

## [1.0.2203]
### Added
- Added device definitions for _SRD05VDCSL_ relay.
