// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file IPortAdapter.h
/// @brief Hardware pin adapter abstraction.
#ifndef _HC_LIB_I_PORT_ADAPTER_H_
#define _HC_LIB_I_PORT_ADAPTER_H_

#include <inttypes.h>
#include "ICloneable.h"

#define NO_DATA 0

/// @brief Hardware pin adapter abstraction.
/// @tparam T The type of the value to read or write.
template<typename T>
class IPortAdapter : public ICloneable<IPortAdapter<T>>
{
    protected:
        const uint8_t m_pin;

        /// @brief Initializes the adapter with the given pin.
        IPortAdapter(const uint8_t t_pin) : m_pin(t_pin) {};
    public:
        virtual ~IPortAdapter() = default;

        /// @brief Sets the input mode for the pin.
        virtual void setInputMode() = 0;

        /// @brief Sets the output mode for the pin.
        virtual void setOutputMode() = 0;

        /// @brief Reads the value from the pin.
        virtual T read() = 0;

        /// @brief Writes the value to the pin.
        virtual void write(const T t_value) = 0;

        /// @brief Gets the state of the pin.
        virtual uint8_t getState() = 0;

        /// @brief Clones the adapter.
        virtual IPortAdapter<T>* clone() const = 0;
};

#endif