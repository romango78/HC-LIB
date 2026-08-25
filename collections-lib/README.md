# HC-LIB
## Collections Arduino Library v1.0.2203
This __library__ contains interfaces and classes that define various collections of objects.

### Dependencies
- HC-LIB.System v1.0.2201

### Features
- Defines generic __Queue{T}__ that represents a first-in, first-out collection of objects. __enqueue()__ returns __err_t__; __dequeue()__ and __peek()__ return __Expected{T}__.
- Defines generic __IEnumerable{T}__ and __IEnumerator{T}__ abstractions, and __EnumeratorBase{T}__, for simple iteration over a generic collection.
- Defines __KeyValuePair{TKey, TValue}__ that can be set or retrieved.

### Usage
Enqueue and dequeue items with __Queue{T}__:
```c++
Queue<uint8_t> queue;
queue.enqueue(1);
queue.enqueue(2);

Expected<uint8_t> result = queue.dequeue();
if (result.hasValue())
{
    uint8_t value = result.getValue();
}
else
{
    err_t error = result.getError();
}
```

Create a key/value pair:
```c++
char key[] = "Key";
KeyValuePair<char*, int> pair(key, 10);
char *name = pair.getKey();
int value = pair.getValue();
```

To iterate a custom collection, implement __IEnumerable{T}__ / __IEnumerator{T}__, or derive from __EnumeratorBase{T}__.

### Sample
No Samples

### Unit tests
The `pio test -e {environment}` command is used for running unit tests on the specified {environment}. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details

### Packages
* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package.
* The `pio package publish {local_repo_folder}/HC-LIB.Collections-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.Collections.nuspec -outputdirectory {local_repo}` command is used for creating NuGet package and store it in the {local_repo} folder.
* The `nuget.exe install HC-LIB.Collections` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
