# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.0.2203]
### Added
- Added generic _IStream{T}_ abstraction for reading and writing data.
- Added _BaseStream{T}_ class with stream mode, error tracking and cloning support.
- Added I/O error constants (_STREAM_NOTCREATED_IO_ERROR_, _STREAM_CLOSED_IO_ERROR_).
