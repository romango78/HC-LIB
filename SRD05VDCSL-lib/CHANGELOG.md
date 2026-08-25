# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2203]
### Changed
- Fixed issue with disposing resources. Added copy/move constructors and assignment for _SRD05VDCSLDevice_.
- _SRD05VDCSLDevice_ constructor now takes _DigitalStream_ instead of _IStream{uint8_t}_.

## [1.0.2202]
### Added
- Added _SRD05VDCSLDevice_ class for the _SRD05VDCSL_ relay.
