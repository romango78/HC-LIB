// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ERRDEF_H_
#define _ERRDEF_H_

#define NO_ERROR 0x0000
#define ARGUMENT_IS_NULL_ERROR (NO_ERROR + 1)
#define OUT_OF_RANGE_ERROR (NO_ERROR + 2)

typedef unsigned short err_t;

#endif