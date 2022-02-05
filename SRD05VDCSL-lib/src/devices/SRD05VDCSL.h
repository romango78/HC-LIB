// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _SRD05VDCSL_H_
#define _SRD05VDCSL_H_

#include "devices/DigitalDevices.h"

struct SRD05VDCSLDevice : public RelayDevice
{
    SRD05VDCSLDevice() = delete;
    SRD05VDCSLDevice(const uint8_t t_pin, IStream<uint8_t>* const t_stream)
        : RelayDevice(RELAY_DEVICE_TYPE, t_pin, t_stream) {};    
};

#endif