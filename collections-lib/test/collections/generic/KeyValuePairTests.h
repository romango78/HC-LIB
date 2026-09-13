// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file KeyValuePairTests.h
/// @brief Unity tests for KeyValuePair{TKey, TValue}.

#ifndef _KEY_VALUE_PAIR_TESTS_H_
#define _KEY_VALUE_PAIR_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "unity_extensions.h"
#include "collections/generic/KeyValuePair.h"

/// @brief Value type used to check a class stored in a pair.
class KeyValuePairTestClass
{
    private:
        int m_data;
    public:
        /// @brief Stores _t_data_ as the payload.
        explicit KeyValuePairTestClass(int t_data)
            : m_data(t_data)
        {}

        /// @brief Returns the stored payload.
        int getData() const
        {
            return m_data;
        }
};

/// @brief Value type used to check a struct stored in a pair.
struct KeyValuePairTestStruct
{
    const char *data;
};

/// @brief Stores a const char* key and a class value (by value).
void KeyValuePair_ShouldCreate_CharKey_ClassValue()
{
    const char* key = "Key";
    int data = 125;
    KeyValuePairTestClass value(data);
    KeyValuePair<const char*, KeyValuePairTestClass> sut(key, value);

    TEST_ASSERT_EQUAL_STRING_MESSAGE(key, sut.getKey(), F("Expected key to match"));
    TEST_ASSERT_EQUAL_MESSAGE(data, sut.getValue().getData(), F("Expected class payload to match"));
}

/// @brief Stores a const char* key and a struct value (by value).
void KeyValuePair_ShouldCreate_CharKey_StructValue()
{
    const char* key = "Key";
    const char* data = "Value";
    KeyValuePairTestStruct value = { data };
    KeyValuePair<const char*, KeyValuePairTestStruct> sut(key, value);

    TEST_ASSERT_EQUAL_STRING_MESSAGE(key, sut.getKey(), F("Expected key to match"));
    TEST_ASSERT_EQUAL_STRING_MESSAGE(data, sut.getValue().data, F("Expected struct payload to match"));
}

/// @brief Stores pointer key and pointer value (pair does not own the strings).
void KeyValuePair_ShouldCreate_RefKey_RefValue()
{
    const char* key = "Key";
    const char* value = "Value";
    KeyValuePair<const char*, const char*> sut(key, value);

    TEST_ASSERT_EQUAL_STRING_MESSAGE(key, sut.getKey(), F("Expected key to match"));
    TEST_ASSERT_EQUAL_STRING_MESSAGE(value, sut.getValue(), F("Expected value to match"));
}

/// @brief Stores a pointer key and an int value.
void KeyValuePair_ShouldCreate_RefKey_ValueValue()
{
    const char* key = "Key";
    int value = 10;
    KeyValuePair<const char*, int> sut(key, value);

    TEST_ASSERT_EQUAL_STRING_MESSAGE(key, sut.getKey(), F("Expected key to match"));
    TEST_ASSERT_EQUAL_MESSAGE(value, sut.getValue(), F("Expected value to match"));
}

/// @brief Stores an int key and a pointer value.
void KeyValuePair_ShouldCreate_ValueKey_RefValue()
{
    int key = 10;
    const char* value = "Value";
    KeyValuePair<int, const char*> sut(key, value);

    TEST_ASSERT_EQUAL_MESSAGE(key, sut.getKey(), F("Expected key to match"));
    TEST_ASSERT_EQUAL_STRING_MESSAGE(value, sut.getValue(), F("Expected value to match"));
}

/// @brief Stores an int key and a double value.
void KeyValuePair_ShouldCreate_ValueKey_ValueValue()
{
    int key = 10;
    double value = 123.44;
    KeyValuePair<int, double> sut(key, value);

    TEST_ASSERT_EQUAL_MESSAGE(key, sut.getKey(), F("Expected key to match"));
    TEST_ASSERT_TRUE_MESSAGE(sut.getValue() == value, F("Expected value to match"));
}

#endif
#endif
