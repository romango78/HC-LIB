# Arduino EEPROM

A working guide to **EEPROM** on AVR Arduino boards (Nano / Uno / ATmega328P and similar). Use it for a few bytes that must **survive power-off**, not as extra RAM or a log file.

On these chips **Program** is flash, **Data** is SRAM, and EEPROM is a third, byte-addressed store. Native (`desktop`) builds have no EEPROM unless you emulate it; they do not prove AVR wear or timing.

Arduino’s official [Memory Guide](https://docs.arduino.cc/learn/programming/memory-guide/) and [EEPROM guide](https://docs.arduino.cc/learn/programming/eeprom-guide/) cover the library and examples. This document stays on **AVR EEPROM** (what a Nano actually has) and the practices those pages leave thin. SRAM is in [SRAM.md](SRAM.md); flash is in [FLASH.md](FLASH.md).

HC-LIB libraries do not use EEPROM. A sketch may, for calibration or last-known state.

---

## What EEPROM is

**EEPROM** (Electrically Erasable Programmable Read-Only Memory) is on-chip non-volatile storage. Values stay when the board is powered off — “a tiny hard drive,” as the official guide puts it.

It is **not** flash and **not** SRAM:

| Memory | ATmega328P | Survives reset | Survives sketch upload | Typical use |
|---|---|---|---|---|
| Flash | 32 KB (~30 KB app) | Yes | Replaced by the new `.hex` | Code, `F()` / `PROGMEM` |
| SRAM | 2 KB | No | Lost | Variables, heap, stack |
| EEPROM | **1 KB** (1024 B) | Yes | **Usually kept** | Settings, calibration, a magic/version byte |

The official memory guide calls flash a *type* of EEPROM. The practical difference: flash is erased in **pages** and holds the firmware; EEPROM is read and written by **byte** (or a small `put`/`get` of a struct) and is meant for data you change rarely.

| | EEPROM | Flash | SRAM |
|---|---|---|---|
| CPU execute | No | Yes | No |
| Byte write from a sketch | Yes | No (SPM / bootloader only) | Yes (but volatile) |
| Typical write endurance | **~100 000** / cell | **~10 000** / page | Unlimited |
| Typical write time | **~3.3 ms** / byte (AVR) | Upload only | Nanoseconds |
| Reads | Unlimited, relatively slow vs SRAM | Fast (instruction fetch) | Fast |

The official EEPROM guide’s opening line says “512 bytes.” That is the **ATmega168** (Duemilanove). **Uno / Nano (ATmega328P) is 1 KB.** Mega is 4 KB. Always use `EEPROM.length()`, do not hard-code 512 or 1024.

EEPROM is **not secure**. Anyone with the board can read it. Do not store passwords or keys.

---

## How EEPROM is organized

### Address map (ATmega328P)

EEPROM is its **own address space**: bytes `0` … `1023`. It is not mapped into SRAM (`0x0100`–`0x08FF`) and not into flash.

You name cells yourself. A typical sketch layout:

```
0x000  magic / version   (1–2 B)   detect blank or old layout
0x002  flags             (1 B)
0x003  calibration       (2–4 B)   e.g. ZMPT zero, gain
0x007  last RelayState   (1 B)
…      unused
0x3FE  CRC16 / CRC32     (optional)
```

There is no filesystem and no wear-leveling in hardware. If you write address `0` every second, **that cell** dies after ~100 000 writes (~27 hours). The rest of the array is still fine.

AVR EEPROM is written through `EEAR` / `EEDR` / `EECR`. The Arduino `EEPROM` class is included with the AVR core (`#include <EEPROM.h>`). You do not install a library for Nano / Uno.

SAMD / many ARM boards in the official memory guide have **no** EEPROM. They emulate it in flash (`FlashStorage`, `EEPROM.commit()`). Those APIs and wear rules are different. This document is for **real** AVR EEPROM.

---

## How it works at runtime

1. After reset, EEPROM still holds the last committed bytes. SRAM and registers do not.
2. `EEPROM.read(addr)` / `EEPROM[addr]` returns one byte. Reads do not wear the cell.
3. `EEPROM.write(addr, value)` programs that byte. On AVR this takes about **3.3 ms** and counts as one write cycle **even if the value did not change**.
4. `EEPROM.update(addr, value)` is `if (read != value) write`. Same 3.3 ms only when the byte differs. Prefer this.
5. `EEPROM.put(addr, obj)` / `EEPROM.get(addr, obj)` copy `sizeof(obj)` bytes. **`put` uses update semantics** (official `eeprom_put` example).
6. A new sketch upload **does not** wipe EEPROM on AVR unless you erase the chip or run a clear sketch. An old struct layout will still be there — hence a **version** byte.

The CPU stalls during a write. Do not write from an ISR. Do not write in a tight `loop()` “just to be safe.”

---

## Symptoms of misuse

- Settings vanish after months: a cell hit ~100 000 writes (`write` in `loop()`, or `write` instead of `update`).
- Sketch hangs for milliseconds every loop: 3.3 ms × N bytes.
- Garbage after `get`: no magic/version; you read an unwritten or old layout (official `eeprom_get` warns about `nan` / junk strings).
- Struct fields wrong: you stored a **pointer** or a type whose `sizeof` changed.
- “I used EEPROM to free SRAM” and the board is still out of RAM — EEPROM is not a heap.

EEPROM overflow is an **address** bug (`addr >= EEPROM.length()`), not a linker error.

---

## How to measure

There is no `avr-size` line for EEPROM. The array is 1024 B on ATmega328P whether you use it or not.

```c++
#include <EEPROM.h>

void setup()
{
    Serial.begin(115200);
    Serial.print(F("EEPROM length: "));
    Serial.println(EEPROM.length());
}
```

Use `EEPROM.length()` so the same sketch ports to Mega (4 KB) or older 168 (512 B). Addresses are powers of two; wrap with `addr &= EEPROM.length() - 1` if you ring-buffer (official examples).

To **inspect** contents, walk the array (official `eeprom_read`) or compute a CRC over `EEPROM[i]` (official `eeprom_crc`). A CRC change means the stored data changed or is corrupt.

`avr-size` / `avr-nm` measure flash and SRAM only. See [FLASH.md](FLASH.md) and [SRAM.md](SRAM.md).

---

## Best practices

### 1. Store only what must survive power-off

Good: calibration, a device id, last mode, a counter you increment on each **boot**.

Bad: ADC samples, log lines, String buffers, anything you already have in SRAM for the current run. The official memory guide: **do not offload SRAM into EEPROM**.

### 2. Prefer `update` / `put` over `write`

```c++
EEPROM.update(0, version);     // no wear if already version
EEPROM.put(2, calibration);    // multi-byte, update per byte
EEPROM.get(2, calibration);
```

`write` always wears the cell. Official note: **~3.3 ms** per programmed byte; skip that when the value is unchanged.

### 3. Never write in `loop()` unless you have a real ring buffer and a budget

```c++
void loop()
{
    EEPROM.write(0, analogRead(A0) / 4);   // dies in ~27 hours
}
```

The official `eeprom_write` / `eeprom_update` *examples* write from `loop()` to demonstrate the API. Do not copy that into a product. Write from `setup()`, from a “save” command, or when a value **changes** and you can debounce it.

Clearing the whole array (`for (i = 0; i < EEPROM.length(); i++) EEPROM.write(i, 0)`) costs 1024 writes × 3.3 ms ≈ **3.4 s** and 1024 cycles on every cell. Do it once in a factory sketch, not at every boot.

### 4. Version the layout

```c++
struct Settings
{
    uint8_t magic;      // e.g. 0xA5
    uint8_t version;    // increment when fields change
    uint16_t zero;
};

Settings s;
EEPROM.get(0, s);
if (s.magic != 0xA5 || s.version != 1)
{
    s.magic = 0xA5;
    s.version = 1;
    s.zero = 512;
    EEPROM.put(0, s);
}
```

Unwritten EEPROM is often `0xFF`, not zero. Do not treat `0xFF` as a valid calibrated zero unless you meant that.

### 5. `put` / `get` only POD that is safe to memcpy

`put` writes `sizeof(T)` bytes. Safe: `uint8_t`, `uint16_t`, `float`, a struct of those plus a `char name[10]`.

Unsafe: `String`, any pointer, a struct that contains a `DigitalStream*`. After reboot the pointer is meaningless.

Mind `sizeof` and padding. Use fixed-width types (`uint16_t`, not `int` if you ever move to ARM). Keep the struct packed or accept explicit padding in the version.

### 6. Use `EEPROM.length()`; stay in range

```c++
if (addr + sizeof(s) > EEPROM.length())
{
    // do not write
}
```

### 7. Optional CRC

If a brown-out can interrupt a `put`, store a CRC after the payload (official CRC example uses the EEPROM as an array). On read, recompute and fall back to defaults.

### 8. Do not emulate EEPROM on AVR flash

ATmega328P already has 1 KB EEPROM. FlashStorage / `EEPROM.commit()` is for SAMD and similar. Rewriting flash pages from a sketch wears the firmware array and can brick the board. See [FLASH.md](FLASH.md).

---

## Tips and tricks

**`EEPROM[i]`** acts like a byte array (`read` / `update` on assign). Official iteration examples increment every cell — that is a demo, not a pattern for `loop()`.

**One write per change.** Keep the live value in SRAM; `put` when the user confirms or when `setup()` finishes calibration (`ZMPT101B::calibrate` result, relay default).

**Wear-level a counter** if you must increment often: move the cell through a ring of N slots and keep an index. For a boot count, 100 000 writes is many years. For a 10 Hz logger, it is hours.

**`get` of a `float` on blank EEPROM** can print `nan` / `ovf` (official warning). Check magic first.

**Multi-byte integers** are little-endian on AVR. `put`/`get` keep that consistent on the same MCU. Do not mix raw `write` of a `uint16_t` without a defined byte order.

**Brown-out.** A reset mid-`put` can leave a half-updated struct. Version + CRC, or write the payload then the magic last.

**Upload keeps EEPROM.** A “broken” device after a firmware change is often an old layout. Bump `version` or run a clear sketch.

**`EEWE` / busy.** The core waits for the previous write. Back-to-back `write` is serial 3.3 ms each.

**Power.** Do not cut 5 V during a write. Use the BOD; do not write in the last milliseconds of a dying battery without a capacitor strategy.

**Native tests.** Host has no AVR EEPROM. Mock `get`/`put` or skip those tests on `desktop`.

---

## Arduino `EEPROM` API (AVR)

| Call | Bytes | Wears if value unchanged? | Notes |
|---|---|---|---|
| `read(addr)` | 1 | No | |
| `write(addr, v)` | 1 | **Yes** | ~3.3 ms |
| `update(addr, v)` | 1 | No | `write` only when different |
| `get(addr, obj)` | `sizeof(obj)` | No | |
| `put(addr, obj)` | `sizeof(obj)` | Per byte, update | Prefer for structs |
| `EEPROM[i]` | 1 | Update on assign | |
| `length()` | — | No | 1024 on ATmega328P |

```c++
#include <EEPROM.h>

void setup()
{
    uint8_t v = EEPROM.read(0);
    EEPROM.update(0, 42);

    uint16_t zero = 512;
    EEPROM.put(1, zero);
    EEPROM.get(1, zero);
}
```

---

## Common mistakes

| Mistake | What happens | Fix |
|---|---|---|
| `write` / `put` in `loop()` | Cell worn in hours–weeks | Write on change or in `setup()` |
| `write` instead of `update` | Wear even when unchanged | `update` / `put` |
| Treat EEPROM as SRAM | Slow, worn, still 1 KB | Keep working data in RAM |
| Hard-coded `1024` | Breaks on 168 / Mega | `EEPROM.length()` |
| No magic / version | Garbage after upgrade or first boot | Defaults when magic mismatches |
| `put` a pointer / `String` | Crash or junk after reset | POD only |
| `write` every analog sample | 3.3 ms stalls + wear | RAM buffer; EEPROM for the result |
| Clear EEPROM every `setup()` | 1024 cycles per boot | Clear once |
| Store secrets | Anyone can dump the chip | Do not |
| FlashStorage on Nano | Wrong tool | Real EEPROM |

---

## HC-LIB conventions

These libraries target the Nano and **do not** read or write EEPROM.

1. **Calibration in RAM.** `ZMPT101BSensor::zero` is an in-memory field from `ZMPT101B::calibrate`. Persist it in a **sketch** with `EEPROM.put` if you need it across power cycles — not inside the library.
2. **Relay state.** `RelayDeviceController` does not remember `On`/`Off` after reset. Persist in the sketch if required.
3. **Do not add EEPROM to a library `src/`** unless the device truly owns a settings block and the wear policy is documented.
4. **Native tests** stay EEPROM-free. Desktop has no 1 KB array and no 3.3 ms write.

---

## Other boards (from the official tables)

| Board | EEPROM | Notes |
|---|---|---|
| Nano / Uno (ATmega328P) | **1 KB** | This guide’s default |
| Duemilanove (ATmega168) | 512 B | Official “512 bytes” line |
| Nano Every (ATmega4809) | 256 B | Smaller than Nano |
| Mega 2560 | 4 KB | Same API, `length()` = 4096 |
| Leonardo / Micro (32U4) | 1 KB | |
| UNO WiFi Rev2 (4809) | 256 B | |
| SAMD / MKR / Nano 33 IoT | — | Emulate in flash if needed |
| Nano 33 BLE / Portenta | — | No AVR EEPROM |

Full Flash / SRAM / EEPROM numbers: [Arduino Memory Guide](https://docs.arduino.cc/learn/programming/memory-guide/).

---

## Checklist before you persist on a Nano

- [ ] The data must survive power-off (not just the current run)
- [ ] Live values stay in SRAM; EEPROM is a snapshot
- [ ] `update` / `put`, not unconditional `write`
- [ ] No EEPROM I/O in `loop()` except a designed, rate-limited save
- [ ] Layout has magic + version; blank `0xFF` is handled
- [ ] Struct is POD; no pointers
- [ ] `addr + sizeof` ≤ `EEPROM.length()`
- [ ] Optional CRC if a mid-write reset is possible
- [ ] Not used as extra RAM or a high-rate log
- [ ] Factory clear is a separate sketch, not every boot

---

## References

- [A guide to EEPROM](https://docs.arduino.cc/learn/programming/eeprom-guide/) — `read` / `write` / `update` / `put` / `get`, `length()`, CRC and iteration examples, ~3.3 ms and 100 000-cycle notes.
- [Arduino Memory Guide](https://docs.arduino.cc/learn/programming/memory-guide/) — EEPROM vs flash vs SRAM, do not offload SRAM, write endurance.
- [EEPROM library reference](https://www.arduino.cc/reference/en/libraries/eeprom/)
- [SRAM.md](SRAM.md) — working memory; EEPROM is not a substitute.
- [FLASH.md](FLASH.md) — firmware store; do not self-write flash for settings on AVR.
