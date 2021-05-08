// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include <iostream>
#include "printf.h"
#include "log/Log.h"

LogLevelEnum gLogLevel()
{
    return LogLevelEnum::debug;
};

void _putchar(char character)
{
    std::cout << character;
};