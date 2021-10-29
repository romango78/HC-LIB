// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
//
// Why the code cannot be moved to cpp file: https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl

#ifndef _BASE_STREAM_H_
#define _BASE_STREAM_H_

#include "IStream.h"

template<typename T>
class BaseStream : IStream<T>
{
    private:
        err_t m_lastError;
        
    protected:
        uint8_t m_isInitialized;
        void resetLastError();
        void setLastError(err_t lastError);
    public:
        BaseStream() : m_lastError(NO_ERROR), m_isInitialized(UNDEF_MODE)
        {};
        virtual ~BaseStream() = default;

        virtual void begin(StreamMode t_mode) override;

        T read() override;
        void write(T t_data) override;
        void end() override;

        bool canRead() override;
        bool canWrite() override;

        bool hasError() override;
        err_t getLastError() override;
};

template<typename T>
void BaseStream<T>::begin(StreamMode t_mode)
{
    resetLastError();
    m_isInitialized = t_mode;
};

template<typename T>
T BaseStream<T>::read()
{ 
    resetLastError();
    return T();
};

template<typename T>
void BaseStream<T>::write(T t_data)
{
    resetLastError();
};

template<typename T>
void BaseStream<T>::end()
{
    resetLastError();
    m_isInitialized = UNDEF_MODE;
};

template<typename T>
bool BaseStream<T>::canRead()
{
    return m_isInitialized == READ_MODE;
};

template<typename T>
bool BaseStream<T>::canWrite()
{
    return m_isInitialized == WRITE_MODE;
};

template<typename T>
bool BaseStream<T>::hasError()
{
    return m_lastError != NO_ERROR;
};

template<typename T>
err_t BaseStream<T>::getLastError()
{
    return m_lastError;
};

template<typename T>
void BaseStream<T>::resetLastError()
{
    m_lastError = NO_ERROR;
};

template<typename T>
void BaseStream<T>::setLastError(err_t lastError)
{
    m_lastError = lastError;
};

#endif