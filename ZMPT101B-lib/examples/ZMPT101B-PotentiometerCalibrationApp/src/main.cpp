// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file main.cpp
/// @brief ZMPT101B calibration helper or RMS / True RMS demo.
/// @note Hardware Calibration Guide for ZMPT101B
/// Although the physical **Sampling / Feedback Resistor** is fixed on the PCB, you can easily adjust the overall board gain using the onboard blue trimming potentiometer (pot). This scales the output AC sine wave to utilize the maximum range of the Arduino ADC without distortion.
/// - **Step 1: Upload a Peak-Detector Sketch**  
///    Upload a simple Arduino script that continuously tracks and prints the minimum (MIN) and maximum (MAX) raw ADC values (0 to 1023) over a 1-second window.
/// - **Step 2: Check the Zero-Voltage Offset**  
///    With the 220V mains disconnected, check the Serial Monitor. The MIN and MAX values should be nearly identical and settle around 512 (the 2.5V DC offset midpoint).
/// - **Step 3: Connect Mains and Adjust the Potentiometer**  
///    Carefully connect the 220V AC input. The values will expand outward (e.g., MIN: 400 | MAX: 620). Use a small screwdriver to turn the blue potentiometer screw:
/// - **Increase the gain** until the MAX value reaches roughly **850 to 900** and the MIN value drops to **120 to 170**.
/// - **Avoid Signal Clipping:** Never let the values hit exactly **0 or 1023**. Leaving a safety margin of ~100 ADC steps ensures the sensor can measure unexpected voltage spikes without flattening the peak of the sine wave.
/// Once adjusted, the hardware is optimally scaled, and you can proceed with software True RMS calculations.
#include <Arduino.h>

#define ZMPT101B_PIN A0

uint16_t _max_value;
uint16_t _min_value;

void setup()
{
    Serial.begin(115200);

    delay(2000);
    _max_value = 0;
    _min_value = 1023;
}

void loop()
{
    uint16_t adcValue = analogRead(ZMPT101B_PIN);

    if(_max_value < adcValue)
    {
        _max_value = adcValue;
    }
    if(_min_value > adcValue)
    {
        _min_value = adcValue;
    }

    Serial.print(F("MAX:"));
    Serial.print(_max_value);
    Serial.print(F(", MIN:"));    
    Serial.print(_min_value);
    Serial.print(F(", ADC:"));
    Serial.println(adcValue);
}