# Arduino SRAM

A working guide to SRAM on AVR Arduino boards (Nano / Uno / ATmega328P and similar). Use it when a sketch or test image must stay under a few kilobytes of RAM.

On these chips **Program** is flash and **Data** is SRAM. Native (`desktop`) builds have no 2 KB limit and do not prove that a Nano will fit.

Arduino’s official [Memory Guide](https://docs.arduino.cc/learn/programming/memory-guide/) covers flash, SRAM, and EEPROM across AVR and ARM boards. This document stays on **AVR SRAM** (what HC-LIB must fit) and adds measurement and practices the official page leaves thin.

---

## What SRAM is

**SRAM** (Static Random-Access Memory) is the chip’s working memory. The CPU reads and writes it every cycle. It holds:

- global and `static` variables
- the heap (`new` / `malloc`)
- the call stack (locals, arguments, return addresses)

It is **volatile**: contents are lost on reset or power-off. It is **scarce**: an ATmega328P has **2048 bytes**. Flash is 32 KB; EEPROM is 1 KB. Those three memories are separate address spaces on AVR.

| Memory | ATmega328P | Survives reset | CPU can execute from it | Typical use |
|---|---|---|---|---|
| Flash (PROGMEM) | 32 KB (30 KB after bootloader) | Yes | Yes | Code, `F()` / `PROGMEM` data |
| SRAM | 2 KB | No | No | Variables, heap, stack |
| EEPROM | 1 KB | Yes | No | Small persistent settings |

AVR cannot fetch operands from flash the same way it fetches from SRAM. A `const char msg[] = "hello";` still lives in SRAM. Only `PROGMEM` / `F()` keep the bytes in flash; you copy or stream them when needed.

### Why AVR copies literals into SRAM (Harvard)

AVR is a **Harvard** machine: the CPU has one bus for **program** (flash) and another for **data** (SRAM). A C pointer is a data-space address. A string literal is therefore copied from flash into SRAM at startup so ordinary `char*` code can use it.

That is why `Serial.println("hello")` costs SRAM and `Serial.println(F("hello"))` does not. `F()` / `PROGMEM` keep the bytes on the program bus; `Print` and `pgm_read_*` fetch them one byte at a time.

ARM Cortex-M boards in the official guide are often **von Neumann** or hybrid: `static const` data can stay in flash without `PROGMEM`. Do not assume that on a Nano.

The official AVR picture also lists a `text` section. **`text` is code in flash**, not SRAM. SRAM is `.data`, `.bss`, heap, and stack.

---

## How SRAM is organized

### Address map (ATmega328P)

The first 256 bytes of the data space are not general SRAM:

| Range | Size | Role |
|---|---|---|
| `0x0000`–`0x001F` | 32 B | CPU registers R0–R31 |
| `0x0020`–`0x005F` | 64 B | I/O registers |
| `0x0060`–`0x00FF` | 160 B | Extended I/O |
| `0x0100`–`0x08FF` | 2048 B | Internal SRAM (`RAMEND` = `0x08FF`) |

Application SRAM is only that last 2 KB.

### Layout inside the 2 KB

From low address to high:

```
0x0100  ┌─────────────────┐
        │ .data           │  initialized globals / statics (copied from flash at start)
        ├─────────────────┤
        │ .bss            │  zeroed globals / statics
        ├─────────────────┤
        │ heap →          │  new / malloc, grows upward
        │                 │
        │   (free)        │
        │                 │
        │          ← stack│  locals, calls, ISRs, grows downward from RAMEND
0x08FF  └─────────────────┘
```

| Region | Who allocates | Lifetime | In `avr-size` “Data”? |
|---|---|---|---|
| `.data` | Linker | Whole run | Yes |
| `.bss` | Linker | Whole run | Yes |
| Heap | `new` / `malloc` | Until `delete` / `free` | No |
| Stack | Calls / locals | Until the function returns | No |

**`avr-size` Data = `.data` + `.bss` only.** A report of 700 B used does **not** include the stack or the heap. Leave headroom (often 300–600 B) for calls, interrupts, and `Serial`.

The Arduino IDE says the same thing in different words after a compile:

```text
Global variables use 1595 bytes (77%) of dynamic memory, leaving 453 bytes for local variables.
Low memory available, stability problems may occur.
```

**Dynamic memory** here is `.data` + `.bss`. **Leaving N bytes for local variables** is the remainder for stack and heap, not a measured stack size. The “Low memory available” warning is the IDE’s hint that you are near a collision. PlatformIO’s `avr-size` **Data** line is that same global total.

Startup (`crt` / Arduino `init`) copies `.data` from flash into SRAM and clears `.bss`. That copy is why every initialized global costs **both** flash (the initial image) and SRAM (the live copy).

### What the compiler puts where

| You write | Goes to |
|---|---|
| `int x = 1;` at file scope | `.data` (SRAM + flash image) |
| `int x;` at file scope | `.bss` (SRAM, zeroed) |
| `static int x;` inside a function | `.data` / `.bss` (SRAM for the whole run) |
| `int x;` inside a function | Stack (SRAM while the function runs) |
| `new int[n]` / `malloc` | Heap |
| `"hello"` passed to a C API | `.data` unless you use `F()` / `PSTR()` |
| `const int t[64] = {…};` on AVR | Still `.data` (SRAM). `const` is not PROGMEM on AVR |
| `const int t[64] PROGMEM = {…};` | Flash; read with `pgm_read_*` |
| `Serial.println(F("hello"));` | Flash; Arduino streams it without a permanent SRAM copy |

On ARM / ESP32, `const` often lives in flash automatically. **On AVR it does not.** That is the most common SRAM surprise when porting.

---

## How it works at runtime

1. Reset: stack pointer = `RAMEND`. `.data` is copied, `.bss` is zeroed.
2. `main` → Arduino `setup()` / `loop()`. Each call pushes a frame; each return pops it.
3. Interrupts (Timer0 for `millis()`, UART, etc.) push another frame on top of whatever is running.
4. `new` / `malloc` take bytes from the heap and walk upward. `delete` / `free` return them; the heap can **fragment**.
5. If the stack and heap meet, you get silent corruption: wrong values, reboots, hangs. There is no MMU and usually no hard fault.

The UART ISR writes the **Serial RX buffer** in SRAM. Default Arduino AVR buffers are **64 B RX** and **64 B TX** (128 B together) plus the `HardwareSerial` object.

Soft-float (`float` / `double` on AVR) uses extra stack and flash. `printf` float support (`printf_flt`) pulls a large converter and more stack. Integer math is cheaper.

---

## Symptoms of running out

- Random resets or a boot loop (watchdog or jump to `0`).
- `Serial` prints garbage, stops, or never appears.
- Variables change “by themselves” (stack smash / heap overwrite).
- `new` returns `nullptr` and a later dereference crashes.
- A function works until you add one more `Serial.println` or a local array.
- The sketch **compiles and uploads**, then halts or resets (official guide: resource hogging / not enough SRAM at run time).
- Tests pass on `desktop` and crash or fail to link/run on `nano-board`.

Treat SRAM as a budget. Measure after every feature that adds strings, buffers, or float.

---

## How to measure

Build the **same** ELF you will run. `pio run` and `pio test` overwrite `.pio/build/nano-board/firmware.elf`. Do not mix them in parallel (shared SCons cache).

### Static size (`.data` + `.bss`)

```powershell
pio run -e nano-board
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -C --mcu=atmega328p .pio\build\nano-board\firmware.elf
```

Unity test image (does not upload or run):

```powershell
pio test -e nano-board --without-uploading --without-testing
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -C --mcu=atmega328p .pio\build\nano-board\firmware.elf
```

`Data` must stay well under 2048. For HC-LIB Nano tests, keep it under **2 KB with stack headroom** — if Data is already ~1.2 KB+, the next `RUN_TEST` name or float assert can fail the link or the board.

Section breakdown:

```powershell
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -A .pio\build\nano-board\firmware.elf
```

### Largest SRAM symbols

`d` / `b` / `B` are RAM. `T` is flash (code).

```powershell
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-nm.exe" --size-sort --print-size -t d .pio\build\nano-board\firmware.elf
```

Look at the bottom of the list for the biggest `d`/`b`/`B` names: Serial buffers, Unity state, static `F()` copy buffers, large arrays.

### Runtime free estimate (approximate)

The official guide’s AVR helper (same idea as below) uses two linker symbols:

- `__heap_start` — first byte of the heap
- `__brkval` — last heap pointer (`0` if nothing has been `malloc`’d yet)

```c++
void display_freeram()
{
    Serial.print(F("- SRAM left: "));
    Serial.println(freeRam());
}

int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}
```

This is the gap between heap and stack **at that instant**. It is not a guarantee. ISRs and deeper calls use more. The official guide treats a board that is near **maximum capacity** as already unsafe. Do not ship a product that needs this number to be under ~200 B.

On ARM Arduino boards the official guide uses `sbrk(0)` instead (`&top - sbrk(0)`). That does not apply to the Nano.

---

## Best practices

### 1. Keep literals in flash

```c++
Serial.println(F("State: off"));   // flash
Serial.println("State: off");      // SRAM copy of the string
```

Use `F()` for `Serial`, logger format strings, and Unity `*_MESSAGE` text. Flash is larger than SRAM, but it is still finite — do not spam `Serial.println` “just in case.”

Use `PROGMEM` tables for lookup data (`#include <avr/pgmspace.h>`). Read with `pgm_read_byte` / `pgm_read_word` / `strcpy_P` / `strncpy_P`. Flash reads are **slower** than SRAM; keep hot, frequently mutated values in RAM.

On AVR, **`F()` / `PSTR()` cannot initialize a file-scope object**. Assign in `setup()`:

```c++
LogModule module;           // not LogModule(F("App")) at global scope
void setup()
{
    module = LogModule(F("App"));
}
```

### 2. Do not treat `const` as flash on AVR

```c++
const char table[] = "ABCDEF";          // SRAM
const char table[] PROGMEM = "ABCDEF";  // flash
```

### 3. Shrink Serial when lines are short

```ini
build_flags =
    -DSERIAL_RX_BUFFER_SIZE=16
    -DSERIAL_TX_BUFFER_SIZE=32
```

The official guide’s defaults in `HardwareSerial.h` are **64 TX + 64 RX** (128 B). Prefer `-D` build flags over editing the core header. Many sketches only print a few characters. Do not shrink so far that you drop RX bytes if the host bursts data. Third-party libraries often have their own buffers — size those too.

### 4. Prefer C strings and small buffers over `String`

`String` allocates on the heap, concatenates with temporaries, and fragments SRAM. Prefer `char buf[16]`, `snprintf`, or write pieces with `Serial.print`.

If a `String` is unavoidable and it grows, **pre-size it** so it does not reallocate (official `reserve()`):

```c++
String line;
line.reserve(32);
```

### 5. Stack for small locals; `.bss` for large or long-lived buffers

The official guide prefers the **stack** over the heap: a local is fragmentation-free and disappears when the function returns. That is the right default for small values.

Large arrays are the exception. A `char line[128]` in `loop()` sits on the stack on every call, on top of callees and ISRs:

```c++
void loop()
{
    char line[128];   // 128 B on the stack every call, plus callees
}
```

A `static char line[128];` or a file-scope buffer costs 128 B once. Do not put multi-hundred-byte arrays in recursive or interrupt paths. Do not keep globals for data that is only used inside one short function — that permanently raises the heap start.

### 6. Use the smallest type that holds the value

On AVR Arduino, `int` is 2 bytes and `double` is the same 4-byte format as `float` (official type table):

| Type | Bytes (AVR) | Typical range |
|---|---|---|
| `bool` / `boolean` | 1 | true / false |
| `char` / `int8_t` | 1 | −128 … 127 |
| `uint8_t` / `byte` | 1 | 0 … 255 |
| `int` / `int16_t` | 2 | −32768 … 32767 |
| `unsigned int` / `uint16_t` / `word` | 2 | 0 … 65535 |
| `long` / `int32_t` | 4 | ±2.1e9 |
| `unsigned long` / `uint32_t` | 4 | 0 … 4.2e9 |
| `float` / `double` | 4 | ±3.4e38 |

A pin, a flag, or an ADC sample does not need `int` or `long`. On ARM, `int` is often 4 bytes — do not copy AVR sizes to SAMD/nRF.

### 7. Avoid float unless you need it

AVR has no FPU. Soft-float bloats flash and stack. Prefer `uint16_t` ADC counts until the last step. If you must print floats, print with `Serial.print(value, 2)` rather than linking `printf_flt`.

Disable Unity float support when tests do not use `TEST_ASSERT_FLOAT_*`:

```ini
-DUNITY_EXCLUDE_FLOAT
-DUNITY_EXCLUDE_FLOAT_PRINT
-DUNITY_EXCLUDE_DETAILS
```

Keep float in Unity when the tests are the point (RMS, `VoltageStream`).

### 8. Own heap lifetime

Every `new` needs a matching `delete` (or a documented owner). Prefer a single `static` device/stream built in `setup()` over allocating in `loop()`. Repeated allocate/free of different sizes fragments the 2 KB heap.

### 9. Keep call depth shallow

Each frame costs a return address plus saved registers plus locals. Deep recursion and large `Expected<T, Error>` / fat structs by value add up. Pass pointers or references for large objects.

### 10. Do not pack `setup()` / `loop()` into a library `src/`

A consumer sketch already defines them. Extra definitions fail the link. Library firmware stubs belong outside the packaged `src/` (see HC-LIB `firmware/firmware_stub.cpp`).

### 11. Leave headroom

A useful rule for ATmega328P:

| Static Data (`avr-size`) | Risk |
|---|---|
| < 800 B | Comfortable for typical sketches |
| 800–1200 B | Watch stack and Serial |
| > 1200 B | Easy to collide; measure after every change |
| > 1800 B | Almost no stack; expect crashes |

Unity tests need more SRAM than a tiny stub (stub images in this repo are ~9 B Data / ~590 B flash). Judge the **test ELF** separately from the **stub ELF**.

---

## Tips and tricks

**Copy flash to a reusable RAM slot.** C APIs (`printf`, Unity) want a `const char*`. Copy once into a static buffer, then pass that pointer. The next copy overwrites it — do not keep the pointer across another flash-to-RAM call.

HC-LIB `flash.h` does this with `flash_c_str()` (`FLASH_C_STR_SIZE`, default 160). `unity_extensions.h` feeds `F("…")` messages through that helper and copies each `RUN_TEST` name into a 96-byte buffer so Unity does not keep every test name in SRAM.

**`F()` in a header is fine; a string table in SRAM is not.** Prefer one `F()` at the print site over a global `const char* messages[]`.

**Initialized vs zero.** `uint8_t buf[64] = {0};` at file scope is `.bss` (often). `uint8_t buf[64] = {1, 2, …};` is `.data` and also occupies flash for the image. Prefer zero-init when you overwrite the buffer anyway.

**Do not store pointers to stack.** Returning `&local` or stashing a local array for `loop()` to use later is a use-after-return.

**ISRs must be tiny.** They run on the current stack. No `String`, no `Serial.print` of long lines, no deep calls, no `new`.

**Linker garbage-collects unused `.o` functions**, not unused global arrays you referenced. If you `#include` a table, you pay for it.

**Two Serial objects** (e.g. `Serial` + software serial) double the buffers.

**`millis()` / `delay()`** themselves are cheap. The cost is everything you do between them.

**Print in pieces** instead of building one big line:

```c++
Serial.print(F("RMS="));
Serial.println(value, 2);
```

**Native `F()` is not flash.** On desktop, HC-LIB `F()` is a pass-through so the same source compiles. SRAM savings appear only on AVR.

**After repacking a PlatformIO tarball**, delete the consumer’s `.pio/libdeps` so the new headers (for example `unity_extensions.h`) are actually unpacked.

**Do not run `pio run` and `pio test` together** on the same env. They share `.pio/build` and can corrupt the image you then size.

**Watch `double`.** On AVR, `double` is the same 32-bit format as `float`, but some libraries still pull heavier math.

**Alignment.** AVR 8-bit loads are byte-oriented; padding is small. The usual waste is unused arrays and string literals, not alignment.

**EEPROM is not extra SRAM.** It is byte-addressable, slow, and **write-limited** (typical AVR endurance **~100 000 writes** per cell). Do not offload runtime SRAM into EEPROM, and do not `EEPROM.write` inside `loop()`. Reads are unlimited. See Arduino’s [EEPROM guide](https://docs.arduino.cc/learn/programming/eeprom-guide).

**Drop unused `#include`s and dead functions.** They cost flash first; some libraries also allocate SRAM buffers as soon as you construct an object (`WiFi`, `SD`, a second `Serial`).

**Keep literals short** when they must stay in SRAM. One `char` is one byte plus the `'\0'`.

---

## Common mistakes

| Mistake | What happens | Fix |
|---|---|---|
| `"long message"` everywhere | Each literal sits in `.data` | `F("long message")` |
| `const` tables without `PROGMEM` | Tables occupy SRAM | `PROGMEM` + `pgm_read_*` |
| Default Serial 64/64 | 128 B gone before your code | `SERIAL_*_BUFFER_SIZE` |
| Unity details + float + long names | Test image > 2 KB | `UNITY_EXCLUDE_*`, `F()` names/messages |
| `String` in a loop | Heap fragmentation | Fixed `char[]`; else `reserve()` |
| Large locals | Stack hits heap | `static` or smaller frames |
| `new` in `loop()` without `delete` | Heap grows until crash | Allocate once in `setup()` |
| Trusting `avr-size` Data = “free RAM” | Ignores stack/heap | Leave headroom; optional `freeRam()` |
| `EEPROM.write` in `loop()` | Wear-out, lost settings | Write rarely; EEPROM is not RAM |
| Global `F()` initializer | Does not compile or is wrong on AVR | Assign in `setup()` |
| `setup`/`loop` in a packaged library | Consumer sketch fails to link | Firmware stub outside `src/` |

---

## HC-LIB conventions

These libraries target the Nano. The same rules apply to any AVR sketch that uses them.

1. **Flash strings.** `F()` for logs, Serial, and Unity messages. Include `unity_extensions.h` **after** `unity.h` (and after `Log.h` if the logger defines its own native `F()`).
2. **Nano test flags.** Shrink Serial (`16` / `32`). Set `UNITY_EXCLUDE_DETAILS`. Exclude Unity float unless the tests use `FLOAT_WITHIN`.
3. **Measure the test image**, not only the empty stub. Stub size in this repo is typically **590 B flash / 9 B SRAM**. Tests are hundreds of bytes of Data.
4. **`test_build_src = yes`** when the library has its own `.cpp`. Do not also add `+<../test/tests_runner.cpp>` to `build_src_filter` (duplicate `setup` / `loop`).
5. **Inspect commands** are in each library README (`avr-size`, `avr-nm`).

---

## Other boards (from the official allocation table)

| Board | SRAM | Flash | EEPROM | Notes |
|---|---|---|---|---|
| Nano / Uno (ATmega328P) | 2 KB | 32 KB | 1 KB | This guide’s default |
| Nano Every (ATmega4809) | 6 KB | 48 KB | 256 B | Still AVR / Harvard |
| Mega 2560 | 8 KB | 256 KB | 4 KB | Same SRAM layout idea |
| Leonardo / Micro (32U4) | 2.5 KB | 32 KB | 1 KB | USB stack uses a slice |
| SAMD / MKR / Nano 33 IoT | 32 KB | 256 KB | — | Von Neumann; `static const` often in flash |
| Nano 33 BLE | 256 KB | 1 MB | — | Cortex-M4; not these PROGMEM rules |

Full Flash / SRAM / EEPROM numbers for more boards are in the [Arduino Memory Guide](https://docs.arduino.cc/learn/programming/memory-guide/).

---

## Checklist before you ship a Nano image

- [ ] `avr-size` Data leaves hundreds of bytes for stack
- [ ] `avr-nm` shows no surprise large `d`/`b` symbols
- [ ] User-facing strings use `F()` / `PROGMEM`
- [ ] Serial buffers match how you actually print / read
- [ ] No `String` in hot paths (`reserve()` only if you must keep `String`)
- [ ] Small locals on the stack; large buffers in `.bss`
- [ ] Types are no wider than the value needs
- [ ] Float / `printf` float only if required
- [ ] Heap allocations are one-shot or paired
- [ ] EEPROM is not used as scratch RAM
- [ ] Unity-only flags are not baked into consumer firmware
- [ ] You sized the **firmware** ELF and, if you run on-device tests, the **test** ELF

---

## References

- [Arduino Memory Guide](https://docs.arduino.cc/learn/programming/memory-guide/) — flash / SRAM / EEPROM, Harvard vs von Neumann, IDE output, `freeRam()`, `F()`, `PROGMEM`, `reserve()`, Serial buffers, EEPROM wear.
- [Arduino EEPROM guide](https://docs.arduino.cc/learn/programming/eeprom-guide/)
- [PROGMEM (Language Reference)](https://www.arduino.cc/reference/en/language/variables/utilities/progmem/)
- [String.reserve()](https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/reserve/)
- [avr-libc `<avr/pgmspace.h>`](https://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html)
