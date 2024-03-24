// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _I_TWI_STREAM_H_
#define _I_TWI_STREAM_H_

#include "stream/BaseStream.h"

class ITwiMasterStream : public BaseStream<uint8_t>
{
private:
    void begin(const StreamMode t_mode) final {};
protected:
    ITwiMasterStream() 
        : BaseStream() {};    
public:
    virtual ~ITwiMasterStream() = default;
    
    virtual void begin(const StreamMode t_mode, const uint8_t t_address) = 0;    
};

class ITwiSlaveStream : public BaseStream<uint8_t>
{
protected:
    ITwiSlaveStream() 
        : BaseStream() {};
public:    
    virtual ~ITwiSlaveStream() = default;
};

#endif