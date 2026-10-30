# ZMPT101B-ReadExampleApp

Prints RMS and True RMS millivolts from a ZMPT101B module on an Arduino Nano.

Use this sketch after the module trimmer is set and you already have a `calibration_factor`. It is the runtime demo: it does not search for a new factor.

## How it works

`setup()` opens Serial at 115200 and waits 2 seconds. It does not calibrate.

`loop()` calls `ZMPT101BRmsReader` and then `ZMPT101BTrueRmsReader` on the same `ZMPT101BSensor`. Both readers share one `ArduinoTimer`, and the calls do not overlap, so one timer is enough. Each read waits until the waveform is near mid-scale, samples four AC periods (80 ms at 50 Hz), and returns `Expected<ZMPT101B_ACVoltage, Error>` in millivolts.

The sensor is constructed with `ZMPT101B_CALIBRATION_FACTOR` (`0.275`). Replace that value with the factor printed by __ZMPT101B-SensorCalibrationApp__. Millivolts are `amplitude × calibration_factor × Vcc / 1024`. A successful pair is printed as `RMS_mV:<n>, TrueRMS_mV:<n>`. A failed read prints the error message instead of the number. The loop then waits 500 ms.

Wiring is the module 5V, GND, and OUT to Nano 5V, GND, and A0. Change `ZMPT101B_PIN` if OUT is on another analog pin. Mains on the screw terminals is hazardous.

From this folder:

```powershell
pio run -e nano-board -t upload
```

`platformio.ini` uses `/dev/ttyUSB0`. On Windows set that port to the COM port shown in Device Manager.
