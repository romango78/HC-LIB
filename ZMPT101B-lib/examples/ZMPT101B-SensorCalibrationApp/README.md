# ZMPT101B-SensorCalibrationApp

Finds `ZMPT101BSensor::calibration_factor` from a known mains voltage.

Use this sketch when the trimmer is already set and a multimeter on the same circuit shows the RMS voltage. The printed factor is the scale to copy into a sketch that reads the same module.

## How it works

Set `ETALON_VOLTAGE_MV` to the meter reading in millivolts before you upload. The default is `230000` (230 V RMS). The sensor is constructed with scale 1.

`setup()` opens Serial at 115200, waits 2 seconds, and prints the etalon. `ZMPT101B::calibrate` then takes one RMS window with `calibration_factor` forced to 1, so the reading is the unscaled module output. The stored factor is `etalon / measured`. `setup()` prints that factor as `FACTOR:` with four decimal places. If the stream is missing or the measured RMS is 0, the previous factor is left in place (here, 1).

`loop()` reads RMS with `ZMPT101BRmsReader` and prints `RMS (mV):` every second. After a successful calibration this value should stay near `ETALON_VOLTAGE_MV`.

Wiring is the module 5V, GND, and OUT to Nano 5V, GND, and A0. Mains on the screw terminals is hazardous.

From this folder:

```powershell
pio run -e nano-board -t upload
```

`platformio.ini` uses `/dev/ttyUSB0`. On Windows set that port to the COM port shown in Device Manager. This example links the ZMPT101B sources in the library root (`symlink://../..`), so it builds the current `calibrate` implementation.
