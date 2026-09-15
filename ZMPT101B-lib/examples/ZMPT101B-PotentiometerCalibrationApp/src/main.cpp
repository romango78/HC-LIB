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

#define ADC_BITS    10
#define ADC_COUNTS  (1<<ADC_BITS)
#define ZMPT101B_PIN A0


uint16_t _maxValue;
uint16_t _minValue;
float _zero;

void setup()
{
    Serial.begin(115200);
    _maxValue = 0;
    _minValue = 1023;
    _zero = ADC_COUNTS>>1;

    delay(2000);
}

void loop()
{
    uint16_t rawValue = analogRead(ZMPT101B_PIN);
    
    _zero += (rawValue - _zero) / ADC_COUNTS;
    float adjValue = rawValue - _zero;

    if(_maxValue < rawValue)
    {
        _maxValue = rawValue;
    }
    if(_minValue > rawValue)
    {
        _minValue = rawValue;
    }

    Serial.print(F("MAX:"));
    Serial.print(_maxValue);
    Serial.print(F(", MIN:"));    
    Serial.print(_minValue);
    Serial.print(F(", ADC:"));
    Serial.print(rawValue);
    Serial.print(F(", ZERO:"));
    Serial.print(_zero);
    Serial.print(F(", ADJ_ADC:"));
    Serial.println(adjValue);
}