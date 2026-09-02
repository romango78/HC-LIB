// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
//
// Why the code cannot be moved to cpp file: https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl

/// @file BaseStream.h
/// @brief Default IStream implementation: mode flags and last-error storage.
#ifndef _HC_LIB_BASE_STREAM_H_
#define _HC_LIB_BASE_STREAM_H_

#include "IStream.h"

/// @brief IStream with last-error and open-mode tracking.
/// @tparam T The value type transferred by the stream.
template<typename T>
class BaseStream : public IStream<T>
{
    protected:
        Error m_lastError;
        uint8_t m_mode;

        /// @brief Initializes a closed stream with no error.
        BaseStream()
            : m_lastError(to_error(GenericError::NoError)), m_mode(STREAM_MODE_UNDEF) {};

        /// @brief Clears the last error to GenericError::NoError.
        void resetLastError();

        /// @brief Stores _t_error_ as the last error.
        void setLastError(const Error& t_error);

        /// @brief Copies mode and last error into _t_clone_.
        /// @return _t_clone_.
        IStream<T>* clone(BaseStream<T> *t_clone) const;
    public:
        virtual ~BaseStream() = default;

        void begin(const StreamMode t_mode) override;

        T read() override;
        void write(const T t_data) override;
        void end() override;

        bool canRead() const override;
        bool canWrite() const override;

        bool hasError() const override;
        Error getLastError() const override;
};

template<typename T>
void BaseStream<T>::begin(const StreamMode t_mode)
{
    resetLastError();
    m_mode = static_cast<uint8_t>(t_mode);
};

template<typename T>
T BaseStream<T>::read()
{
    resetLastError();
    return T();
};

template<typename T>
void BaseStream<T>::write(const T t_data)
{
    resetLastError();
};

template<typename T>
void BaseStream<T>::end()
{
    resetLastError();
    m_mode = STREAM_MODE_UNDEF;
};

template<typename T>
bool BaseStream<T>::canRead() const
{
    return !(m_mode & STREAM_MODE_UNDEF) && !(m_mode & STREAM_MODE_WRITE);
};

template<typename T>
bool BaseStream<T>::canWrite() const
{
    return !(m_mode & STREAM_MODE_UNDEF) && (m_mode & STREAM_MODE_WRITE);
};

template<typename T>
bool BaseStream<T>::hasError() const
{
    return static_cast<bool>(m_lastError);
};

template<typename T>
Error BaseStream<T>::getLastError() const
{
    return m_lastError;
};

template<typename T>
void BaseStream<T>::resetLastError()
{
    m_lastError = to_error(GenericError::NoError);
};

template<typename T>
void BaseStream<T>::setLastError(const Error& t_error)
{
    m_lastError = t_error;
};

template<typename T>
IStream<T>* BaseStream<T>::clone(BaseStream<T> *t_clone) const
{
    if(t_clone)
    {
        t_clone->m_mode = m_mode;
        t_clone->m_lastError = m_lastError;
    }
    return t_clone;
}

#endif
