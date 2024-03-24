// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _TWI_MASTER_STREAM_H_
#define _TWI_MASTER_STREAM_H_

#include <inttypes.h>
#include "adapter/ITwiAdapter.h"
#include "ITwiStream.h"

class TwiMasterStream : public ITwiMasterStream
{
private:
    ITwiAdapter* const m_twi;
public:
    TwiMasterStream() = delete;
    TwiMasterStream(ITwiAdapter* const t_twi)
        : ITwiMasterStream(), m_twi(t_twi) {};
    virtual ~TwiMasterStream()
    {
        if(m_twi)
        {
            delete m_twi;
        }
    };

    void begin(const StreamMode t_mode, const uint8_t t_address) override;
    uint8_t read() override;
    void write(const uint8_t t_data) override;
    void end() override;

    IStream<uint8_t>* clone() const override;
};

#endif