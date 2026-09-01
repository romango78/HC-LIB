# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.1.2609]
### Added
- _Error_, _ErrorCategory_, and _GenericError_ with category-based messages and _make_error()_.
- _Unexpected{E}_ and _make_error(E&&)_ so _Expected{T, E}_ can return a typed error.
- Arduino _std::remove_cv_ and _std::decay_ in _lib-utility.h_ so _make_error_ compiles on AVR.
- Unit tests for _Expected_, _lib-utility.h_ (_std::move_, _std::forward_, _std::decay_), and _ArduinoTimer_.

### Changed
- Renamed _move.h_ to _lib-utility.h_. Native builds include _\<utility\>_ and _\<type_traits\>_.
- A moved-from _Expected_ stays engaged and holds a moved-from _T_ or _E_.
- _ITimer::getInterval_ and _isStarted_ are const. _ArduinoTimer_ documents one-shot latch, restart, and ignored _setInterval_ while running.
- Sample _desktop-debug_ uses _build_type = test_ so _pio run_ generates Unity's _unity_config.h_ and links the test binary with debug symbols.

### Removed
- _errdef.h_ error constants. Use _GenericError_ instead. 

## [1.0.2404]
### Changed
- Added destructor, copy/move assignment, and move cleanup for __Expected\<T, E\>__ so the moved-from instance no longer destroys _T_.
- __Expected__ is parameterized by error type _E_ (defaults to _err_t_). The union always destroys the active member (_T_ or _E_).
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