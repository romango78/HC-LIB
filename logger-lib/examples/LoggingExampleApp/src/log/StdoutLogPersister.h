// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _STDOUT_LOG_PERSISTER_H_
#define _STDOUT_LOG_PERSISTER_H_

#if !defined(ARDUINO)

#include <cstdio>
#include "log/persisters/ILogPersister.h"

/// @brief Native stand-in for SerialLogPersister: writes to stdout.
class StdoutLogPersister : public ILogPersister
{
    public:
        StdoutLogPersister() = default;

        void write(const char t_character) override
        {
            putchar(t_character);
            if(t_character == '\n')
            {
                fflush(stdout);
            }
        };
};

#endif
#endif
