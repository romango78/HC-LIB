// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "DigitalDeviceController.h"
#include "ioerrdef.h"

err_t DigitalDeviceController::setState(DigitalDevice* t_device, uint8_t t_data)
{
    if(!t_device)
    {
        return (err_t)ARGUMENT_IS_NULL_ERROR;
    }
    if(!t_device->digitalStream)
    {
        return (err_t)IO_ERROR_STREAM_NOTCREATED;
    };
    if(!t_device->digitalStream->canWrite())
    {
        t_device->digitalStream->begin(StreamMode::Write);
    };
    t_device->digitalStream->write(t_data);
    return NO_ERROR;
}

uint8_t DigitalDeviceController::getState(DigitalDevice* t_device)
{
/*    StateData stateData = {0, NO_ERROR};
    if(!t_device)
    {
        stateData.error = (err_t)ARGUMENT_IS_NULL_ERROR;
    }
    else if(!t_device->digitalStream)
    {
        stateData.error = (err_t)IO_ERROR_STREAM_NOTCREATED;
    }
    else
    {

    }*/
    return t_device->digitalStream->getState();
}