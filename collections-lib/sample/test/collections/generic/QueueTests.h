// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _QUEUE_TESTS_H_
#define _QUEUE_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "collections/generic/Queue.h"

void Queue_ShouldCreateDefaultQueue_And_EnqueueTenItems()
{
    // Arrange
    Queue<uint8_t>* sut = new Queue<uint8_t>();

    // Act
    int error = 0;
    for(uint8_t i = 1; i<=10; i++)
    {
        error += sut->enqueue(i);
    }

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, error, "No errors expected.");
    TEST_ASSERT_EQUAL_MESSAGE(10, sut->count(), "Expected ten items in a queue.");

    delete sut;
}

void Queue_ShouldCreateDefaultQueue_And_DequeueAllItems()
{
    // Arrange
    Queue<uint8_t>* sut = new Queue<uint8_t>();

    for(uint8_t i = 1; i<=10; i++)
    {
        sut->enqueue(i);
    }

    // Act
    bool isEmpty = false;;
    uint8_t count = 0;
    uint16_t sum = 0;
    do
    {
        Expected<uint8_t> result = sut->dequeue();
        if(result.hasValue())
        {
            sum += result.getValue();
            count++;
        }
        else
        {
            isEmpty = true;
        }
    } while(!isEmpty);
    
    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, sut->count(), "Expected all items are dequeue.");
    TEST_ASSERT_EQUAL_MESSAGE(10, count, "Expected dequeue ten items.");
    TEST_ASSERT_EQUAL_MESSAGE(55, sum, "Expected correct sum of dequeued items.");
    
    delete sut;
}

void Queue_ShouldReturnError_WhenDequeueFromEmptyQueue()
{
    // Arrange
    Queue<uint8_t>* sut = new Queue<uint8_t>();

    // Act
    Expected<uint8_t> result = sut->dequeue();

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, sut->count(), "Expected empty queue.");
    TEST_ASSERT_EQUAL_MESSAGE(false, result.hasValue(), "Expected no dequeued value.");
    TEST_ASSERT_EQUAL_MESSAGE(INVALID_OPERATION_ERROR, result.getError(), "Expected 'invalid operation' error.");

    delete sut;
}

void Queue_ShouldReturnError_WhenEnqueue_And_InsufficientMemory()
{
    // Arrange
    Queue<uint8_t>* sut = new Queue<uint8_t>();

    // Act
    err_t result;
    do
    {
        result = sut->enqueue(0xFF);
    } while(result == NO_ERROR);

    // Asserts
    TEST_ASSERT_LESS_OR_EQUAL_MESSAGE(UINT16_MAX, sut->count(), "Expected queue size less or equal of UINT16_MAX.");
    TEST_ASSERT_EQUAL_MESSAGE(INVALID_OPERATION_ERROR, result, "Expected 'invalid operation' error.");

    delete sut;
}

void Queue_ShouldClearQueue()
{
    // Arrange
    Queue<uint8_t>* sut = new Queue<uint8_t>();
    for(uint8_t i = 1; i<=10; i++)
    {
        sut->enqueue(i);
    }

    // Act
    sut->clear();

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, sut->count(), "Expected empty queue.");

    delete sut;
}

void Queue_ShouldClearEmptyQueue()
{
    // Arrange
    Queue<uint8_t>* sut = new Queue<uint8_t>();

    // Act
    sut->clear();

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, sut->count(), "Expected empty queue.");

    delete sut;
}

void Queue_ShouldPeekElementFromQueue()
{
    // Arrange
    Queue<uint8_t>* sut = new Queue<uint8_t>();
    for(uint8_t i = 1; i<=10; i++)
    {
        sut->enqueue(i);
    }

    // Act
    Expected<uint8_t> result1 = sut->peek();
    Expected<uint8_t> result2 = sut->peek();

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(10, sut->count(), "Expected that elements will be not removed.");
    TEST_ASSERT_TRUE_MESSAGE(result1.hasValue(), "Expected peeked value first time.");
    TEST_ASSERT_TRUE_MESSAGE(result2.hasValue(), "Expected peeked value second time.");
    TEST_ASSERT_EQUAL_MESSAGE(result1.getValue(), result2.getValue(), "Expected that peeked values are same.");

    delete sut;
}

void Queue_ShouldReturnError_WhenPeekFromEmptyQueue()
{
    // Arrange
    Queue<uint8_t>* sut = new Queue<uint8_t>();

    // Act
    Expected<uint8_t> result = sut->peek();

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, sut->count(), "Expected empty queue.");
    TEST_ASSERT_EQUAL_MESSAGE(false, result.hasValue(), "Expected no dequeued value.");
    TEST_ASSERT_EQUAL_MESSAGE(INVALID_OPERATION_ERROR, result.getError(), "Expected 'invalid operation' error.");

    delete sut;
}

#endif
#endif