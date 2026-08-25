# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2404]
### Changed
- Added destructor, copy/move assignment, and move cleanup for __Expected\<T\>__ so the moved-from instance no longer destroys _T_.
- _ArduinoTimer_ initializes _m_startedAt_, does not elapse before _start()_, and uses a host clock on native builds.
- NuGet package now includes _ICloneable.h_ and _move.h_. Replaced vendored libstdc++ move/type_traits headers with a small local _std::move_ helper.
  
## [1.0.2203]
### Added
- Added definition of  _ICloneable_ interface. 

## [1.0.2201]
### Added
- Added _Expected<T>_ class for error handling.
- Added a new error code for "out of range" error.

### Changed
- The error definitions were renamed.

## [1.0.2111]
### Added
- Added error definitions.
- Added ITimer interface.
- Added ArduinoTimer class that is implemented ITimer interface using of Arduino __millis()__ function.

### Removed
N/A

### Changed
N/A