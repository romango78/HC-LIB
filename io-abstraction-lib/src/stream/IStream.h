// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file IStream.h
/// @brief Stream mode tags and the IStream abstraction.
#ifndef _HC_LIB_I_STREAM_H_
#define _HC_LIB_I_STREAM_H_

#include <stdint.h>
#include "ICloneable.h"
#include "errors/IoErrors.h"

#define STREAM_MODE_UNDEF 0x80
#define STREAM_MODE_READ  0x00
#define STREAM_MODE_WRITE 0x01

/// @brief Direction in which a stream is opened.
enum class StreamMode : uint8_t
{
    Read = STREAM_MODE_READ,
    Write = STREAM_MODE_WRITE
};

/// @brief Readable and writable stream of values of type _T_.
/// @tparam T The value type transferred by the stream.
/// @note Errors are stored as the last _Error_ (see _hasError_ / _getLastError_).
template<typename T>
class IStream : public ICloneable<IStream<T>>
{
    protected:
        IStream() = default;
    public:
        virtual ~IStream() = default;

        /// @brief Opens the stream in _t_mode_.
        virtual void begin(const StreamMode t_mode) = 0;

        /// @brief Reads the next value.
        virtual T read() = 0;

        /// @brief Writes _t_data_.
        virtual void write(const T t_data) = 0;

        /// @brief Closes the stream.
        virtual void end() = 0;

        /// @brief True when the stream is open for reading.
        virtual bool canRead() const = 0;

        /// @brief True when the stream is open for writing.
        virtual bool canWrite() const = 0;

        /// @brief True when the last operation stored a non-zero error.
        virtual bool hasError() const = 0;

        /// @brief Returns the last stored error. Code 0 means no error.
        virtual Error getLastError() const = 0;
};

#endif
