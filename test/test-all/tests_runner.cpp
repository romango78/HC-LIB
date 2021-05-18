// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#if defined(UNIT_TEST) 

#include <unity.h>
#include "log/LogTests.h"
#include "collections/generic/KeyValuePairTests.h"
#include "collections/generic/EnumeratorBaseTests.h"

//#if defined(ARDUINO)

void setUp(void)
{
}

void tearDown(void) 
{
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    /*****************************************
     * Log Tests (LogTests.h)
     *****************************************/
    RUN_TEST(Log_ShouldLogDebug_WithoutModule);
    RUN_TEST(Log_ShouldLogDebug_WithModule);
    RUN_TEST(Log_ShouldLogInfo_WithoutModule);
    RUN_TEST(Log_ShouldLogInfo_WithModule);
    RUN_TEST(Log_ShouldLogWarning_WithoutModule);
    RUN_TEST(Log_ShouldLogWarning_WithModule);
    RUN_TEST(Log_ShouldLogError_WithoutModule);
    RUN_TEST(Log_ShouldLogError_WithModule);
    RUN_TEST(Log_ShouldLogFatal_WithoutModule);
    RUN_TEST(Log_ShouldLogFatal_WithModule);

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

#endif