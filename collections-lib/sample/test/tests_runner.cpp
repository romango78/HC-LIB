// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#if defined(UNIT_TEST) 

#include <unity.h>

#include "collections/generic/KeyValuePairTests.h"
#include "collections/generic/EnumeratorBaseTests.h"

void dummyTest()
{
    TEST_ASSERT_EQUAL(2, 1+1);
}

void testsProcess() 
{
    UNITY_BEGIN();

    /*****************************************
     * Collections\Generic\KeyValuePair<Tkey, Tvalue>
     *****************************************/
    RUN_TEST(KeyValuePair_ShouldCreate_RefKey_RefValue);
    RUN_TEST(KeyValuePair_ShouldCreate_RefKey_ValueValue);
    RUN_TEST(KeyValuePair_ShouldCreate_ValueKey_RefValue);
    RUN_TEST(KeyValuePair_ShouldCreate_ValueKey_ValueValue);
    RUN_TEST(KeyValuePair_ShouldCreate_CharKey_ClassValue);
    RUN_TEST(KeyValuePair_ShouldCreate_CharKey_StructValue);

    /*****************************************
     * Collections\Generic\IEnumerator<T>
     *****************************************/
    RUN_TEST(EnumeratorBase_ShouldNotGetFirstItem_AfterReset);
    RUN_TEST(EnumeratorBase_ShouldGetFirstItem_AfterReset_AndOneCallMoveNext);
    RUN_TEST(EnumeratorBase_ShouldNotGetLastItem_WhenMoveNextReturnFalse);
    RUN_TEST(EnumeratorBase_ShouldNotGetItem_AfterFirstMoveNext_WithoutReset);
    RUN_TEST(EnumeratorBase_ShouldNotGetItem_AfterInitializing_WithoutReset);
    RUN_TEST(EnumeratorBase_ShouldReset_WhenNoItemsToEnumerate);
    RUN_TEST(EnumeratorBase_ShouldMoveNext_WhenNoItemsToEnumerate);
    RUN_TEST(EnumeratorBase_ShouldEnumerate_ValueTypes);
    RUN_TEST(EnumeratorBase_ShouldEnumerate_RefTypes); 

    UNITY_END();
};

#if !defined(ARDUINO)

void setUp(void)
{
    // STUB
};

void tearDown(void) 
{
    // STUB
};

int main( int argc, char **argv) 
{
    testsProcess();
    return 0;
};

#endif

#if defined(ARDUINO)

#include <Arduino.h>

void setup() {
    // Setup device
    Serial.begin(115200);

    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(4000);

    testsProcess();
};

void loop() 
{
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
};

#endif
#endif