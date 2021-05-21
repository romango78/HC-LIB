// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ZMPT101B_H_
#define _ZMPT101B_H_

#include "SensorReader.h"
#include "SensorData.h"
#include "PortStream.h"

#define ADC_SCALE 1023.0
#define VREF 5.0
#define NUMBER_OF_ITERATIONS 10

struct ZMPT101BSensor : AnalogSensor
{
    int zero;
    float sensitivity;

    ZMPT101BSensor(const uint8_t t_pin)
        : AnalogSensor(ZMPT101B_SENSOR, t_pin), zero(512), sensitivity(0.001) {};
};

struct ZMPT101BVoltageDC : ISensorData<float>
{
    ZMPT101BVoltageDC() : ISensorData() {};
    
    ZMPT101BVoltageDC(ISensor *t_sensor, float t_data)
        : ISensorData<float>(t_sensor, t_data) {};    

    ZMPT101BVoltageDC(ISensor *t_sensor, err_t t_error)
        : ISensorData<float>(t_sensor, t_error) {};  
};

class ZMPT101B
{
    public:
        static void calibrate(ZMPT101BSensor *t_sensor)
        {
            if(!t_sensor)
            {
                return;
            }
            uint16_t voltage = 0;
            for(int index = 0; index < NUMBER_OF_ITERATIONS; index++)
            {
                voltage += analogRead(t_sensor->pin);
            }
            t_sensor->zero = voltage / NUMBER_OF_ITERATIONS;
        };
};

class ZMPT101BVoltageDCReader : ISensorReader<ZMPT101BVoltageDC>
{
    private:
        AnalogPortStream *m_stream;

        int getZero()
        {
            if(!m_sensor)
            {
                return 512;
            }
            return reinterpret_cast<ZMPT101BSensor*>(this->m_sensor)->zero;
        };

        float getSensitivity()
        {
            if(!m_sensor)
            {
                return 0.001;
            }
            return reinterpret_cast<ZMPT101BSensor*>(this->m_sensor)->sensitivity;
        };
    public:
        ZMPT101BVoltageDCReader(ZMPT101BSensor *t_sensor, AnalogPortStream *t_stream)
            : m_stream(t_stream), ISensorReader(t_sensor) {};

        ZMPT101BVoltageDC read() override
        {   
            if(!m_stream)
            {
                return ZMPT101BVoltageDC(this->m_sensor, (err_t)IO_ERROR_STREAM_NOTCREATED);
            };
            if(!m_stream->canRead())
            {
                m_stream->startRead();
            };

            int16_t voltage = 0;
            for(int index = 0; index < NUMBER_OF_ITERATIONS; index++)
            {
                voltage += m_stream->read() - getZero();
            };

            float result = (float)voltage / NUMBER_OF_ITERATIONS / ADC_SCALE * VREF / getSensitivity();
            return ZMPT101BVoltageDC(this->m_sensor, result);
        };
};

#endif