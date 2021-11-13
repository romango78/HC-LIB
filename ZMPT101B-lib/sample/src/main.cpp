// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include <Arduino.h>
#include "app.config/app.config.h"

#ifdef ZMPT101B_POTENTIOMETER_CALIBRATION_APP

#define PolynomialEquation1(x) (2.709*x - 8.35)
#define PolynomialEquation2(x) (0.0007711*x*x + 2.506*x + 0.2662)
#define PolynomialEquation3(x) (0.00000412*x*x*x - 0.000857*x*x + 2.675*x - 3.198)
#define PolynomialEquation4(x) (-0.00000004888*x*x*x*x + 0.00002986*x*x*x - 0.005183*x*x + 2.922*x - 6.085)
#define PolynomialEquation5(x) (-0.0000000001278*x*x*x*x*x + 0.00000003529*x*x*x*x + 0.00001023*x*x*x - 0.003271*x*x + 2.853*x - 5.57)

int _max_value;
int _min_value;

void setup() 
{
    // Setup device
    Serial.begin(115200);   

    delay(2000);

    _max_value = 0;
    _min_value = 1023;

    Serial.println("220V, -220V, MAX, MIN, AC, PQ");
}

void loop() 
{
    int value = analogRead(A0) - 512;
    if(_max_value < value)
    {
        _max_value = value;
    }
    if(_min_value > value)
    {
        _min_value = value;
    }

    Serial.print(220);
    Serial.print(" ");
    Serial.print(-220);
    Serial.print(" ");
    Serial.print(_max_value);
    Serial.print(" ");
    Serial.print(_min_value);
    Serial.print(" ");
    Serial.print(value);
    Serial.print(" ");
    Serial.println(PolynomialEquation3(value));
    delay(100);
}

#endif 

#ifdef ZMPT101B_RMS_READ_EXAMPLE_APP

#include "adapter/AnalogPortAdapter.h"
#include "sensors/ZMPT101B.h"
#include "sensors/readers/ZMPT101BReader.h"
#include "timers/ArduinoTimer.h"

ZMPT101BRmsReader* rmsReader;

void setup() 
{
    // Setup device
    Serial.begin(115200);   

    delay(2000);

    // Setup Application      
    ZMPT101BSensor* sensor = new ZMPT101BSensor(A0);
    sensor->sensitivity = 0.004;
    sensor->zero = 0;
    
    IPortAdapter *adapter = (IPortAdapter*)new AnalogPortAdapter(sensor->pin);
    IVoltageStream *stream = (IVoltageStream *)new VoltageStream(adapter);
    ZMPT101B::calibrate(sensor, stream);

    ITimer *timer = (ITimer *)new ArduinoTimer();

    Serial.println("The ZMPT101B was initialized and calibrated. It contains the following data:");
    Serial.print("      Type: ");
    Serial.println(sensor->type);
    Serial.print("      Category: ");
    Serial.println(sensor->category);
    Serial.print("      Pin: ");
    Serial.println(sensor->pin);
    Serial.print("      Zero: ");
    Serial.println(sensor->zero);
    Serial.print("      Sensitivity: ");
    Serial.println(sensor->sensitivity);

    rmsReader = new ZMPT101BRmsReader(sensor, stream, timer);
}

void loop() 
{
    ZMPT101B_ACVoltage sensorData = rmsReader->read();

    Serial.print("RMS (V): ");
    Serial.println(sensorData.data, 2);
    delay(100);
}

#endif

#ifdef ZMPT101B_TRUE_RMS_READ_EXAMPLE_APP

#include "adapter/AnalogPortAdapter.h"
#include "sensors/ZMPT101B.h"
#include "sensors/readers/ZMPT101BReader.h"
#include "timers/ArduinoTimer.h"

ZMPT101BTrueRmsReader* rmsReader;

void setup() 
{
    // Setup device
    Serial.begin(115200);   

    delay(2000);

    // Setup Application      
    ZMPT101BSensor* sensor = new ZMPT101BSensor(A0);
    sensor->sensitivity = 2.39;//0.004;
    sensor->zero = 0;
    
    IPortAdapter *adapter = (IPortAdapter*)new AnalogPortAdapter(sensor->pin);
    IVoltageStream *stream = (IVoltageStream *)new VoltageStream(adapter);
    ZMPT101B::calibrate(sensor, stream);

    ITimer *timer = (ITimer *)new ArduinoTimer();

    Serial.println("The ZMPT101B was initialized and calibrated. It contains the following data:");
    Serial.print("      Type: ");
    Serial.println(sensor->type);
    Serial.print("      Category: ");
    Serial.println(sensor->category);
    Serial.print("      Pin: ");
    Serial.println(sensor->pin);
    Serial.print("      Zero: ");
    Serial.println(sensor->zero, 2);
    Serial.print("      Sensitivity: ");
    Serial.println(sensor->sensitivity, 4);

    rmsReader = new ZMPT101BTrueRmsReader(sensor, stream, timer);
}

void loop() 
{
    ZMPT101B_ACVoltage sensorData = rmsReader->read();

    Serial.print("True RMS (V): ");
    Serial.println(sensorData.data, 2);
    delay(100);
}

#endif