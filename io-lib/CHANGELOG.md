# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2111]
### Added
N/A

### Removed
N/A

### Changed
- Refactored IVoltageStream interface and VoltageStream calss.
- Moved definitions from errdef.h to HC-LIB.System library.
- Refactored AnalogStream class, changed type of read/write value from __int__ to __uint16_t__.
- Refactored IPortAdapter interface and made it generic.

## [1.0.2110]
### Added
- Added IPortAdapter abstraction for wrapping Arduino build-in methods for working with Ports.
- Added IStream abstraction for streaming with I/O Arduino Ports.
- Added specific Port Adapter for analog port.
- Added specific Stream for analog port.

### Removed
N/A

### Changed
N/A