# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2202]
### Changed
- Fixed issue with disposing _PortAdapter_ resources.

## [1.0.2201]
### Added
- Added _DigitalPortAdapter_ class for reading/writing data to digital port and getting digital port state.
- Added _DigitalStream_ class for digital port.

## [1.0.2111]
### Changed
- Refactored _IVoltageStream_ interface and VoltageStream class.
- Moved definitions from errdef.h to HC-LIB.System library.
- Refactored _AnalogStream_ class, changed type of read/write value from __int__ to __uint16_t__.
- Refactored _IPortAdapter_ interface and made it generic.

## [1.0.2110]
### Added
- Added _IPortAdapter_ abstraction for wrapping Arduino build-in methods for working with Ports.
- Added _IStream_ abstraction for streaming with I/O Arduino Ports.
- Added specific Port Adapter for analog port.
- Added specific Stream for analog port.