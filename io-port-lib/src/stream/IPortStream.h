// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file IPortStream.h
/// @brief Port stream with a readable pin state.
#ifndef _HC_LIB_I_PORT_STREAM_H_
#define _HC_LIB_I_PORT_STREAM_H_

#include "stream/BaseStream.h"

/// @brief Port stream with a readable pin state.
/// @tparam T The type of the value to read or write.
template<typename T>
class IPortStream : public BaseStream<T>
{
    protected:
        /// @brief Initializes the stream.
        IPortStream() 
            : BaseStream<T>() {};
    public:
        virtual ~IPortStream() = default;

        /// @brief Gets the state of the pin.
        /// @return The state of the pin.
        virtual uint8_t getState() = 0;
};

#endif