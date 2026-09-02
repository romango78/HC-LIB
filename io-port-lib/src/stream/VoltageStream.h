// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file VoltageStream.h
/// @brief Voltage stream implementation.
#ifndef _HC_LIB_VOLTAGE_STREAM_H_
#define _HC_LIB_VOLTAGE_STREAM_H_

#include "AnalogStream.h"

/// @note ADC scale was taken from ATMega328P datasheet
#define ADC_SCALE 1024
#define V_REF 5.0f
#define PWM_MAX 255

/// @brief Voltage stream implementation.
/// @tparam T The type of the value to read or write.
class IVoltageStream : public AnalogStream
{
    protected:
        /// @brief Initializes the stream with the given adapter.
        /// @param t_adapter The adapter to initialize the stream with.
        /// @return A pointer to the cloned stream.
        IVoltageStream(IPortAdapter<int>* const t_adapter)
            : AnalogStream(t_adapter) {};
    public:        
        virtual ~IVoltageStream() = default;

        /// @brief Gets the voltage from the stream.
        /// @return The voltage from the stream.
        virtual float getVoltage() = 0;

        /// @brief Sets the PWM value for the stream.
        /// @param t_percentage The percentage to set the PWM value to.
        virtual void setPwm(const int t_percentage) = 0;
};

/// @brief Voltage stream implementation.
/// @tparam T The type of the value to read or write.
class VoltageStream : public IVoltageStream
{
    public:
        /// @brief Initializes the stream.
        VoltageStream() = delete;

        /// @brief Initializes the stream with the given adapter.
        /// @param t_adapter The adapter to initialize the stream with.
        /// @return A pointer to the cloned stream.
        VoltageStream(IPortAdapter<int>* const t_adapter) 
            : IVoltageStream(t_adapter) {};

        /// @brief Destructor.
        virtual ~VoltageStream() = default;

        /// @brief Gets the voltage from the stream.
        /// @return The voltage from the stream.
        float getVoltage() override;

        /// @brief Sets the PWM value for the stream.
        /// @param t_percentage The percentage to set the PWM value to.
        void setPwm(const int t_percentage) override; 

        /// @brief Clones the stream.
        /// @return A pointer to the cloned stream.
        IStream<uint16_t>* clone() const override;
};

#endif