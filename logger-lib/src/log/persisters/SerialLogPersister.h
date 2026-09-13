// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file SerialLogPersister.h
/// @brief ILogPersister that writes to Arduino Serial.
#ifndef _HC_LIB_SERIAL_LOG_PERSISTER_H_
#define _HC_LIB_SERIAL_LOG_PERSISTER_H_

#include "ILogPersister.h"

#if defined(ARDUINO)
#include <Arduino.h>

/// @brief Persists log characters to the Arduino Serial port.
class SerialLogPersister : public ILogPersister
{
    public:
        /// @brief Initializes the persister.
        SerialLogPersister() = default;

        /// @brief Writes one character to the Arduino Serial port.
        /// @param t_character The character to persist.
        void write(const char t_character) override
        {
            Serial.write(t_character);
        };
};

#endif
#endif
