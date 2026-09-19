# ADCSRB (ADC Control and Status Register B)

See [README.md](README.md). Related: [ADMUX](admux.md), [ADCSRA](adcsra.md), [ADCL/ADCH](adc_data.md).

`ADCSRB` holds the auto-trigger source (`ADTS`) and, on larger chips, **`MUX5`**.

---

## 📊 Register Bit Layout Comparison

| Architecture / Chip | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **ATmega328P / 168** *(Uno, Nano)* | — | ACME | — | — | — | ADTS2 | ADTS1 | ADTS0 |
| **ATmega32U4** *(Leonardo, Micro)* | ADHSM | ACME | MUX5 | — | — | ADTS2 | ADTS1 | ADTS0 |
| **ATmega2560 / 1280** *(Arduino Mega)* | — | ACME | MUX5 | — | — | ADTS2 | ADTS1 | ADTS0 |
| **ATmega644 / 1284** *(MightyCore)* | — | ACME | — | — | — | ADTS2 | ADTS1 | ADTS0 |
| **ATtiny85 / 45 / 25** *(Digispark)* | BIN | ACME | IPR | — | REFS2 | ADTS2 | ADTS1 | ADTS0 |

---

## ⚙️ Core Bit Functions (Standard Definition)

### 1. ADC Auto Trigger Source (`ADTS2:ADTS0`)
These bits select the trigger source for the ADC when Auto Triggering is enabled (by setting the `ADATE` bit in `ADCSRA`). The ADC will start a conversion on the positive edge of the selected interrupt signal:
* **`000`**: **Free Running Mode** — only if **`ADATE` is set** in [ADCSRA](adcsra.md). Without `ADATE`, `ADTS` is ignored.
* **`001`**: **Analog Comparator** interrupt.
* **`010`**: **External Interrupt Request 0** (`INT0`).
* **`011`**: **Timer/Counter 0 Compare Match A**.
* **`100`**: **Timer/Counter 0 Overflow**.
* **`101`**: **Timer/Counter 1 Compare Match B**.
* **`110`**: **Timer/Counter 1 Overflow**.
* **`111`**: **Timer/Counter 1 Capture Event**.

### 2. Analog Comparator Multiplexer Enable (`ACME`)
* **`0`**: Standard Analog Comparator operation.
* **`1`**: When the ADC is switched off (`ADEN` in `ADCSRA` is `0`), setting this bit replaces the negative input of the Analog Comparator with any of the selected analog input pins via `ADMUX`.

---

## 🛠️ Platform Specific Specifications & Extended Bits

### 1. ATmega2560 & ATmega32U4 (Extended Channels via `MUX5`)
* **Bit 5 – `MUX5` (ADC Multiplexer 5):** 
  * ADMUX already has **`MUX4:MUX0` (5 bits)**. `MUX5` in ADCSRB is the **sixth** bit.
  * **ATmega2560:** `MUX5 = 1` selects **A8–A15** and extra differential pairs.
  * **ATmega32U4:** `MUX5 = 1` selects **A6–A11** (D4, D6, D8, D9, D10, D12) and differential gains.
  * **`getSupplyVoltage()`** does `ADCSRB &= ~MUX5` so the bandgap encoding in ADMUX is not remapped (without this, Mega often returns a bogus result).

### 2. ATmega32U4 Exclusive (`ADHSM`)
* **Bit 7 – `ADHSM` (ADC High Speed Mode):**
  * **`0`**: Standard operational speed.
  * **`1`**: Enables high-speed ADC sampling. Must be set if the ADC clock frequency exceeds 200 kHz (up to 1 MHz for fast sampling requirements).

### 3. ATtiny85 / 45 / 25 Specialized Bits
* **Bit 7 – `BIN` (Bipolar Input Mode):** Enables bipolar conversion when using differential channels (supports both positive and negative voltage differentials).
* **Bit 5 – `IPR` (Input Polarity Reversal):** Flips the positive and negative inputs on differential channel pairs.
* **Bit 3 – `REFS2` (Internal Voltage Reference 2):** Works together with `REFS1:REFS0` in `ADMUX` to properly map the 2.56V internal voltage rail and toggle external capacitor connection on the AREF pin.

---

## Example: free-running A0 on ATmega328P (not System `src/`)

```cpp
void setup() {
  // 1. Setup ADMUX (AVcc ref, Analog Pin A0)
  ADMUX = (1 << REFS0);
  
  // 2. Setup ADCSRB (Explicitly set Free Running Mode -> ADTS[2:0] = 000)
  ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
  
  // 3. Setup ADCSRA (Enable ADC, Enable Auto Trigger, Start first conversion, set Prescaler to 128)
  ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void loop() {
  int adcResult = ADC; // Will constantly refresh automatically in the background
}
```
