# HC-LIB
## Collections Arduino Library v1.0.2609
This __library__ defines generic collections for Arduino and native builds: __Queue{T}__, __IEnumerable{T}__ / __IEnumerator{T}__, and __KeyValuePair{TKey, TValue}__.

The PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`).

### Dependencies
- HC-LIB.System v1.1.2609

### Features
- Defines __Queue{T, CAPACITY}__, a fixed-size FIFO ring of trivially copyable elements (default __CAPACITY__ 16). Copy and move are disabled. Storage is a compile-time array (no malloc). __dequeue__ / __peek__ return __Expected{T, Error}__; __enqueue__ returns __bool__.
- Defines __IEnumerator{T}__. __getCurrent()__ is const and returns __Expected{T, Error}__ (__GenericError::InvalidOperation__ before the first item or after the last).
- Defines __EnumeratorBase{T, TKey}__, a key-based walker. The collection keeps storage; the derived type implements __getHead()__, __getNext()__, and __getByKey()__. Copy and move are disabled. After create or __reset()__, the first __moveNext()__ advances to the first element. After __moveNext()__ returns false, later calls stay false until __reset()__.
- Defines __IEnumerable{T}__. __getEnumerator()__ is const and returns __std::unique_ptr{IEnumerator{T}}__ (heap enumerator; the unique_ptr releases it).
- Defines __KeyValuePair{TKey, TValue}__. Constructed from const refs. __getKey()__ / __getValue()__ are const. The pair copies the key and value; it does not own pointed-to data.

### Usage
Enqueue and dequeue with __Queue{T, CAPACITY}__:
```c++
Queue<uint8_t, 16> queue;
if (!queue.enqueue(10))
{
    // queue is full
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

Iterate with __IEnumerable{T}__ / __IEnumerator{T}__:
```c++
std::unique_ptr<IEnumerator<int>> enumerator = collection.getEnumerator();
while (enumerator->moveNext())
{
    Expected<int, Error> current = enumerator->getCurrent();
    if (current.hasValue())
    {
        int value = current.getValue();
    }
}
```

Store a key and a value with __KeyValuePair{TKey, TValue}__:
```c++
KeyValuePair<const char*, int> pair("temp", 21);
const char* key = pair.getKey();
int value = pair.getValue();
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

### Inspect memory usage
A Nano has 32 KB flash and 2 KB SRAM. **Program** is flash; **Data** is SRAM (`.data` + `.bss`). Native builds have no 2 KB limit.

Firmware (`pio run -e nano-board`):
```powershell
pio run -e nano-board
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -C --mcu=atmega328p .pio\build\nano-board\firmware.elf
```

The test image uses the same ELF path (`pio test` overwrites it):
```powershell
pio test -e nano-board --without-uploading --without-testing
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -C --mcu=atmega328p .pio\build\nano-board\firmware.elf
```

Largest symbols (`d` / `b` / `B` are RAM; `T` is flash):
```powershell
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-nm.exe" --size-sort --print-size -t d .pio\build\nano-board\firmware.elf
```

Per-section breakdown:
```powershell
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -A .pio\build\nano-board\firmware.elf
```

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
