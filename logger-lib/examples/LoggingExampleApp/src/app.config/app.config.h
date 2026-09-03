// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

// Tiny printf enables float / long-long by default. This app only logs %i / %s.
// Disable unused converters before compiling printf.c so AVR does not pull in
// soft-float (_ftoa::pow10, inf/nan strings) or extra integer paths — that
// SRAM and flash stay in the application, not in the logger library.
#define PRINTF_DISABLE_SUPPORT_FLOAT
#define PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#define PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_DISABLE_SUPPORT_PTRDIFF_T
// Default ntoa buffer is 32 bytes on the stack per integer conversion.
#define PRINTF_NTOA_BUFFER_SIZE 16U

#if defined(ARDUINO)
#include <Arduino.h>
#endif
#include "log/Log.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
namespace sout {
    #include "printf.h"
    #include "printf.c"
}

LogLevel gLogLevel()
{
    return LogLevel::Debug;
};

void sout::_putchar(char character)
{
    // stub for standard output
};
