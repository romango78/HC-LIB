// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#pragma once

#ifndef _I_TWI_ADAPTER_H_
#define _I_TWI_ADAPTER_H_

#include <inttypes.h>
#include "ICloneable.h"
#include "Expected.h"
#include "iotwierrdef.h"

#define NO_DATA 0

class ITwiAdapter : public ICloneable<ITwiAdapter>
{
public:
    ITwiAdapter() = default;
    virtual ~ITwiAdapter() = default;

    virtual err_t openInMasterMode(const uint8_t t_readWriteMode, const uint8_t t_address) = 0;
    virtual err_t openInSlaveMode() = 0;
    virtual err_t close() = 0;

    virtual Expected<uint8_t> read() = 0;
    virtual err_t write(const uint8_t t_data) = 0;    

    virtual ITwiAdapter* clone() const = 0;
};

#endif