# HC-LIB
## Collections Arduino Library __v1.0.2110__
This __library__ contains interfaces and classes that define various collections of objects, such as lists, queues, bit arrays, hash tables and dictionaries.

### Dependencies
No dependencies

### Features
- Defines generic IEnumerable{T} and IEnumerator{T} abstraction for supporting simple iteration over a generic collection.
- Defines a key/value pair that can be set or retrieved.

### Usage
TBD

### Sample
No Samples

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.Collections-1.0.2110.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.Collections.nuspec -outputdirectory {local_repo_folder}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.Collections` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)