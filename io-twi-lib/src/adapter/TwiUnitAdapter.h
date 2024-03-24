// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#pragma once

#ifndef _TWI_UNIT_ADAPTER_H_
#define _TWI_UNIT_ADAPTER_H_

#include "ITwiAdapter.h"

#define UNDEF_TWI_MODE -1
#define MT_TWI_MODE 0
#define MR_TWI_MODE 1
#define STR_TWI_MODE 2

class TwiUnitAdapter : public ITwiAdapter
{
private:    
    uint8_t m_address = 0;
    uint8_t m_mode = UNDEF_TWI_MODE;
  
public:
    TwiUnitAdapter() {};
    virtual ~TwiUnitAdapter();

    err_t openInMasterMode(const uint8_t t_readWriteMode, const uint8_t t_address) override;
    err_t openInSlaveMode() override;
    err_t close() override;

    Expected<uint8_t> read() override;
    err_t write(const uint8_t t_data) override;

    ITwiAdapter* clone() const override;
};

#endif