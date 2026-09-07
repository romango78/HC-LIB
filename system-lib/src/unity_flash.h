// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file unity_flash.h
/// @brief Shared Unity adapter: *_MESSAGE macros accept F() / __FlashStringHelper*.
/// Include after unity.h in any HC-LIB test. Empty unless UNIT_TEST is defined.
#ifndef _HC_LIB_UNITY_FLASH_H_
#define _HC_LIB_UNITY_FLASH_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "flash.h"

#ifdef TEST_FAIL_MESSAGE
#undef TEST_FAIL_MESSAGE
#define TEST_FAIL_MESSAGE(message) UNITY_TEST_FAIL(__LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_IGNORE_MESSAGE
#undef TEST_IGNORE_MESSAGE
#define TEST_IGNORE_MESSAGE(message) UNITY_TEST_IGNORE(__LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_MESSAGE
#undef TEST_MESSAGE
#define TEST_MESSAGE(message) UnityMessage((flash_c_str(message)), __LINE__)
#endif

#ifdef TEST_ASSERT_MESSAGE
#undef TEST_ASSERT_MESSAGE
#define TEST_ASSERT_MESSAGE(condition, message) UNITY_TEST_ASSERT((condition), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_TRUE_MESSAGE
#undef TEST_ASSERT_TRUE_MESSAGE
#define TEST_ASSERT_TRUE_MESSAGE(condition, message) UNITY_TEST_ASSERT((condition), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_FALSE_MESSAGE
#undef TEST_ASSERT_FALSE_MESSAGE
#define TEST_ASSERT_FALSE_MESSAGE(condition, message) UNITY_TEST_ASSERT(!(condition), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_UNLESS_MESSAGE
#undef TEST_ASSERT_UNLESS_MESSAGE
#define TEST_ASSERT_UNLESS_MESSAGE(condition, message) UNITY_TEST_ASSERT(!(condition), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_NULL_MESSAGE
#undef TEST_ASSERT_NULL_MESSAGE
#define TEST_ASSERT_NULL_MESSAGE(pointer, message) UNITY_TEST_ASSERT_NULL((pointer), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_NOT_NULL_MESSAGE
#undef TEST_ASSERT_NOT_NULL_MESSAGE
#define TEST_ASSERT_NOT_NULL_MESSAGE(pointer, message) UNITY_TEST_ASSERT_NOT_NULL((pointer), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_EQUAL_MESSAGE
#undef TEST_ASSERT_EQUAL_MESSAGE
#define TEST_ASSERT_EQUAL_MESSAGE(expected, actual, message) UNITY_TEST_ASSERT_EQUAL_INT((expected), (actual), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_EQUAL_PTR_MESSAGE
#undef TEST_ASSERT_EQUAL_PTR_MESSAGE
#define TEST_ASSERT_EQUAL_PTR_MESSAGE(expected, actual, message) UNITY_TEST_ASSERT_EQUAL_PTR((expected), (actual), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_EQUAL_STRING_MESSAGE
#undef TEST_ASSERT_EQUAL_STRING_MESSAGE
#define TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, message) UNITY_TEST_ASSERT_EQUAL_STRING((expected), (actual), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_FLOAT_WITHIN_MESSAGE
#undef TEST_ASSERT_FLOAT_WITHIN_MESSAGE
#define TEST_ASSERT_FLOAT_WITHIN_MESSAGE(delta, expected, actual, message) UNITY_TEST_ASSERT_FLOAT_WITHIN((delta), (expected), (actual), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_EQUAL_INT_MESSAGE
#undef TEST_ASSERT_EQUAL_INT_MESSAGE
#define TEST_ASSERT_EQUAL_INT_MESSAGE(expected, actual, message) UNITY_TEST_ASSERT_EQUAL_INT((expected), (actual), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_EQUAL_UINT_MESSAGE
#undef TEST_ASSERT_EQUAL_UINT_MESSAGE
#define TEST_ASSERT_EQUAL_UINT_MESSAGE(expected, actual, message) UNITY_TEST_ASSERT_EQUAL_UINT((expected), (actual), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_NOT_EQUAL_MESSAGE
#undef TEST_ASSERT_NOT_EQUAL_MESSAGE
#define TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, message) UNITY_TEST_ASSERT(((expected) != (actual)), __LINE__, (flash_c_str(message)))
#endif

#ifdef TEST_ASSERT_LESS_OR_EQUAL_MESSAGE
#undef TEST_ASSERT_LESS_OR_EQUAL_MESSAGE
#define TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(threshold, actual, message) UNITY_TEST_ASSERT_SMALLER_OR_EQUAL_INT((threshold), (actual), __LINE__, (flash_c_str(message)))
#endif

#if defined(ARDUINO)

#ifndef UNITY_FLASH_NAME_SIZE
#define UNITY_FLASH_NAME_SIZE 96
#endif

/// @brief Copies a flash test name into a dedicated RAM buffer (not flash_c_str).
/// @param t_text PROGMEM string from F(#func). Null returns null.
/// @return Pointer kept by Unity for the duration of the current test.
inline const char* unity_flash_name(const __FlashStringHelper* t_text)
{
    static char buffer[UNITY_FLASH_NAME_SIZE];
    if (!t_text)
    {
        return nullptr;
    }
    strncpy_P(buffer, reinterpret_cast<PGM_P>(t_text), UNITY_FLASH_NAME_SIZE - 1);
    buffer[UNITY_FLASH_NAME_SIZE - 1] = '\0';
    return buffer;
}

#ifdef RUN_TEST
#undef RUN_TEST
#endif
#define RUN_TEST(func) UnityDefaultTestRun((func), unity_flash_name(F(#func)), __LINE__)

#ifdef UNITY_BEGIN
#undef UNITY_BEGIN
#endif
#define UNITY_BEGIN() UnityBegin("")

#endif

#endif
#endif
