# AVR ADC registers (classic megaAVR / tinyAVR)

These notes describe the classic AVR ADC used by `board::getSupplyVoltage()` in [`src/board.h`](../src/board.h). They do not apply to megaAVR 0-series, ARM, or ESP.

Read in this order:

1. [ADMUX](admux.md) — reference (AVcc) and channel (1.1 V bandgap).
2. [ADCSRA](adcsra.md) — `ADSC` start and poll. `getSupplyVoltage()` sets `ADEN` if the ADC was off.
3. [ADCSRB](adcsrb.md) — clear `MUX5` on Mega / 32U4; Tinyx5 `REFS2`.
4. [ADCL / ADCH](adc_data.md) — read `ADCL` then `ADCH`.  
   \(V_{CC}\,\text{(mV)} = 1\,126\,400 / ADC\) (`READ_VCC_CALIBRATION_CONST` = \(1100\,\text{mV} \times 1024\)).

Code samples in these files are host-Arduino sketches (`setup` / `loop` / `Serial`). They are not part of packaged System `src/`.
