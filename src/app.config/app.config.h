// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include <Arduino.h>
#include "Log.h"
#ifdef __cplusplus
extern "C" {
#endif
namespace sout {
    #include "printf.h"
    #include "printf.c"
}
#ifdef __cplusplus
}
#endif

//#define LOGGING_EXAMPLE_APP
#define ZMPT101B_DC_EXAMPLE_APP

LogLevelEnum gLogLevel()
{
    return LogLevelEnum::debug;
};

void sout::_putchar(char character)
{
    Serial.write(character);
};