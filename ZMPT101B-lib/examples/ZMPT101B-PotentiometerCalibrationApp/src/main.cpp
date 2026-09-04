// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file main.cpp
/// @brief ZMPT101B calibration helper or RMS / True RMS demo.
#if defined(ARDUINO)
    #include <Arduino.h>
#else
    #include <cstdio>
    #include <thread>
    #include <chrono>
    inline void delay(unsigned long t_milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(t_milliseconds));
    }
#ifndef F
#define F(string_literal) (string_literal)
#endif
#ifndef A0
#define A0 14
#endif
#endif

#define PolynomialEquation3(x) (0.00000412*x*x*x - 0.000857*x*x + 2.675*x - 3.198)

int _max_value;
int _min_value;

void setup()
{
#if defined(ARDUINO)
    Serial.begin(115200);
#endif
    delay(2000);
    _max_value = 0;
    _min_value = 1023;
#if defined(ARDUINO)
    Serial.println(F("MAX, MIN, ADC, PQ"));
#else
    std::puts("MAX, MIN, ADC, PQ");
#endif
}

void loop()
{
#if defined(ARDUINO)
    int value = analogRead(A0) - 512;
#else
    int value = 0;
#endif
    if(_max_value < value)
    {
        _max_value = value;
    }
    if(_min_value > value)
    {
        _min_value = value;
    }
#if defined(ARDUINO)
    Serial.print(_max_value);
    Serial.write(' ');
    Serial.print(_min_value);
    Serial.write(' ');
    Serial.print(value);
    Serial.write(' ');
    Serial.println(PolynomialEquation3(value));
#else
    std::printf("%d %d %d %f\n", _max_value, _min_value, value, PolynomialEquation3(value));
#endif
    delay(100);
}