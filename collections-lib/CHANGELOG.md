# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [1.1.2609]
### Added
- Unit tests for multi-byte __Queue{T}__ FIFO order and single-item __EnumeratorBase__ destruction.

### Changed
- Requires __HC-LIB.System__ 1.1.2609.
- __Queue{T}::dequeue__ and __peek__ return __Expected{T, Error}__ via __make_error(GenericError::InvalidOperation)__.
- __Queue{T}::enqueue__ returns __Error__ (__NoError__ or __OutOfMemory__).
- __Queue{T}__ and __EnumeratorBase{T}__ disable copy and move.
- __EnumeratorBase{T}__ starts reset so the first __moveNext()__ after create advances to the first element.

### Fixed
- __EnumeratorBase{T}__ destructor walked the list with __while (!m_current)__, leaking nodes or crashing on a single item.
- __Queue{T}::shift__ copied __m_count__ bytes instead of __m_count * sizeof(T)__.

## [1.0.2203]
### Added
- Added __Queue{T}__ class. Defines a first-in, first-out collection of objects.

## [1.0.2110]
### Added
- Added __IEnumerable{T}__ abstraction. Exposes the enumerator, which supports a simple iteration over a collection of a specified type.
- Added __IEnumerator{T}__ abstraction. Supports a simple iteration over a generic collection.
- Added __KeyValuePair{TKey, TValue}__ class. Defines a key/value pair that can be set or retrieved.
