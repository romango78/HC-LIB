// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "DigitalDeviceController.h"
#include "ioerrdef.h"

err_t DigitalDeviceController::setState(DigitalDevice t_device, const uint8_t t_data)
{
    if(!t_device.stream)
    {
        return (err_t)STREAM_NOTCREATED_IO_ERROR;
    };
    if(!t_device.stream->canWrite())
    {
        t_device.stream->begin(StreamMode::Write);
    };
    t_device.stream->write(t_data);
    return NO_ERROR;
}

Expected<uint8_t> DigitalDeviceController::getState(DigitalDevice t_device)
{
    if(!t_device.stream)
    {
        return Expected<uint8_t>::fromError(STREAM_NOTCREATED_IO_ERROR);
    }
    return t_device.stream->getState();
}