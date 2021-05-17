// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _KEY_VALUE_PAIR_TESTS_H_
#define _KEY_VALUE_PAIR_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "generic/KeyValuePair.h"

class KeyValuePairTestClass
{
    private:
        int m_data;
    public:
        KeyValuePairTestClass(int t_data)
        {
            m_data = t_data;
        };

        int getData()
        {
            return m_data;
        };
};

struct KeyValuePairTestStruct
{
    char *data;
};

void KeyValuePair_ShouldCreate_CharKey_ClassValue()
{
    char key[] = "Key";
    int data = 125;
    KeyValuePairTestClass *value = new KeyValuePairTestClass(data);
    KeyValuePair<char*, KeyValuePairTestClass*> *sut = new KeyValuePair<char*, KeyValuePairTestClass*>(key, value);

    TEST_ASSERT_EQUAL_STRING(key, sut->getKey());
    TEST_ASSERT_EQUAL(value, sut->getValue());
    TEST_ASSERT_EQUAL(data, sut->getValue()->getData());
}

void KeyValuePair_ShouldCreate_CharKey_StructValue()
{
    char key[] = "Key";
    char data[] = "Value";
    KeyValuePairTestStruct value = { data };
    KeyValuePair<char*, KeyValuePairTestStruct> *sut = new KeyValuePair<char*, KeyValuePairTestStruct>(key, value);

    TEST_ASSERT_EQUAL_STRING(key, sut->getKey());    
    TEST_ASSERT_EQUAL(data, sut->getValue().data);
}

void KeyValuePair_ShouldCreate_RefKey_RefValue()
{
    char key[] = "Key";
    char value[] = "Value";
    KeyValuePair<char*, char*> *sut = new KeyValuePair<char*, char*>(key, value);

    TEST_ASSERT_EQUAL_STRING(key, sut->getKey());
    TEST_ASSERT_EQUAL_STRING(value, sut->getValue());
}

void KeyValuePair_ShouldCreate_RefKey_ValueValue()
{
    char key[] = "Key";
    int value = 10;
    KeyValuePair<char*, int> *sut = new KeyValuePair<char*, int>(key, value);

    TEST_ASSERT_EQUAL_STRING(key, sut->getKey());
    TEST_ASSERT_EQUAL(value, sut->getValue());
}

void KeyValuePair_ShouldCreate_ValueKey_RefValue()
{
    int key = 10;
    char value[] = "Value";
    KeyValuePair<int, char*> *sut = new KeyValuePair<int, char*>(key, value);

    TEST_ASSERT_EQUAL(key, sut->getKey());
    TEST_ASSERT_EQUAL_STRING(value, sut->getValue());
}

void KeyValuePair_ShouldCreate_ValueKey_ValueValue()
{
    int key = 10;
    double value = 123.44;
    KeyValuePair<int, double> *sut = new KeyValuePair<int, double>(key, value);

    TEST_ASSERT_EQUAL(key, sut->getKey());
    TEST_ASSERT_EQUAL(value, sut->getValue());
}

#endif
#endif