// Copyright (c) 2024 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "Log.h"
#include <stddef.h>
#include <string.h>

constexpr LogModule Log::EMPTY_LOG_MODULE;

#if defined(ARDUINO) && defined(__AVR__)
#include <avr/pgmspace.h>

static const char kFmtDatetime[] PROGMEM = "%i-%02i-%02i %02i:%02i:%02i ";
static const char kLevelOff[] PROGMEM = "O";
static const char kLevelCritical[] PROGMEM = LOG_LEVEL_CRITICAL_PREFIX;
static const char kLevelError[] PROGMEM = LOG_LEVEL_ERROR_PREFIX;
static const char kLevelWarn[] PROGMEM = LOG_LEVEL_WARNING_PREFIX;
static const char kLevelInfo[] PROGMEM = LOG_LEVEL_INFO_PREFIX;
static const char kLevelDebug[] PROGMEM = LOG_LEVEL_DEBUG_PREFIX;
static const char* const kLevelNames[] PROGMEM = {
    kLevelOff, kLevelCritical, kLevelError, kLevelWarn, kLevelInfo, kLevelDebug
};

void Log::copyFromFlash(char *t_dest, const char *t_src, const size_t t_destSize) const
{
    strncpy_P(t_dest, t_src, t_destSize - 1);
    t_dest[t_destSize - 1] = '\0';
}

void Log::writeFlash(const char *t_src)
{
    char c;
    while((c = static_cast<char>(pgm_read_byte(t_src++))) != '\0')
    {
        write(c);
    }
}

void Log::writeLevelPrefix(const LogLevel t_level)
{
    const uint8_t index = static_cast<uint8_t>(t_level);
    if(index > static_cast<uint8_t>(LogLevel::Debug))
    {
        write('?');
        write(' ');
        return;
    }

    const char *name = reinterpret_cast<const char *>(pgm_read_word(&kLevelNames[index]));
    writeFlash(name);
    write(' ');
}

#else

void Log::copyFromFlash(char *t_dest, const char *t_src, const size_t t_destSize) const
{
    strncpy(t_dest, t_src, t_destSize - 1);
    t_dest[t_destSize - 1] = '\0';
}

void Log::writeFlash(const char *t_src)
{
    while(*t_src != '\0')
    {
        write(*t_src++);
    }
}

void Log::writeLevelPrefix(const LogLevel t_level)
{
    static const char *const levelNames[] = LOG_LEVEL_NAMES;
    const uint8_t index = static_cast<uint8_t>(t_level);
    const char *prefix = index <= static_cast<uint8_t>(LogLevel::Debug)
        ? levelNames[index]
        : "?";
    writeFlash(prefix);
    write(' ');
}

#endif

void Log::writeDateTime(const DateTime& t_datetime)
{
#if defined(ARDUINO) && defined(__AVR__)
    char fmt[sizeof(kFmtDatetime)];
    copyFromFlash(fmt, kFmtDatetime, sizeof(fmt));
    sout::fctprintf(&Log::writeBounce, this, fmt,
        t_datetime.year,
        t_datetime.month,
        t_datetime.day,
        t_datetime.hours,
        t_datetime.minutes,
        t_datetime.seconds);
#else
    sout::fctprintf(&Log::writeBounce, this,
        "%i-%02i-%02i %02i:%02i:%02i ",
        t_datetime.year,
        t_datetime.month,
        t_datetime.day,
        t_datetime.hours,
        t_datetime.minutes,
        t_datetime.seconds);
#endif
}

void Log::writeModule(const LogModule& t_module)
{
    write('[');
    if(t_module.inFlash)
    {
        writeFlash(t_module.name);
    }
    else
    {
        sout::fctprintf(&Log::writeBounce, this, "%s", t_module.name);
    }
    write(']');
    write(' ');
}
