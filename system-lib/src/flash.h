// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.


/// @file flash.h
/// @brief Flash-string helpers. On Arduino, F() stores literals in PROGMEM.
#ifndef _HC_LIB_FLASH_H_
#define _HC_LIB_FLASH_H_

#ifndef FLASH_C_STR_SIZE
#define FLASH_C_STR_SIZE 160
#endif

#if defined(ARDUINO)
#include <Arduino.h>

/// @brief Copies a flash string into a static RAM buffer for C APIs (Unity, printf).
/// @param t_text PROGMEM string from F(). Null returns null.
/// @return Pointer to the RAM buffer. Overwritten by the next call.
inline const char* flash_c_str(const __FlashStringHelper* t_text)
{
    static char buffer[FLASH_C_STR_SIZE];
    if(!t_text)
    {
        return nullptr;
    }
    strncpy_P(buffer, reinterpret_cast<PGM_P>(t_text), FLASH_C_STR_SIZE - 1);
    buffer[FLASH_C_STR_SIZE - 1] = '\0';
    return buffer;
}

#else

#ifndef F
#define F(string_literal) (string_literal)
#endif

#endif

/// @brief Pass-through on native builds (F() is already a C string).
/// @param t_text C string.
/// @return Pointer to the C string.
inline const char* flash_c_str(const char* t_text)
{
    return t_text;
}

#endif
