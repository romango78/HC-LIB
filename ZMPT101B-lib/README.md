# HC-LIB
## Arduino ZMPT101B Library v1.0.2609
This __library__ defines __ZMPT101BSensor__ and RMS / True RMS readers for the ZMPT101B AC voltage module.

The PlatformIO project is the library root (`platformio.ini`, `src/`, `test/`, `firmware/`).

### Dependencies
- HC-LIB.Devices v1.1.2609

---
### Features
- Defines __ZMPT101BSensor__ as an __AnalogSensor__ on an analog pin, with a calibrated ADC __zero__.
- __ZMPT101B::calibrate__ averages samples into that zero.
- __ZMPT101BRmsReader__ and __ZMPT101BTrueRmsReader__ return __Expected{ZMPT101B_ACVoltage, Error}__.
- Errors are __DeviceError::TimerIsNotInitialized__ and __IoError::StreamNotCreated__.

---
### Usage
1. Connect __ZMPT101B__ to an analog pin (see __How to connect device to Arduino board__).
2. Create a sketch. It should contain the following sections:
    - ZMPT101B sensor initialization in `setup()`:
    ```c++
    IPortAdapter<int> *adapter = new AnalogPortAdapter(ZMPT101B_PIN);
    IStream<uint16_t> *stream = new AnalogStream(adapter);
    ZMPT101BSensor *sensor = new ZMPT101BSensor(ZMPT101B_PIN, stream);
    ZMPT101B::calibrate(sensor);
    ```
    - Initialize the corresponding reader for RMS or True RMS:
        - for RMS:
        ```c++
        ITimer *timer = new ArduinoTimer();
        ZMPT101BRmsReader *rmsReader = new ZMPT101BRmsReader(timer);
        ```
        - or for True RMS:
        ```c++
        ITimer *timer = new ArduinoTimer();
        ZMPT101BTrueRmsReader *trueRmsReader = new ZMPT101BTrueRmsReader(timer);
        ```
    - Read data from the sensor:
        - RMS reader
        ```c++
        Expected<ZMPT101B_ACVoltage, Error> sensorData1 = rmsReader->read(*sensor);
        if (sensorData1.hasValue())
        {
            float volts = sensorData1.getValue().data;
        }
        else
        {
            Error error = sensorData1.getError();
        }
        ```
        - or True RMS reader
        ```c++
        Expected<ZMPT101B_ACVoltage, Error> sensorData2 = trueRmsReader->read(*sensor);
        if (sensorData2.hasValue())
        {
            float volts = sensorData2.getValue().data;
        }
        else
        {
            Error error = sensorData2.getError();
        }
        ```

---
### Device purpose
The __ZMPT101B__ is an isolated single-phase AC voltage sensor. A 2 mA : 2 mA voltage transformer and an onboard op-amp turn mains voltage into a 0–5 V analog waveform centered near mid-scale. The Arduino ADC samples that waveform; this library converts the samples to RMS or True RMS volts.

Use it for energy monitors, under/over-voltage detection, and other sketches that need a safe, isolated AC voltage reading.

---
### Device characteristics
Values below are typical for the common ZMPT101B breakout (confirm the marking on your board).

| Item | Typical value |
|---|---|
| Supply | 5 V DC |
| Transformer | ZMPT101B, 2 mA : 2 mA |
| Analog out | ~0–5 V, mid-point ~2.5 V (ADC ~512 at 10-bit) |
| AC input | screw terminals L / N (mains, isolated from the MCU) |
| Gain | onboard multi-turn potentiometer |
| Network frequency | 50 Hz (`AC_NETWORK_FREQUENCY`) |
| Sample window | two AC periods (40 ms at 50 Hz) |

Calibrate with no AC (or a known idle waveform) so __zero__ tracks the module mid-point. The onboard trimmer sets gain; the example's polynomial maps ADC offset from zero to volts.

### Voltage transformation
The **ZMPT101B** active voltage sensor module works in three main physical stages: **Current Limiting**, **Current Transformation**, and **Operational Amplification (I-V Conversion & Level Shifting)**.

The instantaneous output voltage $(U_\text{out})$ sent to the Arduino's analog pin based on the instantaneous input mains voltage $(U_\text{in})$ is expressed as:
$$
U_\text{out} = U_\text{offset} + (U_\text{in} \cdot \frac{R_\text{sampling}}{R_\text{in}} \cdot K_\text{t} \cdot G_\text{op\_amp})
$$

where:
- $U_\text{in}$ (**Instantaneous Input Voltage**): The high AC voltage you are measuring (e.g., the real-time wave value of 220V/110V AC).
- $R_\text{sampling}$ (**Sampling / Feedback Resistor**): A resistor that converts the secondary output current back into a small voltage signal.
- $R_\text{in}$ (**Input Current-Limiting Resistor**): A high-resistance resistor (typically 820 kΩ or a combination of resistors on the board) connected in series with the transformer primary winding. It converts the high input voltage into a safe, micro-level current.
- $K_\text{t}$ (**Turns Ratio / Transformation Coefficient**): The transformation ratio of the ZMPT101B toroidal transformer. It is engineered for a 1:1 current ratio (typically 2mA : 2mA).
- $G_\text{op\_amp}$ (**Gain of the Operational Amplifier**): The amplification factor of the onboard LM358 op-amp. This factor is adjustable using the multi-turn blue potentiometer (trimmer) on the module to prevent signal clipping.
- $U_\text{offset}$ (**DC Offset Voltage**): Since microcontrollers cannot natively measure negative AC voltages, the op-amp introduces a DC bias. For a 5V Arduino, it shifts the signal center up to 2.5V $(V_\text{CC} / 2)$.

---
### Analog-to-Digital Converter (ADC)
The microcontroller on a standard Arduino board features an internal **10-bit Successive Approximation Analog-to-Digital Converter (ADC)**.

The primary role of this ADC when paired with the ZMPT101B is to convert the continuous, level-shifted analog AC sine wave coming from the sensor into discrete digital integer steps that the microprocessor can manipulate mathematically.

#### Core Specifications
- **Resolution:** 10-bit. This provides a digital range of 2¹⁰ = 1024 discrete steps, yielding values from **0 to 1023**.
- **Conversion Time:** By default, a single standard analog conversion takes about 100 microseconds. This translates to a sampling rate of roughly 9.6 kHz to 10 kHz (approx. 190–200 samples per single 50 Hz AC cycle), which is more than sufficient for high-accuracy True RMS calculations.
- **Input Impedance:** The ADC circuitry is optimized for analog signal sources with an output impedance of 10 kΩ or less.

#### Governing Equations
1. **ADC Resolution Step (Step Size / LSB Value)**  
    The smallest change in analog voltage that the 10-bit ADC can resolve depends entirely on the reference voltage $(U_\text{ref})$. Assuming a standard 5.0V system:
$$
Step\_Size = \frac{U_\text{ref}}{1024} = \frac{5.0\text{\ V}}{1024} \approx 0.00488\text{\ V}\ \ \ (4.88\text{\ mV\ per\ step})
$$
2. **Analog-to-Digital Encoding (Forward Transfer)**  
    When an analog voltage $(U_\text{in})$ is applied to an analog input pin (e.g., A0), the ADC converts it into an integer code according to the formula:
$$
ADC_\text{code}=\text{round}\left(\frac{U_\text{in}}{U_\text{ref}}\times 1023\right)
$$
3. **Firmware Voltage Reconstruction (Inverse Transfer)**  
    To reconstruct the voltage value inside your Arduino C++ code from the raw integer read by `analogRead()`:
$$
U_\text{in}=\frac{ADC_\text{code}}{1023.0}\times U_\text{ref}
$$

#### ADC Voltage Reference Configurations $(U_{ref})$
The reference voltage acts as the absolute measuring ceiling for the system. In Arduino hardware, this can be altered using the `analogReference()` function:
- **DEFAULT:** The reference voltage matches the board’s operating voltage (typically 5V or 3.3V). *Limitation*: If your Arduino is powered via a computer USB cable, this 5V rail frequently sags to ~4.7V, causing the calculated AC voltage to fluctuate.
- **INTERNAL**: Invokes an onboard, highly stable 1.1V bandgap reference (available on ATmega328P). It ignores system power fluctuations entirely but limits your maximum measurable analog input voltage to 1.1V.
- **EXTERNAL**: Allows the user to supply a precise, low-noise external reference voltage (e.g., using a 4.096V or 5.000V precision shunt regulator) directly to the **AREF** pin for maximum absolute accuracy.

---
### How to connect device to Arduino board
The example uses a ZMPT101B module on an Arduino Nano (ATmega328, new bootloader). Control-side wiring:

| Module pin | Arduino Nano | Role |
|---|---|---|
| 5V / VCC | 5V | Module supply |
| GND | GND | Common ground |
| OUT | A0 | Analog sample (`ZMPT101B_PIN` in the example) |

AC side (screw terminals, isolated from the MCU):

| Terminal | Role |
|---|---|
| L | Line of the voltage under test |
| N | Neutral of the voltage under test |

![ZMPT101B & Arduino](./docs/ZMPT101B-Arduino.jpg)
![ZMPT101B pinout](./docs/ZMPT101B-Pinout.jpg)

Mains on L/N is hazardous. Keep that wiring away from the Nano. Fuse the measured circuit and do not touch the screw terminals while they are live.

1. Wire 5V, GND, and OUT as in the table. Leave L/N disconnected until the analog output looks mid-scale on A0.
2. Adjust the trimmer so a known AC voltage matches the printed RMS (optional).
3. Upload __examples/ZMPT101B-ReadExampleApp__ (`pio run -e nano-board -t upload`). Serial is `115200` on `COM3`.
4. The sketch calibrates, then prints `220V`, RMS, and True RMS every 500 ms.

To use another analog pin, change `ZMPT101B_PIN` and move the OUT wire to that pin.

---
### Hardware Calibration Guide for ZMPT101B
Although the physical **Sampling / Feedback Resistor** is fixed on the PCB, you can easily adjust the overall board gain using the onboard blue trimming potentiometer (pot). This scales the output AC sine wave to utilize the maximum range of the Arduino ADC without distortion.
- **Step 1: Upload a Peak-Detector Sketch**  
    Upload a simple Arduino script that continuously tracks and prints the minimum (MIN) and maximum (MAX) raw ADC values (0 to 1023) over a 1-second window.
- **Step 2: Check the Zero-Voltage Offset**  
    With the 220V mains disconnected, check the Serial Monitor. The MIN and MAX values should be nearly identical and settle around 512 (the 2.5V DC offset midpoint).
- **Step 3: Connect Mains and Adjust the Potentiometer**  
    Carefully connect the 220V AC input. The values will expand outward (e.g., MIN: 400 | MAX: 620). Use a small screwdriver to turn the blue potentiometer screw:
  - **Increase the gain** until the MAX value reaches roughly **850 to 900** and the MIN value drops to **120 to 170**.
  - **Avoid Signal Clipping:** Never let the values hit exactly **0 or 1023**. Leaving a safety margin of ~100 ADC steps ensures the sensor can measure unexpected voltage spikes without flattening the peak of the sine wave.

Once adjusted, the hardware is optimally scaled, and you can proceed with software True RMS calculations.

### AC Voltage Calculations
#### Overview
When measuring Alternating Current (AC) voltage via a sensor with a DC offset (such as the ZMPT101B), two primary methods are used to determine the effective voltage: **Standard RMS (Peak-based)** and **True RMS (Sample-based)**. The choice of method directly impacts measurement accuracy depending on the waveform's distortion.

| Feature | Standard RMS (Peak-to-RMS) | True RMS
|---|---|---|
| Waveform Support | Pure sinusoidal waves only (50/60 Hz grid utility). | Any waveform shape (pure/distorted sine, square, triangle, PWM). |
| Accuracy | High for ideal grids; very low if harmonics or dimmers are present. | High across all operating conditions. |
| Computational Cost | Minimal (requires tracking peak value and 1 multiplication). | Moderate to High (requires continuous sampling, squaring, and square root). |

#### Standard RMS (Peak-to-RMS Method)
This method derives the effective voltage strictly from the peak value of the waveform. It assumes the input signal is a mathematically perfect sine wave.
##### Mathematical Definition
For a pure sine wave, the relationship between the peak amplitude $(U_\text{peak})$ and the effective value $(U_\text{RMS})$ is a fixed constant:
$$
U_{RMS}=\frac{U_{peak}}{\sqrt{2}}\approx U_{peak}\times 0.7071
$$
##### Calculation Algorithm
- **Find the Peak**: Scan the discrete ADC samples over at least one full cycle to find the maximum raw value $(U_{max})$.
- **Remove DC Offset**: Subtract the zero-line offset $(U_{offset})$ to find the true peak amplitude:
$$
U_{peak} = U_{max} - U_{offset}
$$
- **Scale to RMS**: Multiply by the sinusoidal form factor and the sensor calibration coefficient $(K_{calib})$:
$$
U_{RMS} = (U_{max} - U_{offset})\times 0.7071\times K_{calib}
$$
*Warning: If the waveform contains harmonics, noise, or is chopped by a triac/dimmer, this method produces significant measurement errors.*

#### True RMS (Continuous Integration Method)
True RMS measures the actual power dissipation capability of the signal, regardless of the wave shape. It fundamentally follows the physical definition of heating power equivalent to DC.
##### Mathematical Definition
$$
U_{TrueRMS}=\sqrt{\frac{1}{T}\int _{0}^{T}\left(v(t)-U_{offset}\right)^{2}dt}
$$
For a digitized signal sampled at a constant frequency, the integral is replaced by a Riemann sum over **N** samples captured during one or more complete cycles:
$$
U_{TrueRMS}=\sqrt{\frac{1}{N}\sum _{i=1}^{N}\left(U_{i}-U_{offset}\right)^{2}}\times K_{calib}
$$
Where:
- **$U_{i}$** is the individual digital value of the i-th sample.
- **N** is the total number of samples taken during the measurement window.
- **$U_{offset}$** is the operational DC offset.
- **$K_{calib}$** is the linear scaling factor to convert raw digital values to Volts.
##### Calculation Algorithm
- **Instantaneous DC Removal**: For every incoming sample $U_{i}$, subtract the offset to obtain the true AC component: $\Delta U_{i} = U_{i} - U_{offset}$.
- **Squaring**: Square the result to eliminate negative signs and weigh the values properly: $(\Delta U_{i})^2$.
- **Accumulation**: Sum all squared deviations over the window **N**.
- **Mean Evaluation**: Divide the total sum by the sample count **N** to find the variance.
- **Square Root & Calibration**: Take the square root of the mean and multiply by the transformation constant $K_{calib}$ to yield the final physical voltage value.

---
### DC Voltage Calculations
TBD

---
### Environments
Run commands from this folder (`ZMPT101B-lib`).

| Environment | Platform | Purpose |
|---|---|---|
| `nano-board` | Arduino Nano (ATmega328, new bootloader) | `pio run`: __firmware/firmware_stub.cpp__. `pio test`: __test/tests_runner.cpp__ |
| `desktop` | native | Unity tests |
| `desktop-debug` | native | Unity tests with debug symbols (`-O0 -ggdb3`). Default. |

`pio run -e nano-board` links the firmware stub (`setup()` / `loop()`). `pio test` links the Unity runner. Do not put `setup()` / `loop()` in packaged `src/` — that would break consumer sketches. `extra_src_filter.py` adds the stub for `pio run` only.

---
### Build
```powershell
pio run -e nano-board
pio run -e nano-board -t upload
pio run -e desktop-debug
```

Firmware is written to `.pio/build/nano-board/firmware.hex`. Default serial settings are `115200` baud on `COM3`. `pio run -e desktop-debug` builds the Unity test binary (`build_type = test`), not a board image.

Do not run `pio run` and `pio test` in parallel against the same environment; they share `.pio/build` and can corrupt the SCons cache.

---
### Unit tests
```powershell
pio test -e desktop-debug
```

Use `-e desktop` for a non-debug native run, or `-e nano-board` to run tests on the board. See https://docs.platformio.org/en/latest/plus/unit-testing.html for more details.

`pio test` compiles `test/` only. Project `src/*.cpp` is omitted unless `test_build_src = yes`. `build_src_filter` does not change that for a test build.

This library has reader implementations in `src/` (__ZMPT101BReaders.cpp__). The tests call those symbols, so every env sets `test_build_src = yes`. Dependency `.cpp` files arrive through `lib_deps`, which PlatformIO always compiles.

Do not combine `test_build_src = yes` with `build_src_filter = +<*> +<../test/tests_runner.cpp>`. Then __tests_runner.cpp__ is compiled twice (`setup` / `loop` defined twice).

Assertion messages may use __F()__ so the text stays in flash on AVR. Include __unity_extensions.h__ after __unity.h__ (from HC-LIB.System, empty unless `UNIT_TEST` is set). On AVR it also copies each __RUN_TEST__ name from flash into a 96-byte RAM buffer.

Nano test SRAM must stay under 2 KB. `[env:nano-board]` shrinks Serial buffers (`16` / `32`) and sets `UNITY_EXCLUDE_DETAILS`. Float asserts stay enabled for RMS / True RMS tests. After packing a new System tarball, delete `.pio/libdeps` so PlatformIO unpacks `unity_extensions.h`.

---
### Inspect memory usage
A Nano has 32 KB flash and 2 KB SRAM. **Program** is flash; **Data** is SRAM (`.data` + `.bss`). Native builds have no 2 KB limit.

Firmware (`pio run -e nano-board`):
```powershell
pio run -e nano-board
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -C --mcu=atmega328p .pio\build\nano-board\firmware.elf
```

The test image uses the same ELF path (`pio test` overwrites it):
```powershell
pio test -e nano-board --without-uploading --without-testing
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -C --mcu=atmega328p .pio\build\nano-board\firmware.elf
```

Largest symbols (`d` / `b` / `B` are RAM; `T` is flash):
```powershell
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-nm.exe" --size-sort --print-size -t d .pio\build\nano-board\firmware.elf
```

Per-section breakdown:
```powershell
& "$env:USERPROFILE\.platformio\packages\toolchain-atmelavr\bin\avr-size.exe" -A .pio\build\nano-board\firmware.elf
```

---
### Sample
The voltage demo is __examples/ZMPT101B-ReadExampleApp__. The trimmer helper is __examples/ZMPT101B-PotentiometerCalibrationApp__. From either folder:
```powershell
pio run -e nano-board -t upload
```

---
### Packages
Arduino Library Manager metadata is in [library.properties](library.properties). PlatformIO metadata is in [library.json](library.json).

* The `pio package pack -o {local_repo_folder}` command is used for creating PlatformIO package `HC-LIB.ZMPT101B-{version}.tar.gz` in the `{local_repo_folder}` folder.
* The `pio package publish {local_repo_folder}/HC-LIB.ZMPT101B-{version}.tar.gz` command is used for publishing PlatformIO package.

* The `nuget.exe pack HC-LIB.ZMPT101B.nuspec -outputdirectory {local_repo_folder}` command is used for creating NuGet package and store it in the `{local_repo_folder}` folder.
* The `nuget.exe install HC-LIB.ZMPT101B` command is used for installing NuGet package.

---
### Changelog
See [CHANGELOG.md](CHANGELOG.md)

---
### License
See [LICENSE.md](LICENSE.md)
