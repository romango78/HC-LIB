// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _IO_TWI_ERRDEF_H_
#define _IO_TWI_ERRDEF_H_

#include "ioerrdef.h"

#define IO_TWI_ERROR (IO_ERROR + 0x100)
#define UNIT_DISABLED_TWI_ERROR (IO_TWI_ERROR + 0x01)
#define TIMEOUT_OCCURRED_TWI_ERROR (IO_TWI_ERROR + 0x02)
#define DEVICE_NOT_FOUND_TWI_ERROR (IO_TWI_ERROR + 0x03)
#define TRANSMITTION_TWI_ERROR (IO_TWI_ERROR + 0x30)
#define ARBITRATION_LOST_TWI_ERROR (IO_TWI_ERROR + 0x38)

#endif