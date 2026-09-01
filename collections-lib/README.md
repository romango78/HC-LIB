# HC-LIB
## Collections Arduino Library v1.0.2609
This __library__ defines generic collections for Arduino and native builds: __Queue{T}__, __IEnumerable{T}__ / __IEnumerator{T}__, and __KeyValuePair{TKey, TValue}__.

The PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`).

### Dependencies
- HC-LIB.System v1.1.2609

### Features
- Defines __Queue{T}__, a first-in, first-out buffer of trivially copyable elements. Copy and move are disabled. __dequeue__ / __peek__ return __Expected{T, Error}__; __enqueue__ returns __Error__.
- Defines __IEnumerable{T}__ and __IEnumerator{T}__. __EnumeratorBase{T}__ walks an owned linked list; the first __moveNext()__ after create or __reset()__ advances to the first element.
- Defines __KeyValuePair{TKey, TValue}__.

### Usage
Enqueue and dequeue with __Queue{T}__:
```c++
Queue<uint8_t> queue;
Error status = queue.enqueue(10);
if (status)
{
    // GenericError::OutOfMemory
}

Expected<uint8_t, Error> front = queue.peek();
Expected<uint8_t, Error> item = queue.dequeue();
if (item.hasValue())
{
    uint8_t value = item.getValue();
}
else if (item.getError() == GenericError::InvalidOperation)
{
    // queue was empty
}
```

Iterate with __IEnumerator{T}__:
```c++
IEnumerator<int> *enumerator = collection.getEnumerator();
while (enumerator->moveNext())
{
    int value = enumerator->getCurrent();
}
delete enumerator;
```

### Environments
Run commands from this folder (`collections-lib`).

| Environment | Platform | Purpose |
|---|---|---|
| `nano-board` | Arduino Nano (ATmega328, new bootloader) | Firmware: `setup()` / `loop()` from __test/tests_runner.cpp__ |
| `desktop` | native | Unity tests |
| `desktop-debug` | native | Unity tests with debug symbols (`-O0 -ggdb3`). Default. |

__test/tests_runner.cpp__ is the single entry point: a firmware stub when `UNIT_TEST` is off, and the Unity runner when it is on.

### Build
```powershell
pio run -e nano-board
pio run -e nano-board -t upload
pio run -e desktop-debug
```

Firmware is written to `.pio/build/nano-board/firmware.hex`. Default serial settings are `115200` baud on `COM3`.

### Unit tests
```powershell
pio test -e desktop-debug
```

Use `-e desktop` for a non-debug native run, or `-e nano-board` to run tests on the board. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details.

### Packages
Arduino Library Manager metadata is in [library.properties](library.properties). PlatformIO metadata is in [library.json](library.json).

* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package `HC-LIB.Collections-{version}.tar.gz` in the `{local_repo_folder}` folder.
* The `pio package publish {local_repo_folder}/HC-LIB.Collections-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.Collections.nuspec -outputdirectory {local_repo_folder}` command is used for creating NuGet package and store it in the `{local_repo_folder}` folder.
* The `nuget.exe install HC-LIB.Collections` command is used for installing NuGet package.

### Changelog
See [CHANGELOG.md](CHANGELOG.md)

### License
See [LICENSE.md](LICENSE.md)
