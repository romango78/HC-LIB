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
#include "unity_extensions.h"
#include "collections/generic/Queue.h"
#include "errors/GenericErrors.h"

void Queue_ShouldCreateDefaultQueue_And_EnqueueTenItems()
{
    Queue<uint8_t, 10> sut;
    bool status = true;
    // Act
    for(uint8_t i = 1; i <= 10; i++)
    {
        status &= sut.enqueue(i);
    }

    // Asserts
    TEST_ASSERT_TRUE_MESSAGE(status, F("No errors expected."));
    TEST_ASSERT_EQUAL_MESSAGE(10, sut.count(), F("Expected ten items in a queue."));
}

void Queue_ShouldCreateDefaultQueue_And_DequeueAllItems()
{
    // Arrange
    Queue<uint8_t, 16> sut;

    for(uint8_t i = 1; i <= 10; i++)
    {
        sut.enqueue(i);
    }

    // Act
    bool isEmpty = false;
    uint8_t count = 0;
    uint16_t sum = 0;
    uint8_t guard = 0;
    do
    {
        Expected<uint8_t, Error> result = sut.dequeue();
        if(result.hasValue())
        {
            sum += result.getValue();
            count++;
        }
        else
        {
            isEmpty = true;
        }
        ++guard;
    } while(!isEmpty && guard < 32);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, sut.count(), F("Expected all items are dequeue."));
    TEST_ASSERT_EQUAL_MESSAGE(10, count, F("Expected dequeue ten items."));
    TEST_ASSERT_EQUAL_MESSAGE(55, sum, F("Expected correct sum of dequeued items."));
}

void Queue_ShouldReturnError_WhenDequeueFromEmptyQueue()
{
    // Arrange
    Queue<uint8_t, 8> sut;

    // Act
    Expected<uint8_t, Error> result = sut.dequeue();

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, sut.count(), F("Expected empty queue."));
    TEST_ASSERT_EQUAL_MESSAGE(false, result.hasValue(), F("Expected no dequeued value."));
    TEST_ASSERT_TRUE_MESSAGE(result.getError() == GenericError::InvalidOperation, F("Expected 'invalid operation' error."));
}

void Queue_ShouldReturnFalse_WhenEnqueue_And_QueueIsFull()
{
    // Arrange
    Queue<uint8_t, 4> sut;

    // Act
    for(uint8_t i = 1; i <= 4; i++)
    {
        sut.enqueue(i);
    }

    // Asserts
    TEST_ASSERT_FALSE_MESSAGE(sut.enqueue(5), F("Expected enqueue to fail when full."));
    TEST_ASSERT_EQUAL_MESSAGE(4, sut.count(), F("Expected four items when full."));
}

void Queue_ShouldClearQueue()
{
    // Arrange
    Queue<uint8_t, 10> sut;
    for(uint8_t i = 1; i <= 10; i++)
    {
        sut.enqueue(i);
    }

    // Act
    sut.clear();

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, sut.count(), F("Expected empty queue."));
    TEST_ASSERT_TRUE_MESSAGE(sut.enqueue(1), F("Enqueue should succeed after clear."));
}

void Queue_ShouldClearEmptyQueue()
{
    // Arrange
    Queue<uint8_t, 8> sut;

    // Act
    sut.clear();

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, sut.count(), F("Expected empty queue."));
}

void Queue_ShouldPeekElementFromQueue()
{
    // Arrange
    Queue<uint8_t, 10> sut;
    for(uint8_t i = 1; i <= 10; i++)
    {
        sut.enqueue(i);
    }

    // Act
    Expected<uint8_t, Error> result1 = sut.peek();
    Expected<uint8_t, Error> result2 = sut.peek();

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(10, sut.count(), F("Expected that elements will be not removed."));
    TEST_ASSERT_TRUE_MESSAGE(result1.hasValue(), F("Expected peeked value first time."));
    TEST_ASSERT_TRUE_MESSAGE(result2.hasValue(), F("Expected peeked value second time."));
    TEST_ASSERT_EQUAL_MESSAGE(result1.getValue(), result2.getValue(), F("Expected that peeked values are same."));
}

void Queue_ShouldReturnError_WhenPeekFromEmptyQueue()
{
    // Arrange
    Queue<uint8_t, 8> sut;

    // Act
    Expected<uint8_t, Error> result = sut.peek();

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(0, sut.count(), F("Expected empty queue."));
    TEST_ASSERT_EQUAL_MESSAGE(false, result.hasValue(), F("Expected no dequeued value."));
    TEST_ASSERT_TRUE_MESSAGE(result.getError() == GenericError::InvalidOperation, F("Expected 'invalid operation' error."));
}

void Queue_ShouldPreserveOrder_WhenElementSizeIsLargerThanByte()
{
    // Arrange
    Queue<uint16_t, 8> sut;
    const uint16_t values[] = { 0x0102, 0x0304, 0x0506, 0x0708, 0x090A };

    // Act & Asserts
    for(uint8_t i = 0; i < 5; i++)
    {
        TEST_ASSERT_TRUE_MESSAGE(sut.enqueue(values[i]), F("Enqueue should succeed."));
    }

    TEST_ASSERT_EQUAL_MESSAGE(5, sut.count(), F("Expected five items."));

    for(uint8_t i = 0; i < 5; i++)
    {
        Expected<uint16_t, Error> result = sut.dequeue();
        TEST_ASSERT_TRUE_MESSAGE(result.hasValue(), F("Dequeue should return a value."));
        TEST_ASSERT_EQUAL_MESSAGE(values[i], result.getValue(), F("FIFO order and multi-byte values should be preserved."));
    }

    TEST_ASSERT_EQUAL_MESSAGE(0, sut.count(), F("Expected empty queue after dequeue."));
}

void Queue_ShouldPreserveOrder_WhenIndicesWrap()
{
    // Arrange
    Queue<uint8_t, 4> sut;

    // Asserts
    TEST_ASSERT_TRUE(sut.enqueue(1));
    TEST_ASSERT_TRUE(sut.enqueue(2));
    TEST_ASSERT_TRUE(sut.enqueue(3));
    TEST_ASSERT_TRUE(sut.enqueue(4));
    TEST_ASSERT_EQUAL(4, sut.count());

    TEST_ASSERT_EQUAL(1, sut.dequeue().getValue());
    TEST_ASSERT_EQUAL(2, sut.dequeue().getValue());
    TEST_ASSERT_EQUAL(2, sut.count());

    TEST_ASSERT_TRUE(sut.enqueue(5));
    TEST_ASSERT_TRUE(sut.enqueue(6));
    TEST_ASSERT_FALSE(sut.enqueue(7));
    TEST_ASSERT_EQUAL(4, sut.count());

    TEST_ASSERT_EQUAL(3, sut.dequeue().getValue());
    TEST_ASSERT_EQUAL(4, sut.dequeue().getValue());
    TEST_ASSERT_EQUAL(5, sut.dequeue().getValue());
    TEST_ASSERT_EQUAL(6, sut.dequeue().getValue());
    TEST_ASSERT_EQUAL(0, sut.count());
}

#endif
#endif
