// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

#if defined(ARDUINO)
    #include "log/persisters/SerialLogPersister.h"
#else
    #include "StdoutLogPersister.h"
#endif
#include "../providers/DateTimeProvider.h"
#include "log/Log.h"

class LogManager
{
    public:
        static Log* getLogger();
};

/// @brief Gets the logger instance.
/// @return A pointer to the logger instance.
/// @note 1. Meyers' Singleton: Single shared instances in global data (.bss/.data).
///      Prevents stack allocation/overflow and avoids dangerous heap fragmentation (no 'new').
///      2. Lazy Initialization: Objects are constructed only on the very first call.
///      Saves CPU cycles and startup time if logging is not utilized.
///      3. Low-Overhead Return: Passes a 2/4-byte pointer address.
///      Completely avoids expensive object copying.
/// @note 'inline' suggests the compiler embed the function directly into the call site.
///      Eliminates function call overhead (saving stack space and CPU cycles on 16MHz MCUs).
inline Log* LogManager::getLogger()
{
    // Function-static objects live for the process lifetime and skip heap
    // allocation. new Log / new persister would pull malloc bookkeeping into
    // SRAM and fragment the 2 KB Nano heap on every boot.
#if defined(ARDUINO)
    static SerialLogPersister persister;
#else
    static StdoutLogPersister persister;
#endif
    static DateTimeProvider dateTime;
    static Log log(&persister, &dateTime);
    return &log;
}

#endif
