# ADCL and ADCH (ADC Data Registers)

See [README.md](README.md). Related: [ADMUX](admux.md), [ADCSRA](adcsra.md), [ADCSRB](adcsrb.md).

These two 8-bit registers hold the 10-bit conversion result.

---

## ⚠️ The Golden Rule of Reading ADC Data
The AVR hardware includes an atomic latching mechanism to prevent data corruption while reading the 10-bit result across two separate 8-bit clock cycles:
1. **You MUST read `ADCL` first.** Reading `ADCL` locks both `ADCL` and `ADCH` data registers. This ensures that even if a new conversion completes while you are reading, the high byte will not be overwritten until you read it.
2. **You MUST read `ADCH` second.** Reading `ADCH` unlocks both registers, allowing the hardware to safely write the next conversion result.

> 💡 **Arduino C++ Shortcut:** In AVR-GCC (Arduino IDE), you can simply use the 16-bit virtual register macro **`ADC`** or **`ADCW`**. The compiler will automatically execute the low-byte-first read sequence in assembly under the hood.

---

## 🔄 Bit Layout Configurations (The Impact of `ADLAR`)

The way the 10-bit result is distributed across `ADCH` and `ADCL` depends entirely on the **`ADLAR`** (ADC Left Adjust Result) bit in the **`ADMUX`** register.

### Configuration A: Right-Adjusted (`ADLAR = 0`)
*This is the default setting used by standard Arduino cores (`analogRead`). It maps the standard 10-bit integer format (`0` to `1023`).*

#### **ADCH (High Byte)**

| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
|   -   |   -   |   -   |   -   |   -   |   -   | ADC9  | ADC8  |

#### **ADCL (Low Byte)**

| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| ADC7  | ADC6  | ADC5  | ADC4  | ADC3  | ADC2  | ADC1  | ADC0  |

* **Reading Code:** `int result = ADC;` (or sequentially read `ADCL` then `ADCH`).

---

### Configuration B: Left-Adjusted (`ADLAR = 1`)
*Used for high-speed systems or signal processing where 8-bit precision is sufficient. It allows you to discard `ADCL` entirely and save clock cycles.*

#### **ADCH (High Byte)**

| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| ADC9  | ADC8  | ADC7  | ADC6  | ADC5  | ADC4  | ADC3  | ADC2  |

#### **ADCL (Low Byte)**

| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| ADC1  | ADC0  |   -   |   -   |   -   |   -   |   -   |   -   |

* **Reading 8-bit:** `uint8_t fastResult = ADCH;`  
  The hardware **locks only when you read `ADCL`**. Reading **`ADCH` alone** does not latch and does not need an unlock. Use ADCL-then-ADCH (or the `ADC` macro) when you want all 10 bits.

---

`board::getSupplyVoltage()` reads `ADCL` then `ADCH` (right-adjust, `ADLAR = 0`) and computes  
`READ_VCC_CALIBRATION_CONST / result` millivolts. It does not use the `ADC` macro.

---

## Example: manual 10-bit read (not System `src/`)

```cpp
int readAnalogPinManual() {
  // 1. Start the conversion
  ADCSRA |= (1 << ADSC);
  
  // 2. Wait for completion (poll ADSC bit)
  while (ADCSRA & (1 << ADSC));
  
  // 3. Read registers explicitly in the required order
  uint8_t lowByte  = ADCL;  // Lock registers & snapshot low data
  uint8_t highByte = ADCH;  // Unlock registers & snapshot high data
  
  // 4. Combine into a standard 10-bit integer
  return (highByte << 8) | lowByte;
}
```
