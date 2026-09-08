# Arduino Flash

A working guide to **flash** (program memory) on AVR Arduino boards (Nano / Uno / ATmega328P and similar). Use it when a sketch or test image must stay under the bootloader’s application limit.

On these chips **Program** is flash and **Data** is SRAM. Native (`desktop`) builds have no 30 KB limit and do not prove that a Nano will fit.

Arduino’s official [Memory Guide](https://docs.arduino.cc/learn/programming/memory-guide/) covers flash, SRAM, and EEPROM across AVR and ARM boards. This document stays on **AVR flash** (what HC-LIB must fit) and adds measurement and practices the official page leaves thin. SRAM is in [SRAM.md](SRAM.md).

---

## What flash is

**Flash** is the chip’s non-volatile program store. After reset the CPU fetches instructions from it. It also holds:

- compiled code (`.text`)
- the initial image of initialized globals (copied into SRAM `.data` at start)
- tables and strings you mark `PROGMEM` / `F()`
- the bootloader (top of the array)

It **survives reset and power-off**. You populate it at **upload** time. A sketch cannot treat it as extra RAM: on AVR a normal C pointer addresses SRAM, not flash.

| Memory | ATmega328P | Survives reset | CPU can execute from it | Typical use |
|---|---|---|---|---|
| Flash (PROGMEM) | 32 KB (~30 KB after bootloader) | Yes | Yes (code) | Firmware, `F()` / `PROGMEM` data |
| SRAM | 2 KB | No | No | Variables, heap, stack |
| EEPROM | 1 KB | Yes | No | Small persistent settings |

Flash is a kind of EEPROM (the official guide says this). The practical difference: flash is erased and written in **pages**; EEPROM is read/written by **byte**. Flash writes are slower and fewer (typical endurance **~10 000** erase/write cycles vs **~100 000** for EEPROM). Do not use flash as a settings store from `loop()`.

---

## How flash is organized

### Address map (ATmega328P)

Flash is word-addressed in the CPU (16-bit words) and byte-addressed in the linker map. Application space starts at `0x0000`. The bootloader sits at the high end.

| Region | Typical contents |
|---|---|
| Vectors | Reset and interrupt jump table |
| `.text` | Functions, including Arduino `setup` / `loop` and the core |
| PROGMEM / `__flash` | `F()` strings, `PROGMEM` tables |
| `.data` image | Initial values copied to SRAM at start |
| Bootloader | Optiboot / STK500; used for USB/UART upload |

PlatformIO `nanoatmega328new` reports **30720 bytes** of application flash (32 KB minus a 2 KB boot section in that board definition). An empty HC-LIB stub is about **590 B** program / **9 B** SRAM.

The official AVR picture lists a `text` section with SRAM’s `.data` / `.bss` / heap / stack. **`text` is flash.** SRAM is only `.data`, `.bss`, heap, and stack.

### What the compiler puts in flash

| You write | Flash? | SRAM too? |
|---|---|---|
| Functions (used) | Yes (`.text`) | No (except their stack frames at run time) |
| Unused functions in a `.o` that is linked | Often collected if `--gc-sections` | No |
| `int x = 1;` at file scope | Yes (initializer image) | Yes (live `.data`) |
| `int x;` at file scope | No | Yes (`.bss`, zeroed) |
| `"hello"` to a C API | Yes (then copied) | Yes (`.data` copy) |
| `Serial.println(F("hello"));` | Yes | No permanent copy |
| `const int t[] PROGMEM = {…};` | Yes | No (unless you copy it out) |
| `const int t[] = {…};` on AVR | Yes (image) | Yes (live array) |
| Bootloader | Yes (reserved) | No |

On ARM, `static const` often stays in flash without `PROGMEM`. **On AVR `const` is not flash.**

---

## How it works at runtime

1. Reset: PC = `0`. The vector table in flash jumps to the C runtime.
2. CRT copies the `.data` image from flash into SRAM and clears `.bss`.
3. `main` → Arduino `init` → `setup` / `loop`. Every instruction fetch is from flash.
4. `F()` / `PROGMEM` bytes stay on the **program bus**. `Print`, `pgm_read_byte`, `strcpy_P`, and `strncpy_P` pull them one byte (or word) at a time.
5. A Harvard pointer is SRAM. Casting a PROGMEM address to `const char*` and dereferencing it reads **the wrong memory**.

Flash is read-only for the sketch. Self-programming (SPM) exists for bootloaders; do not erase application pages from `loop()`. Wear the array only when you upload.

Soft-float, `printf` with float (`printf_flt`), C++ exceptions, RTTI, `String`, and fat libraries increase **`.text`**. They also cost SRAM (see [SRAM.md](SRAM.md)).

---

## Symptoms of running out

- Linker: `region 'text' overflowed by N bytes` / sketch too big.
- Arduino IDE: program storage at or over 100%; upload refused.
- PlatformIO: `Flash: [=======] 100%` then link failure.
- A feature “fits” on desktop and fails to link for `nano-board`.
- Upload succeeds but the image is so large you cannot add logging or tests later.

Flash overflow is usually a **link-time** failure. SRAM overflow is often a **run-time** crash. Treat them separately.

---

## How to measure

Build the **same** ELF you will run. `pio run` and `pio test` overwrite `.pio/build/nano-board/firmware.elf`. Do not mix them in parallel (shared SCons cache).

### Static size (`.text` + `.data` image)

`avr-size` **Program** is flash: `.text` + `.data` (the initializer image) + bootloader contribution in that ELF.

```powershell
pio run -e nano-board
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -C --mcu=atmega328p .pio\build\nano-board\firmware.elf
```

Unity test image (does not upload or run):

```powershell
pio test -e nano-board --without-uploading --without-testing
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -C --mcu=atmega328p .pio\build\nano-board\firmware.elf
```

Arduino IDE says the same thing as **program storage space** (example: “Sketch uses 14596 bytes (47%) of program storage space. Maximum is 30720 bytes.”).

Section breakdown:

```powershell
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -A .pio\build\nano-board\firmware.elf
```

### Largest flash symbols

`T` / `t` are code. `d` / `b` / `B` are RAM.

```powershell
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-nm.exe" --size-sort --print-size -t d .pio\build\nano-board\firmware.elf
```

Look at the largest `T` names: `printf`, soft-float helpers, Unity, `String`, unused Arduino modules you accidentally pulled in.

PIO also prints `Flash: used X from 30720` after `pio run -e nano-board`.

---

## Best practices

### 1. Put constants in flash on purpose

```c++
Serial.println(F("State: off"));          // flash only
Serial.println("State: off");             // flash image + SRAM copy

const uint16_t table[] PROGMEM = { 1, 2, 3 };
uint16_t x = pgm_read_word(&table[i]);
```

Use `F()` at print / log sites. Use `PROGMEM` for tables (`#include <avr/pgmspace.h>`). Flash reads are **slower** than SRAM; keep hot, mutated values in RAM.

On AVR, **`F()` / `PSTR()` cannot initialize a file-scope object**. Assign in `setup()`:

```c++
LogModule module;
void setup()
{
    module = LogModule(F("App"));
}
```

### 2. Do not treat `const` as flash on AVR

```c++
const char table[] = "ABCDEF";          // SRAM + flash image
const char table[] PROGMEM = "ABCDEF";  // flash only
```

On SAMD / many ARM boards, `static const` is enough (official guide). Not on a Nano.

### 3. Drop code you do not call

- Remove unused `#include`s and libraries. A constructed `WiFi` / `SD` / extra `Serial` pulls flash **and** SRAM buffers.
- Prefer one function with parameters over three copies (official “modular tasks”).
- Arduino AVR already uses `-ffunction-sections` and `--gc-sections`. A referenced global table is **not** collected.

### 4. Avoid flash-heavy libraries in the default path

Biggest typical pull-ins on AVR:

| Feature | Why it is expensive |
|---|---|
| Soft-float / `double` | No FPU; large helpers |
| `printf` + float (`printf_flt`) | Converter + stack |
| `String` | Heap + methods |
| C++ exceptions / RTTI | Tables and landing pads |
| Unity details + float asserts | Test image only — keep out of consumer firmware |

Print floats with `Serial.print(value, 2)` when you can. HC-LIB logger should not enable float converters unless the sketch needs them.

### 5. Initialized globals cost flash twice in spirit

`uint8_t buf[64] = { 1, 2, … };` stores the 64 bytes in flash **and** 64 bytes in SRAM. If you overwrite the buffer in `setup()`, use `.bss` (`uint8_t buf[64];` or `= {0}`) and fill it at run time.

`F()` / `PROGMEM` is the way to keep a **read-only** copy in flash only.

### 6. Do not use flash as EEPROM

Do not emulate a settings store by rewriting application pages. Use the 1 KB EEPROM (wear ~100 000 writes, not in `loop()`). The official guide’s FlashStorage / EEPROM-emulation libraries target boards **without** EEPROM (SAMD). ATmega328P already has EEPROM.

### 7. Keep the test image and the firmware image separate

`pio test` links Unity and every `RUN_TEST`. That flash cost is for the lab, not the product. Do not put `setup()` / `loop()` in packaged `src/` — a consumer sketch already has them (HC-LIB `firmware/firmware_stub.cpp`).

### 8. Leave headroom

A useful rule for ATmega328P application flash (30720 B):

| `avr-size` Program | Risk |
|---|---|
| < 15 KB | Comfortable for typical HC-LIB sketches |
| 15–24 KB | Watch the next library (float, SD, Unity) |
| > 24 KB | Little room for logging or on-device tests |
| Link overflow | Cut code or move data to PROGMEM / drop a library |

Judge the **firmware** ELF and the **test** ELF separately. Stubs in this repo are ~590 B flash; ZMPT101B Unity tests were ~20 KB.

---

## Tips and tricks

**`F()` vs `PSTR()` vs `PROGMEM`.** `F("…")` returns `__FlashStringHelper*` for `Print` / Arduino APIs. `PSTR("…")` is a `const char*` in program space for `*_P` functions. `PROGMEM` on an array is the general form. They all live in flash.

**C APIs cannot read PROGMEM.** `printf`, Unity, and tiny printf want a RAM `const char*`. Copy once into a static buffer, then call. The next copy overwrites it.

HC-LIB `flash.h` does this with `flash_c_str()` (`FLASH_C_STR_SIZE`, default 160). Logger copies a PROGMEM format into a stack buffer before `fctprintf`. `unity_extensions.h` copies each `RUN_TEST` name into a 96-byte buffer so names stay in flash until the test runs.

**PROGMEM arrays must be global or `static`.** A function-local `const char msg[] PROGMEM` is not reliable on AVR.

**`pgm_read_*` takes the address of the cell**, not a SRAM pointer:

```c++
const uint8_t kMap[] PROGMEM = { 10, 20, 30 };
uint8_t v = pgm_read_byte(&kMap[1]);   // 20
```

**`memcpy_P` / `strcpy_P` / `strncpy_P`** copy a block from flash to SRAM when you need a real `char*`.

**Do not store a `char*` to a `F()` literal** and later pass it to `strcmp`. The pointer is a program-space address. Use `strcmp_P` or `flash_c_str` first.

**Duplicate string literals.** The compiler may merge identical `"ok"` in `.data`. `F("ok")` at two sites may still emit two flash copies. Prefer one `PROGMEM` table if you print the same line often.

**LTO** (`-flto`) can shrink `.text` further. Arduino AVR cores vary; measure before and after.

**Virtual methods** put a vtable in flash and a vptr in each object (SRAM). Fine for a few devices; expensive as a forest of tiny classes.

**`inline` is not a flash guarantee.** The linker decides. Large `inline` in a header can duplicate `.text` in every TU if it is not actually inlined.

**Native `F()` is not flash.** On desktop, HC-LIB `F()` is a pass-through so the same source compiles. Size savings appear only on AVR.

**After repacking a PlatformIO tarball**, delete the consumer’s `.pio/libdeps` so new sources are actually compiled.

---

## Common mistakes

| Mistake | What happens | Fix |
|---|---|---|
| `"long message"` everywhere | SRAM copy of every literal | `F("long message")` |
| `const` tables without `PROGMEM` | Flash image **and** SRAM | `PROGMEM` + `pgm_read_*` |
| `char*` to a `F()` / `PSTR()` | Reads SRAM garbage | `Print` / `*_P` / `flash_c_str` |
| Local `PROGMEM` array | Unreliable placement | File-scope or `static` |
| `printf_flt` “to be safe” | Huge `.text` | `Serial.print(x, 2)` |
| Unused libraries still constructed | Flash + SRAM buffers | Remove includes / objects |
| Initialized 1 KB table you overwrite | Pays flash and SRAM | `.bss` + fill at run time |
| Self-write flash for settings | Wear, brick risk | EEPROM (rarely) |
| Trusting desktop size | Host has no 30 KB cap | Size the AVR ELF |
| `setup`/`loop` in packaged `src/` | Consumer fails to link | Firmware stub outside `src/` |

---

## HC-LIB conventions

These libraries target the Nano.

1. **Flash strings.** `F()` for logs, Serial, and Unity messages. Include `unity_extensions.h` **after** `unity.h` (and after `Log.h` if the logger defines its own native `F()`).
2. **Logger.** Built-in format and level names are `PROGMEM`. Formats are copied to a small RAM buffer because tiny printf cannot read flash.
3. **`flash_c_str`.** One reusable SRAM slot (`FLASH_C_STR_SIZE`). Do not keep the pointer across another flash-to-RAM call.
4. **Nano test flags.** Shrink Serial; drop Unity details (and Unity float unless tests need it). That saves **SRAM** first; Unity still costs flash.
5. **Measure both ELFs.** Stub ~**590 B flash / 9 B SRAM**. Tests are larger. `avr-size` / `avr-nm` commands are in each library README.
6. **Do not pack `setup`/`loop` in `src/`.** `extra_src_filter.py` adds `firmware/firmware_stub.cpp` for `pio run` only.

---

## Other boards (from the official allocation table)

| Board | Flash | SRAM | Notes |
|---|---|---|---|
| Nano / Uno (ATmega328P) | 32 KB (~30 KB app) | 2 KB | This guide’s default |
| Nano Every (ATmega4809) | 48 KB | 6 KB | Still AVR / Harvard |
| Mega 2560 | 256 KB | 8 KB | Same PROGMEM rules |
| Leonardo / Micro (32U4) | 32 KB | 2.5 KB | USB stack uses flash |
| SAMD / MKR / Nano 33 IoT | 256 KB | 32 KB | `static const` often enough |
| Nano 33 BLE | 1 MB | 256 KB | Not these PROGMEM rules |

Full numbers: [Arduino Memory Guide](https://docs.arduino.cc/learn/programming/memory-guide/).

---

## Checklist before you ship a Nano image

- [ ] `avr-size` Program leaves room for the next feature
- [ ] `avr-nm` shows no surprise large `T` symbols (`printf`, float, unused libs)
- [ ] User-facing strings use `F()` / `PROGMEM`
- [ ] No `char*` dereference of program-space addresses
- [ ] Initialized globals that you overwrite are `.bss` instead
- [ ] Float / `printf` float only if required
- [ ] Unused libraries are not linked
- [ ] Flash is not used as a settings store
- [ ] Unity-only code is not in the consumer package
- [ ] You sized the **firmware** ELF and, if you run on-device tests, the **test** ELF

---

## References

- [Arduino Memory Guide](https://docs.arduino.cc/learn/programming/memory-guide/) — flash vs SRAM vs EEPROM, Harvard, IDE “program storage”, `F()`, `PROGMEM`, unused sources, modular functions.
- [PROGMEM (Language Reference)](https://www.arduino.cc/reference/en/language/variables/utilities/progmem/)
- [avr-libc `<avr/pgmspace.h>`](https://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html)
- [SRAM.md](SRAM.md) — `.data` / `.bss` / heap / stack, `F()` SRAM savings, Serial buffers.
- [EEPROM.md](EEPROM.md) — 1 KB settings store; do not emulate EEPROM in flash on AVR.
