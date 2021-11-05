// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ZMPT101B_DC_READER_H_
#define _ZMPT101B_DC_READER_H_

#include "sensors/readers/SensorReader.h"
#include "sensors/ZMPT101B.h"

class ZMPT101BDCReader : public SensorReader<ZMPT101BVoltageDC>
{
    private:
        IVoltageStream *m_stream;

        float getZero()
        {
            if(!m_sensor)
            {
                return 0;
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
        ZMPT101BDCReader(ZMPT101BSensor *t_sensor, IVoltageStream *t_stream)
            : SensorReader(t_sensor), m_stream(t_stream) {};
        ~ZMPT101BDCReader()
        {
            if(m_stream)
            {
                delete m_stream;
            }
        };

        ZMPT101BVoltageDC read() override
        {   
            if(!m_stream)
            {
                return ZMPT101BVoltageDC((ZMPT101BSensor *)this->m_sensor, (err_t)IO_ERROR_STREAM_NOTCREATED);
            };
            if(!m_stream->canRead())
            {
                m_stream->begin(StreamMode::Read);
            };

            float voltage = 0;
            int zero = getZero();
            for(int index = 0; index < ZMPT101B_READ_ITERATIONS; index++)
            {
                voltage += m_stream->getVoltage() - zero;
            };

            float result = (int)((voltage / ZMPT101B_READ_ITERATIONS) / getSensitivity()) * getSensitivity();
            return ZMPT101BVoltageDC((ZMPT101BSensor *)this->m_sensor, result);
        };
};

#endif