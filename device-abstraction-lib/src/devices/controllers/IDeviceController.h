// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _I_DEVICE_CONTROLLER_H_
#define _I_DEVICE_CONTROLLER_H_

#include "errdef.h"
#include "Expected.h"

template<typename State, typename Device>
class IDeviceController
{
    public:
        IDeviceController() {};
        virtual ~IDeviceController() = default;

        virtual err_t setState(Device t_device, const State t_state) = 0;    
        virtual Expected<State> getState(Device t_device) = 0;
};

#endif