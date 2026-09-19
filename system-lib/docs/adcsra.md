# ADCSRA (ADC Control and Status Register A)

See [README.md](README.md). Related: [ADMUX](admux.md), [ADCSRB](adcsrb.md), [ADCL/ADCH](adc_data.md).

`ADCSRA` enables the ADC, starts conversions, auto-trigger, interrupts, and the clock prescaler.

---

## 📊 Register Bit Layout

The bit layout for `ADCSRA` is identical across all classic AVR chips (**ATmega328P/168, ATmega32U4, ATmega2560, ATmega644, and ATtiny85**):

| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
|  ADEN |  ADSC |  ADATE|  ADIF |  ADIE | ADPS2 | ADPS1 | ADPS0 |

---

## ⚙️ Core Bit Functions

### 1. ADC Enable (`ADEN`)
* **`0`**: **ADC is turned off**. Turning off the ADC saves power when analog sampling is not needed.
* **`1`**: **ADC is turned on**. Must be set to `1` before any analog conversion can take place.

### 2. ADC Start Conversion (`ADSC`)
* **In Single Conversion Mode**: Set this bit to `1` to manually start an analog-to-digital conversion. This bit will automatically switch back to `0` as soon as the hardware finishes the conversion.
* **In Auto Trigger / Free Running Mode**: Set this bit to `1` to initiate the very first conversion.
* **Polling Rule**: You can track when a conversion is done by polling this bit in a loop: `while (ADCSRA & (1 << ADSC));`.

### 3. ADC Auto Trigger Enable (`ADATE`)
* **`0`**: **Auto Trigger disabled**. Conversions are started manually by writing to `ADSC`.
* **`1`**: **Auto Trigger enabled**. The next conversion starts on the positive edge of the source selected by `ADTS` in [ADCSRB](adcsrb.md). **`ADTS = 000` is free-running only when `ADATE` is 1.** Setting `ADTS` to 000 without `ADATE` does not free-run.

### 4. ADC Interrupt Flag (`ADIF`)
* This bit is hardware-driven and automatically turns to `1` as soon as an ADC conversion completes and data registers are updated.
* If the ADC Interrupt is enabled (`ADIE = 1`), the chip executes the `ADC_vect` Interrupt Service Routine (ISR), and `ADIF` clears to `0` automatically.
* **Manual Clear**: If interrupts are not used, you can clear this flag manually by writing a logical `1` to it.

### 5. ADC Interrupt Enable (`ADIE`)
* **`0`**: ADC conversion complete interrupt is disabled.
* **`1`**: ADC conversion complete interrupt is enabled. Activates the `ISR(ADC_vect)` block every time a sample is ready.

### 6. ADC Prescaler Select (`ADPS2:ADPS0`)
These bits determine the division factor between the system clock frequency (e.g., 16 MHz on an Arduino Uno) and the input clock to the ADC hardware. 
> 💡 *Note: For maximum 10-bit accuracy, the AVR ADC architecture requires an input clock frequency between **50 kHz and 200 kHz**.*

| ADPS2 | ADPS1 | ADPS0 | Division Factor | ADC Clock (at 16 MHz System Clock) | Sampling Note |
| :---: | :---: | :---: | :-------------: | :--------------------------------: | :------------ |
|   0   |   0   |   0   |      **2**      | 8 MHz | Too fast (Low accuracy) |
|   0   |   0   |   1   |      **2**      | 8 MHz | Too fast (Low accuracy) |
|   0   |   1   |   0   |      **4**      | 4 MHz | Fast (Reduced resolution) |
|   0   |   1   |   1   |      **8**      | 2 MHz | Fast (Reduced resolution) |
|   1   |   0   |   0   |     **16**      | 1 MHz | Fast (Good for 8-bit tracking) |
|   1   |   0   |   1   |     **32**      | 500 kHz | Moderate |
|   1   |   1   |   0   |     **64**      | 250 kHz | Close to target spec |
|   1   |   1   |   1   |    **128**      | **125 kHz** | **Standard (Max 10-bit accuracy)** |

*Arduino's standard core library configuration defaults to a **128 prescaler** to maintain maximum accuracy.*

---

`board::getSupplyVoltage()` sets `ADEN` if needed, and sets prescaler 128 if ADPS is 0. It starts two conversions (discards the first after the mux change) and polls `ADSC`. It leaves the ADC enabled if the core already had it on.

---

## Example: interrupt-driven A0 (not System `src/`)

Host Arduino sketch: sample A0 in `ISR(ADC_vect)`. Not part of packaged System.

```cpp
volatile int latestValue = 0;

void setup() {
  Serial.begin(9600);

  // 1. Setup ADMUX: AVcc reference, Analog Pin A0
  ADMUX = (1 << REFS0);

  // 2. Setup ADCSRA:
  // Enable ADC (ADEN), Enable Interrupt (ADIE), Set Prescaler to 128 (ADPS2:0 = 111)
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // 3. Kick off the first manual conversion
  ADCSRA |= (1 << ADSC);
}

void loop() {
  // Print the latest value updated cleanly by the ISR in the background
  noInterrupts();
  int currentSample = latestValue;
  interrupts();

  Serial.println(currentSample);
  delay(500);
}

// ADC Conversion Complete Interrupt Service Routine
ISR(ADC_vect) {
  // Read low register first, or use the 16-bit macro 'ADC'
  latestValue = ADC; 
  
  // In single/interrupt mode, start the next conversion manually
  ADCSRA |= (1 << ADSC); 
}
```
