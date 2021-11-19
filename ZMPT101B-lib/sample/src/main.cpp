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

    Serial.println("MAX, MIN, ADC, PQ");
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

#ifdef ZMPT101B_READ_EXAMPLE_APP

#include "adapter/AnalogPortAdapter.h"
#include "stream/AnalogStream.h"
#include "sensors/ZMPT101B.h"
#include "sensors/readers/ZMPT101BReaders.h"
#include "timers/ArduinoTimer.h"

#define ZMPT101B_PIN A0

ZMPT101BSensor* sensor;
ZMPT101BRmsReader* rmsReader;
ZMPT101BTrueRmsReader* trueRmsReader;

void setup() 
{
    // Setup device
    Serial.begin(115200);      

    // Setup Application      
    IPortAdapter<int> *adapter = (IPortAdapter<int>*)new AnalogPortAdapter(ZMPT101B_PIN);
    IStream<uint16_t> *stream = (IStream<uint16_t> *)new AnalogStream(adapter);

    sensor = new ZMPT101BSensor(ZMPT101B_PIN, stream);
    ZMPT101B::calibrate(sensor);

    ITimer *timer = (ITimer *)new ArduinoTimer();

    rmsReader = new ZMPT101BRmsReader(timer);
    trueRmsReader = new ZMPT101BTrueRmsReader(timer);

    Serial.println("220V,RMS,TrueRMS");

    delay(2000);
}

void loop() 
{
    ZMPT101B_ACVoltage sensorData1 = rmsReader->read(sensor);
    ZMPT101B_ACVoltage sensorData2 = trueRmsReader->read(sensor);

    Serial.print(220.00);
    Serial.print(" ");
    Serial.print(sensorData1.data, 2);
    Serial.print(" ");
    Serial.println(sensorData2.data, 2);
    
    delay(500);
}

#endif