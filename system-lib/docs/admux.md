# ADMUX (ADC Multiplexer Selection Register)

See [README.md](README.md) for the reading order. Related: [ADCSRA](adcsra.md), [ADCSRB](adcsrb.md), [ADCL/ADCH](adc_data.md).

---

## 📊 Register Bit Layout Comparison

| Architecture / Chip | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **ATmega328P / 168** *(Uno, Nano, Pro Mini)* | REFS1 | REFS0 | ADLAR | — | MUX3 | MUX2 | MUX1 | MUX0 |
| **ATmega32U4** *(Leonardo, Micro, Pro Micro)* | REFS1 | REFS0 | ADLAR | MUX4 | MUX3 | MUX2 | MUX1 | MUX0 |
| **ATmega2560 / 1280** *(Arduino Mega)* | REFS1 | REFS0 | ADLAR | MUX4 | MUX3 | MUX2 | MUX1 | MUX0 |
| **ATmega644 / 1284** *(MightyCore)* | REFS1 | REFS0 | ADLAR | MUX4 | MUX3 | MUX2 | MUX1 | MUX0 |
| **ATtiny85 / 45 / 25** *(Digispark)* | REFS1 | REFS0 | ADLAR | — | MUX3 | MUX2 | MUX1 | MUX0 |
| **ATtiny84 / 44 / 24** | REFS1 | REFS0 | MUX5 | MUX4 | MUX3 | MUX2 | MUX1 | MUX0 |

> ⚠️ **Note:** For chips with extended channels (e.g., Mega, Leonardo), the `MUX5` bit or additional configuration options are located in the secondary **ADCSRB** register.

---

## ⚙️ Core Bit Functions (Standard Definition)

### 1. Reference Voltage Selection (`REFS1:REFS0`)
Defines the upper limit voltage (the maximum value of `1023` in a 10-bit ADC). 
* **`00`**: **AREF pin** (External reference voltage, Internal reference turned off).
* **`01`**: **AVcc pin** (Default power rail connection, e.g., 5V or 3.3V).
* **`10`**: **Reserved** on ATmega328P / 168. On ATmega2560 / 644 / 1284 this is the **1.1 V** internal reference.
* **`11`**: **Internal reference** (1.1 V on 328P; 2.56 V on 2560 / 32U4 / 644 / 1284 — see per-chip notes).

### 2. ADC Left Adjust Result (`ADLAR`)
Controls the orientation of the 10-bit result across the 8-bit `ADCL` and `ADCH` registers.
* **`0`**: **Right-adjusted**. Standard 10-bit mode. Read `ADCL` first, then `ADCH` (or just read the 16-bit `ADC` macro).
* **`1`**: **Left-adjusted**. High 8 bits are shifted entirely into `ADCH`. Perfect for fast, lower-precision 8-bit applications where you only read `ADCH`.

### 3. Analog Channel Selection (`MUX4:MUX0`)
Selects which physical pin, internal sensor, or differential combination connects to the ADC.

---

## 🛠️ Platform Specific Specifications

### 1. ATmega328P / ATmega168 / ATmega8
* **Boards:** Arduino Uno, Nano, Pro Mini, LilyPad
* **Internal Reference (`11`):** 1.1V
* **Channel Selection (`MUX3:MUX0`):**
  * `0000` to `0101`: Pins **A0** to **A5** (`ADC0`–`ADC5`)
  * `0110` to `0111`: Pins **A6** and **A7** (Available only on Nano/Pro Mini TQFP packages)
  * `1000`: Internal Temperature Sensor
  * `1110`: Internal 1.1V Reference $(V_{BG})$
  * `1111`: Ground (**GND**)

### 2. ATmega32U4
* **Boards:** Arduino Leonardo, Micro, Pro Micro
* **Reference Voltage Configuration:**
  * `00`: AREF pin (external Vref, internal Vref off)
  * `01`: AVcc (with capacitor on AREF)
  * `10`: **Reserved**
  * `11`: Internal 2.56 V (with capacitor on AREF)
* **Extended Channels:** ADMUX has **`MUX4:MUX0`** (5 bits). **`MUX5` in ADCSRB** selects A6–A11 (on digital pins) and differential 10× / 40× pairs. Cap vs no-cap for 2.56 V on **ATtiny85** uses `REFS2` in ADCSRB, not these `REFS` codes.

### 3. ATmega2560 / ATmega1280
* **Boards:** Arduino Mega 2560, Mega ADK
* **Reference Voltage Configuration:**
  * `00`: AREF pin (External Vref)
  * `01`: AVcc pin
  * `10`: Internal 1.1V Reference
  * `11`: Internal 2.56V Reference
* **Extended Channels:** Single-ended **A0–A7** use `MUX4:MUX0` in ADMUX (5 bits). **A8–A15** and extra differential pairs need **`MUX5` in ADCSRB**. `getSupplyVoltage()` clears `MUX5` so the bandgap mux in ADMUX is not remapped.

### 4. ATmega644 / ATmega1284
* **Boards:** Custom DIP-40 target boards, MightyCore platforms
* **Internal references:** `10` = 1.1 V, `11` = 2.56 V.
* **Advanced Features:** Differential selections in `MUX4:MUX0`. Gain **1×, 10×, and 200×** on `ADC0`–`ADC7`.
* **`getSupplyVoltage()`:** `REFS0 | MUX4 | MUX3 | MUX2 | MUX1` (AVcc, bandgap).

### 5. ATtiny85 / ATtiny45 / ATtiny25
* **Boards:** Digispark ATtiny, bare chip applications
* **Reference Voltage Configuration:**
  * `00`: Vcc pin (Supply voltage)
  * `01`: AREF pin (External reference on Physical Pin PB5)
  * `10`: Internal 1.1V Reference
  * `11`: Internal 2.56V Reference *(Note: Has an extra `REFS2` configuration bit located inside `ADCSRB` for AREF capacitor routing)*
* **Channel Selection (`MUX3:MUX0`):** 4 single-ended inputs (`ADC0`–`ADC3`) and differential pairs with 1× or 20× gain.
* **`getSupplyVoltage()`:** `MUX3 | MUX2` (`1100`) selects the 1.1 V bandgap.

### 6. ATtiny84 / ATtiny44 / ATtiny24
* **Boards:** Bare DIP-14 / DIP-20, some tinyAVR cores
* **ADMUX:** `REFS1:0` plus **`MUX5:MUX0`** (no `ADLAR` in this register).
* **`getSupplyVoltage()`:** `MUX5 | MUX0` selects the 1.1 V bandgap (same combination as SecretVoltmeter / Arduino cores).

---

## 🚫 Non-AVR Platforms (Important Exception)
The **ADMUX register does not exist** on modern architectures such as:
* **megaAVR 0-series** (e.g., Arduino Every / ATmega4809)
* **tinyAVR 1-series** (e.g., ATtiny1614)
* **ARM-based boards** (e.g., Arduino Uno R4 / Renesas, SAMD21)
* **Espressif chips** (ESP32 / ESP8266)

These platforms use other registers (`ADCn.MUXPOS`, `ADCn.CTRLC`) or a HAL. Use `analogReference()` / `analogRead()` for portable sketches. `board::getSupplyVoltage()` returns **3300** (mV) on ARM and other non-AVR builds.

---

## How `board::getSupplyVoltage()` uses ADMUX

[`src/board.h`](../src/board.h) measures **AVcc** by converting the **1.1 V bandgap** with AVcc as the reference:

| Chip | ADMUX (plus notes) |
|---|---|
| ATmega328P / 168 | `REFS0 \| MUX3 \| MUX2 \| MUX1` → AVcc, channel `1110` (\(V_{BG}\)) |
| ATmega644 / 1284 | `REFS0 \| MUX4:1` |
| ATmega2560 / 32U4 | same mux; also `ADCSRB &= ~MUX5` |
| ATtinyx5 | `MUX3 \| MUX2` |
| ATtinyx4 | `MUX5 \| MUX0` |

Then: enable `ADEN` (prescaler 128 if the ADPS bits are 0), `delay(2)`, discard one conversion, convert again, read [ADCL then ADCH](adc_data.md),  
`millivolts = READ_VCC_CALIBRATION_CONST / ADC` (default \(1100 \times 1024 = 1\,126\,400\)). Restores ADMUX (and ADCSRB on Mega / 32U4). Turns the ADC off again only if it was off. Returns 0 if the ADC reading is 0.
