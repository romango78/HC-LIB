// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file AnalogPortAdapter.h
/// @brief Analog port adapter implementation.
#ifndef _HC_LIB_ANALOG_PORT_ADAPTER_H_
#define _HC_LIB_ANALOG_PORT_ADAPTER_H_

#include "IPortAdapter.h"

/// @brief Analog port adapter implementation.
/// @tparam T The type of the value to read or write.
class AnalogPortAdapter : public IPortAdapter<int>
{
    public:
        /// @brief Initializes the adapter with the given pin.
        AnalogPortAdapter() = delete;

        /// @brief Initializes the adapter with the given pin.
        /// @param t_pin The pin to initialize the adapter with.
        AnalogPortAdapter(const uint8_t t_pin) 
            : IPortAdapter(t_pin) {};

        /// @brief Destructor.
        virtual ~AnalogPortAdapter() = default;

        /// @brief Sets the input mode for the pin.        
        void setInputMode() override;
        
        /// @brief Sets the output mode for the pin.
        void setOutputMode() override;

        /// @brief Reads the value from the pin.
        /// @return The value read from the pin.
        int read() override;

        /// @brief Writes the value to the pin.
        /// @param t_value The value to write to the pin.
        void write(const int t_value) override;

        /// @brief Gets the state of the pin.
        /// @return The state of the pin.
        uint8_t getState() override;

        /// @brief Clones the adapter.
        /// @return A pointer to the cloned adapter.

        IPortAdapter<int>* clone() const override;
};

#endif