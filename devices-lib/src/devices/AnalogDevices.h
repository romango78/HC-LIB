// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file AnalogDevices.h
/// @brief Analog devices implementation.
#ifndef _HC_LIB_ANALOG_DEVICES_H_
#define _HC_LIB_ANALOG_DEVICES_H_

#include "devices/Device.h"
#include "stream/AnalogStream.h"
#include <new>

/// @brief Analog device implementation.
/// @tparam T The type of the value to read or write.
struct AnalogDevice : public IDevice
{
private:
    /// @brief Whether the device is disposed.
    bool m_isDisposed = false;

    /// @brief Clones the stream.
    /// @param source The source device to clone the stream from.
    /// @return A pointer to the cloned stream.
    AnalogStream* clone_stream(const AnalogDevice& source) const
    {
        if(!source.m_isDisposed && source.stream)
        {
            return reinterpret_cast<AnalogStream*>(source.stream->clone());
        }
        return nullptr;
    }

public:        
    /// @brief The pin number of the device.
    const uint8_t pin;

    /// @brief The stream of the device.
    AnalogStream* const stream;

    /// @brief Initializes the device.
    AnalogDevice() = delete;

    /// @brief Initializes the device with the given type, pin, and stream.
    /// @param t_type The type of the device.
    /// @param t_pin The pin number of the device.
    /// @param t_stream The stream of the device.
    /// @return A pointer to the cloned device.
    AnalogDevice(const uint8_t t_type, const uint8_t t_pin, AnalogStream* const t_stream) 
        : IDevice(t_type, DeviceCategory::Analog), pin(t_pin), stream(t_stream) {};

    /// @brief Initializes the device with the given source device.
    /// @param source The source device to initialize the device from.
    /// @return A pointer to the cloned device.
    AnalogDevice(const AnalogDevice& source) noexcept
        : IDevice(source), pin(source.pin), stream(clone_stream(source)) 
        {
            m_isDisposed = false;
        };

    /// @brief Initializes the device with the given source device.
    /// @param source The source device to initialize the device from.
    /// @return A pointer to the cloned device.
    AnalogDevice(AnalogDevice&& source) noexcept
        : IDevice(source), pin(source.pin), stream(source.stream) 
        {
            source.m_isDisposed = true;
        };

    /// @brief Destructor.
    virtual ~AnalogDevice()
    {
        if(!m_isDisposed && stream)
        {
            delete stream;
        }
        m_isDisposed = true;
    }

    /// @brief Assigns the device with the given source device.
    /// @param source The source device to assign the device to.
    /// @return A pointer to the assigned device.
    AnalogDevice& operator= (const AnalogDevice& source) noexcept
    {
        if(this == &source)
        {
            return *this;
        }

        this->~AnalogDevice();
        new (this) AnalogDevice(source);

        return *this;
    }

    /// @brief Assigns the device with the given source device.
    /// @param source The source device to assign the device to.
    /// @return A pointer to the assigned device.
    AnalogDevice& operator= (const AnalogDevice&& source) = delete;
};

#endif