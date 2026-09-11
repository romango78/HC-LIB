// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file EnumeratorBaseTests.h
/// @brief Unity tests for EnumeratorBase{T, TKey} using a fixed array of keys.

#ifndef _ENUMERATOR_BASE_TESTS_H_
#define _ENUMERATOR_BASE_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "unity_extensions.h"
#include "collections/generic/IEnumerator.h"

/// @brief Test enumerator over a fixed array. Keys are 0-based indexes.
/// @tparam T Item type stored in the array.
/// @tparam SIZE Maximum number of items that can be added.
template <typename T, uint16_t SIZE>
class EnumeratorBaseTest : public EnumeratorBase<T, uint16_t>
{
    private:
        /// @brief Contiguous items. Valid indexes are 0 .. m_count - 1.
        T m_items[SIZE];
        /// @brief Number of items added with add().
        uint16_t m_count;
    protected:
        /// @brief Returns the key of the first item.
        /// @return 0 when the array is not empty; InvalidOperation when it is empty.
        Expected<uint16_t, Error> getHead() const override
        {
            if (m_count == 0)
            {
                return make_error(GenericError::InvalidOperation);
            }
            return 0;
        }
        
        /// @brief Returns the key after _t_key_.
        /// @param t_key Current item index.
        /// @return t_key + 1, or InvalidOperation when _t_key_ is the last item.
        Expected<uint16_t, Error> getNext(uint16_t t_key) const override
        {
            if (t_key + 1 >= m_count)
            {
                return make_error(GenericError::InvalidOperation);
            }
            return t_key + 1;
        }

        /// @brief Returns the item at _t_key_.
        /// @param t_key Index previously returned by getHead() or getNext().
        /// @return A copy of the stored item.
        T getByKey(uint16_t t_key) const override
        {
            return m_items[t_key];
        }
    public:
        /// @brief Creates an empty enumerator (before the first element).
        EnumeratorBaseTest()
            : EnumeratorBase<T, uint16_t>(), m_items{}, m_count(0)
        {}

        /// @brief Appends an item at the next free index.
        /// @param t_data Item to store. Caller must not add more than SIZE items.
        void add(T t_data)
        {
            m_items[m_count++] = t_data;
        }
};

/// @brief getCurrent() is InvalidOperation after reset(); the cursor is before the first item.
void EnumeratorBase_ShouldNotGetFirstItem_AfterReset()
{
    uint8_t values[] = {10, 20, 30};
    EnumeratorBaseTest<uint8_t, 3> sut;
    sut.add(values[0]);
    sut.add(values[1]);
    sut.add(values[2]);
    
    sut.reset();
    TEST_ASSERT_FALSE_MESSAGE(sut.getCurrent().hasValue(), F("Expected error, but got value"));
    TEST_ASSERT_TRUE_MESSAGE(sut.getCurrent().getError() == GenericError::InvalidOperation, F("Expected InvalidOperation error after reset"));
}

/// @brief After reset(), the first moveNext() positions the cursor on the first item.
void EnumeratorBase_ShouldGetFirstItem_AfterReset_AndOneCallMoveNext()
{
    int values[] = {10, 20, 30};
    EnumeratorBaseTest<int, 3> sut;
    sut.add(values[0]);
    sut.add(values[1]);
    sut.add(values[2]);
    
    sut.reset();
    sut.moveNext();
    TEST_ASSERT_TRUE_MESSAGE(sut.getCurrent().hasValue(), F("Expected value, but got error"));
    TEST_ASSERT_EQUAL_MESSAGE(values[0], sut.getCurrent().getValue(), F("Expected value of the first item, but got different value"));
}

/// @brief After moveNext() returns false, getCurrent() is InvalidOperation (past the last item).
void EnumeratorBase_ShouldNotGetLastItem_WhenMoveNextReturnFalse()
{
    int values[] = {10, 20, 30};
    EnumeratorBaseTest<int, 3> sut;
    sut.add(values[0]);
    sut.add(values[1]);
    sut.add(values[2]);
    
    sut.reset();
    do {} while (sut.moveNext());
    TEST_ASSERT_FALSE_MESSAGE(sut.getCurrent().hasValue(), F("Expected error, but got value"));
    TEST_ASSERT_TRUE_MESSAGE(sut.getCurrent().getError() == GenericError::InvalidOperation, F("Expected InvalidOperation error when all items are enumerated"));
}

/// @brief Construction leaves the cursor before the first item; the first moveNext() is enough.
void EnumeratorBase_ShouldGetFirstItem_AfterFirstMoveNext_WithoutReset()
{
    int values[] = {10, 20, 30};
    EnumeratorBaseTest<int, 3> sut;
    sut.add(values[0]);
    sut.add(values[1]);
    sut.add(values[2]);
    
    sut.moveNext();
    TEST_ASSERT_TRUE_MESSAGE(sut.getCurrent().hasValue(), F("Expected value, but got error"));
    TEST_ASSERT_EQUAL_MESSAGE(values[0], sut.getCurrent().getValue(), F("Expected value of the first item, but got different value"));
}

/// @brief A one-item collection: first moveNext() succeeds and getCurrent() returns that item.
void EnumeratorBase_ShouldEnumerate_WhenSingleItem()
{
    EnumeratorBaseTest<int, 1> sut;
    sut.add(7);
    sut.reset();

    TEST_ASSERT_TRUE_MESSAGE(sut.moveNext(), F("Expected take next item, but got false"));
    TEST_ASSERT_TRUE_MESSAGE(sut.getCurrent().hasValue(), F("Expected value, but got error"));
    TEST_ASSERT_EQUAL_MESSAGE(7, sut.getCurrent().getValue(), F("Expected value of the first item, but got different value"));
}

/// @brief getCurrent() is InvalidOperation immediately after construction, before moveNext().
void EnumeratorBase_ShouldNotGetItem_AfterInitializing_WithoutReset()
{
    int values[] = {10, 20, 30};
    EnumeratorBaseTest<int, 3> sut;
    sut.add(values[0]);
    sut.add(values[1]);
    sut.add(values[2]);
    
    TEST_ASSERT_FALSE_MESSAGE(sut.getCurrent().hasValue(), F("Expected error, but got value"));
    TEST_ASSERT_TRUE_MESSAGE(sut.getCurrent().getError() == GenericError::InvalidOperation, F("Expected InvalidOperation error after initializing without reset"));
}

/// @brief reset() on an empty collection leaves getCurrent() as InvalidOperation.
void EnumeratorBase_ShouldReset_WhenNoItemsToEnumerate()
{    
    EnumeratorBaseTest<int, 3> sut;

    sut.reset();   
    
    TEST_ASSERT_FALSE_MESSAGE(sut.getCurrent().hasValue(), F("Expected error, but got value"));
    TEST_ASSERT_TRUE_MESSAGE(sut.getCurrent().getError() == GenericError::InvalidOperation, F("Expected InvalidOperation error when no items to enumerate"));
}

/// @brief moveNext() on an empty collection is false; getCurrent() stays InvalidOperation.
void EnumeratorBase_ShouldMoveNext_WhenNoItemsToEnumerate()
{    
    EnumeratorBaseTest<int, 3> sut;

    sut.reset();

    TEST_ASSERT_FALSE_MESSAGE(sut.moveNext(), F("Expected no move next when no items to enumerate"));
    TEST_ASSERT_FALSE_MESSAGE(sut.getCurrent().hasValue(), F("Expected error when no items to enumerate"));
    TEST_ASSERT_TRUE_MESSAGE(sut.getCurrent().getError() == GenericError::InvalidOperation, F("Expected InvalidOperation error when no items to enumerate"));
}

/// @brief Walks three value-type items in insertion order.
void EnumeratorBase_ShouldEnumerate_ValueTypes()
{
    int values[] = {10, 20, 30};
    EnumeratorBaseTest<int, 3> sut;
    sut.add(values[0]);
    sut.add(values[1]);
    sut.add(values[2]);
    
    sut.reset();
    int index = 0;
    while (sut.moveNext())
    {
        TEST_ASSERT_TRUE_MESSAGE(sut.getCurrent().hasValue(), F("Expected value, but got error"));
        TEST_ASSERT_EQUAL_MESSAGE(values[index++], sut.getCurrent().getValue(), F("Expected value of the current item"));
    }
}

/// @brief Walks three const char* items in insertion order.
void EnumeratorBase_ShouldEnumerate_RefTypes()
{
    const char* values[] = {"T10", "T20", "T30"};
    EnumeratorBaseTest<const char*, 3> sut;
    sut.add(values[0]);
    sut.add(values[1]);
    sut.add(values[2]);
    
    sut.reset();
    int index = 0;
    while (sut.moveNext())
    {
        TEST_ASSERT_TRUE_MESSAGE(sut.getCurrent().hasValue(), F("Expected value, but got error"));
        TEST_ASSERT_EQUAL_STRING_MESSAGE(values[index++], sut.getCurrent().getValue(), F("Expected value of the current item"));
    }
}

#endif
#endif
