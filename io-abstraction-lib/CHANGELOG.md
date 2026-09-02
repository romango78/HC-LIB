# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.1.2609]
### Added
- _IoError_ / _IoErrors.h_ (_StreamNotCreated_, _StreamClosed_).
- PlatformIO project at the library root (`platformio.ini`, `src/`, `test/`).

### Changed
- Requires __HC-LIB.System__ 1.1.2609.
- _StreamMode_ is an _enum class_ (_Read_, _Write_).
- _IStream::getLastError_ returns _Error_. _canRead_, _canWrite_, _hasError_, and _getLastError_ are const.
- _BaseStream_ stores last error as _Error_ and resets to _GenericError::NoError_.

### Removed
- _ioerrdef.h_ macros. Use _IoError_ instead.

## [1.0.2203]
### Added
- _IStream{T}_ and _BaseStream{T}_ abstractions.
