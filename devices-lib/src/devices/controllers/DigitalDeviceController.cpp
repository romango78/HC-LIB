// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "DigitalDeviceController.h"
#include "errors/IoErrors.h"

Error DigitalDeviceController::setState(const DigitalDevice& t_device, const uint8_t& t_state)
{
    if(!t_device.stream)
    {
        return to_error(IoError::StreamNotCreated);
    }
    if(!t_device.stream->canWrite())
    {
        t_device.stream->begin(StreamMode::Write);
    }
    t_device.stream->write(t_state);
    return to_error(GenericError::NoError);
}

Expected<uint8_t, Error> DigitalDeviceController::getState(const DigitalDevice& t_device) const
{
    if(!t_device.stream)
    {
        return make_error(IoError::StreamNotCreated);
    }
    return t_device.stream->getState();
}
