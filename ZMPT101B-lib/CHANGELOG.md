# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2609]
### Added
- Firmware stub and _extra_src_filter.py_ so `pio run` has `setup()` / `loop()` without packing them in `src/`.
- Unity tests use _unity_extensions.h_ / _F()_ from HC-LIB.System. Nano test builds shrink Serial buffers and drop Unity details to stay under 2 KB SRAM.

### Changed
- PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`, `firmware/`).
- Requires __HC-LIB.System__ and __HC-LIB.Devices__ 1.1.2609. _zero_ and _calibration_factor_ are _Q10_. The sensor constructor takes a float scale (default 1.0).
- _adjustZero_ is a Q10 EMA (weight 0.001). _isCloseToZero_ is ±10% of the mid-point.
- _ZMPT101BRmsReader_ is peak-to-peak × `1/(2√2)`. _ZMPT101BTrueRmsReader_ sums squares in `int64_t` and uses _Q10::sqrt()_. A negative root returns _GenericError::InvalidOperation_.
- Millivolts are `amplitude × calibration_factor × Vcc / 1024`, with Vcc cached from _board::getSupplyVoltage()_.
- _ZMPT101BRmsReader_ and _ZMPT101BTrueRmsReader_ return _Expected{ZMPT101B_ACVoltage, Error}_ (`uint32_t` millivolts). Both use the same sample window.
- RMS and True RMS live in separate `.cpp` files so unused True RMS can be dropped from flash.
- Samples live in __examples/ZMPT101B-ReadExampleApp__ and __examples/ZMPT101B-PotentiometerCalibrationApp__. The read example uses static objects, one heap stream the sensor owns, and one shared timer. The calibration sketch prints integers only.

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
