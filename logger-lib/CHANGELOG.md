# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2609]
### Changed
- PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`).
- _LogLevel_ is an _enum class_ (_Off_, _Critical_, _Error_, _Warn_, _Info_, _Debug_).
- _IDateTimeProvider::getLocalDatetime_ is _const_ and returns _DateTime_ by value.
- _SerialLogPersister_ is compiled only on Arduino.

## [1.0.2110]
### Added
- Added ILogPersister abstraction for persisting logger's messages.
- Added IDatetimeProvider abstraction for receiving current date and time.
- Added Log class that is encapsulated the logger functionality.
- Added LogManager sample class for injection the logger instance into other classes for logging messages
