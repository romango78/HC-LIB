// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ENUMERATOR_BASE_TESTS_H_
#define _ENUMERATOR_BASE_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "generic/IEnumerator.h"

template <class T>
class EnumeratorBaseTest : public EnumeratorBase<T>
{
    private:
        EnumeratedItem<T> *m_top;
    public:
        EnumeratorBaseTest() 
        {
            m_top = nullptr;
        };

        ~EnumeratorBaseTest() {};

        void add(T t_data)
        {
            EnumeratedItem<T> *item = new EnumeratedItem<T>();
            item->item = t_data;
            item->next = nullptr;
            if(!m_top)
            {
                m_top = item;
                this->m_head = m_top;
            }
            else
            {
                m_top->next = item;
                m_top = item;
            }
        };
};

void EnumeratorBase_ShouldNotGetFirstItem_AfterReset()
{
    int values[] = {10,20,30};
    EnumeratorBaseTest<int> *sut = new EnumeratorBaseTest<int>();
    sut->add(values[0]);
    sut->add(values[1]);
    sut->add(values[2]);
    
    sut->reset();
    TEST_ASSERT_EQUAL(0, sut->getCurrent());

    delete sut;
};

void EnumeratorBase_ShouldGetFirstItem_AfterReset_AndOneCallMoveNext()
{
    int values[] = {10,20,30};
    EnumeratorBaseTest<int> *sut = new EnumeratorBaseTest<int>();
    sut->add(values[0]);
    sut->add(values[1]);
    sut->add(values[2]);
    
    sut->reset();
    sut->moveNext();
    TEST_ASSERT_EQUAL(values[0], sut->getCurrent());

    delete sut;
};

void EnumeratorBase_ShouldNotGetLastItem_WhenMoveNextReturnFalse()
{
    int values[] = {10,20,30};
    EnumeratorBaseTest<int> *sut = new EnumeratorBaseTest<int>();
    sut->add(values[0]);
    sut->add(values[1]);
    sut->add(values[2]);
    
    sut->reset();
    do{} while(sut->moveNext());
    TEST_ASSERT_EQUAL(0, sut->getCurrent());

    delete sut;
};

void EnumeratorBase_ShouldNotGetItem_AfterFirstMoveNext_WithoutReset()
{
    int values[] = {10,20,30};
    EnumeratorBaseTest<int> *sut = new EnumeratorBaseTest<int>();
    sut->add(values[0]);
    sut->add(values[1]);
    sut->add(values[2]);
    
    sut->moveNext();
    TEST_ASSERT_EQUAL(0, sut->getCurrent());

    delete sut;
};

void EnumeratorBase_ShouldNotGetItem_AfterInitializing_WithoutReset()
{
    int values[] = {10,20,30};
    EnumeratorBaseTest<int> *sut = new EnumeratorBaseTest<int>();
    sut->add(values[0]);
    sut->add(values[1]);
    sut->add(values[2]);
    
    TEST_ASSERT_EQUAL(0, sut->getCurrent());

    delete sut;
};

void EnumeratorBase_ShouldReset_WhenNoItemsToEnumerate()
{    
    EnumeratorBaseTest<int> *sut = new EnumeratorBaseTest<int>();

    sut->reset();   
    TEST_ASSERT_EQUAL(0, sut->getCurrent());

    delete sut;
};

void EnumeratorBase_ShouldMoveNext_WhenNoItemsToEnumerate()
{    
    EnumeratorBaseTest<int> *sut = new EnumeratorBaseTest<int>();

    sut->reset();
    TEST_ASSERT_EQUAL(false, sut->moveNext());
    TEST_ASSERT_EQUAL(0, sut->getCurrent());

    delete sut;
};

void EnumeratorBase_ShouldEnumerate_ValueTypes()
{
    int values[] = {10,20,30};
    EnumeratorBaseTest<int> *sut = new EnumeratorBaseTest<int>();
    sut->add(values[0]);
    sut->add(values[1]);
    sut->add(values[2]);
    
    sut->reset();
    int index = 0;
    while(sut->moveNext())
    {
        TEST_ASSERT_EQUAL(values[index++], sut->getCurrent());
    };    

    delete sut;
};

void EnumeratorBase_ShouldEnumerate_RefTypes()
{
    char* values[] = {"T10","T20","T30"};
    EnumeratorBaseTest<char*> *sut = new EnumeratorBaseTest<char*>();
    sut->add(values[0]);
    sut->add(values[1]);
    sut->add(values[2]);
    
    sut->reset();
    TEST_ASSERT_EQUAL_STRING(nullptr, sut->getCurrent());
    int index = 0;
    while(sut->moveNext())
    {
        TEST_ASSERT_EQUAL_STRING(values[index++], sut->getCurrent());
    };    

    delete sut;
};

#endif
#endif