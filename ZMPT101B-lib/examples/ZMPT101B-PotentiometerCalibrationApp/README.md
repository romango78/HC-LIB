# ZMPT101B-PotentiometerCalibrationApp

Shows the raw ADC span of a ZMPT101B module so you can set the onboard trimmer.

Use this sketch before software calibration. The trimmer sets the analog gain. This app does not compute `calibration_factor` and does not use the ZMPT101B readers. It only prints ADC counts so the sine wave uses most of the 0–1023 range without clipping.

## How it works

`setup()` opens Serial at 115200, sets the tracked maximum to 0, the tracked minimum to 1023, and the running mid-point `_zero` to 512. It then waits 2 seconds.

Each `loop()` reads A0 with `analogRead`. `_zero` is an exponential average of those samples (`ADC_COUNTS` is 1024, so each sample moves the mid-point by 1/1024 of the error). `ADJ_ADC` is the sample minus that mid-point. `MIN` and `MAX` keep the lowest and highest raw counts seen since reset. They are not cleared, so they show the lifetime peak of this run.

The line is `MAX`, `MIN`, `ADC`, `ZERO`, `ADJ_ADC`.

Wiring is the module 5V, GND, and OUT to Nano 5V, GND, and A0. Mains on the screw terminals is hazardous. Leave L/N disconnected until the idle reading is mid-scale.

## Calibrate the potentiometer on the board

The blue multi-turn trimmer sets the module gain. The sampling resistor on the PCB is fixed. This sketch only shows whether that gain uses the ADC range. It does not write `calibration_factor`.

`MIN` and `MAX` are the lowest and highest ADC counts since the Nano reset. They do not follow the latest turn of the screw. Press reset after each adjustment, then read the new span.

1. Upload this sketch and open the Serial Monitor at 115200.
2. Leave the mains screw terminals disconnected. `MIN` and `MAX` should both sit near 512. `ZERO` should settle near 512 (about 2.5 V on a 5 V board). If they do not, check 5V, GND, and OUT on A0 before connecting mains.
3. Connect mains to L and N. Fuse the circuit and do not touch the screw terminals. `MIN` and `MAX` move apart (for example `MIN` 400 and `MAX` 620).
4. Turn the trimmer with a small screwdriver, reset the Nano, and read `MIN` and `MAX` again.
5. Stop when `MAX` is about 850–900 and `MIN` is about 120–170. That uses most of the 10-bit range and leaves about 100 counts before 0 and 1023.
6. If either value reaches 0 or 1023, the sine wave is clipped. Turn the gain down, reset, and repeat until both ends stay inside that margin.

When the span is in range, find `calibration_factor` with __ZMPT101B-SensorCalibrationApp__.

From this folder:

```powershell
pio run -e nano-board -t upload
```

`platformio.ini` uses `/dev/ttyUSB0`. On Windows set that port to the COM port shown in Device Manager.
