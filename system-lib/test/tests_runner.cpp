// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#if defined(ARDUINO) && !defined(UNIT_TEST)

#include <Arduino.h>

void setup()
{
}

void loop()
{
    delay(100);
}

#endif

#if defined(UNIT_TEST)

#include <unity.h>
#include "ExpectedTests.h"
#include "MoveTests.h"
#include "ArduinoTimerTests.h"

void dummyTest()
{
    TEST_ASSERT_EQUAL(2, 1+1);
}

void testsProcess() 
{
    UNITY_BEGIN();

    /*****************************************
     * Expected<T>
     *****************************************/
    RUN_TEST(ExpectedValueType_ShouldContainError_WhenSomeErrorOccurred);
    RUN_TEST(ExpectedValueType_ShouldNotContainError_WhenSomeErrorNotOccurred);
    RUN_TEST(ExpectedPointer_ShouldNotContainError_WhenSomeErrorNotOccurred);
    RUN_TEST(ExpectedPointer_ShouldContainError_WhenSomeErrorOccurred);
    RUN_TEST(ExpectedReferenceValue_ShouldNotContainError_WhenSomeErrorNotOccurred);
    RUN_TEST(ExpectedReferenceValue_ShouldContainError_WhenSomeErrorOccurred);
    RUN_TEST(ExpectedVoid_ShouldNotContainError_WhenSomeErrorNotOccurred);
    RUN_TEST(ExpectedVoid_ShouldContainError_WhenSomeErrorOccurred);
    RUN_TEST(ExpectedExpectedType_ShouldNotContainError_WhenSomeErrorNotOccurred);
    RUN_TEST(ExpectedExpected_ShouldContainError_WhenSomeErrorOccurred);
    RUN_TEST(Expected_ValueCtor_ShouldStoreValue_WhenConstructedFromConstLvalue);
    RUN_TEST(Expected_ValueCtor_ShouldMoveValue_WhenConstructedFromRvalue);
    RUN_TEST(Expected_ErrorCtor_ShouldStoreError_WhenConstructedFromUnexpectedLvalue);
    RUN_TEST(Expected_ErrorCtor_ShouldStoreError_WhenConstructedFromUnexpectedRvalue);
    RUN_TEST(Expected_ErrorCtor_ShouldConvertError_WhenUnexpectedTypeDiffers);
    RUN_TEST(Expected_CopyCtor_ShouldCopyValue_WhenSourceHasValue);
    RUN_TEST(Expected_CopyCtor_ShouldCopyError_WhenSourceHasError);
    RUN_TEST(Expected_CopyAssign_ShouldCopyValue_WhenBothHaveValue);
    RUN_TEST(Expected_CopyAssign_ShouldCopyError_WhenBothHaveError);
    RUN_TEST(Expected_CopyAssign_ShouldReplaceValueWithError_WhenSourceHasError);
    RUN_TEST(Expected_CopyAssign_ShouldReplaceErrorWithValue_WhenSourceHasValue);
    RUN_TEST(Expected_CopyAssign_ShouldKeepValue_WhenSelfAssigned);
    RUN_TEST(Expected_MoveCtor_ShouldPreserveValue_WhenSourceHasValue);
    RUN_TEST(Expected_MoveCtor_ShouldMoveError_WhenSourceHasError);
    RUN_TEST(Expected_MoveAssign_ShouldReplaceValue_WhenBothHaveValue);
    RUN_TEST(Expected_MoveAssign_ShouldMoveError_WhenBothHaveError);
    RUN_TEST(Expected_MoveAssign_ShouldReplaceValueWithError_WhenSourceHasError);
    RUN_TEST(Expected_MoveAssign_ShouldReplaceErrorWithValue_WhenSourceHasValue);
    RUN_TEST(Expected_MoveAssign_ShouldKeepValue_WhenSelfAssigned);
    RUN_TEST(Expected_BoolConversion_ShouldBeTrue_WhenHasValue);
    RUN_TEST(Expected_BoolConversion_ShouldBeFalse_WhenHasError);
    RUN_TEST(Expected_Dereference_ShouldReturnValue_WhenHasValue);
    RUN_TEST(Expected_ConstAccessors_ShouldReturnValueAndError);
    RUN_TEST(Expected_Destroy_ShouldDestroyValue_WhenScopeEnds);
    RUN_TEST(Expected_Copy_ShouldDestroyEachCopy_WhenScopeEnds);

    /*****************************************
     * lib-utility.h
     *****************************************/
    RUN_TEST(Move_ShouldNotMutateSource_WhenResultIsNotConsumed);
    RUN_TEST(Move_ShouldSelectMoveConstructor_WhenInitializingFromMovedLvalue);
    RUN_TEST(Move_ShouldSelectMoveAssignment_WhenAssigningFromMovedLvalue);
    RUN_TEST(Move_ShouldSelectCopyConstructor_WhenSourceIsConst);
    RUN_TEST(Forward_ShouldSelectCopyConstructor_WhenForwardingLvalue);
    RUN_TEST(Forward_ShouldSelectMoveConstructor_WhenForwardingRvalue);
    RUN_TEST(Forward_ShouldPreserveAddress_WhenForwardingLvalueReference);
    RUN_TEST(RemoveReference_ShouldYieldValueType_WhenGivenValueOrReference);
    RUN_TEST(Decay_ShouldStripReferenceAndCv_WhenGivenQualifiedType);

    /*****************************************
     * ArduinoTimer
     *****************************************/
    RUN_TEST(ArduinoTimer_Ctor_ShouldBeStopped_WhenDefaultConstructed);
    RUN_TEST(ArduinoTimer_ConstAccessors_ShouldReadState_WhenNotStarted);
    RUN_TEST(ArduinoTimer_SetInterval_ShouldUpdateInterval_WhenNotStarted);
    RUN_TEST(ArduinoTimer_SetInterval_ShouldKeepInterval_WhenStarted);
    RUN_TEST(ArduinoTimer_IsElapsed_ShouldBeTrueImmediately_WhenIntervalIsZero);
    RUN_TEST(ArduinoTimer_IsElapsed_ShouldBecomeTrue_AfterInterval);
    RUN_TEST(ArduinoTimer_IsElapsed_ShouldStayTrue_UntilStopped);
    RUN_TEST(ArduinoTimer_Stop_ShouldClearStartedAndElapsed_WhenStopped);
    RUN_TEST(ArduinoTimer_Start_ShouldRestartCountdown_WhenAlreadyElapsed);
    RUN_TEST(ArduinoTimer_Start_ShouldRunNewInterval_WhenRestartedAfterStop);
    
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