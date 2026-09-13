// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file DigitalDevices.h
/// @brief Digital devices implementation.
#ifndef _HC_LIB_DIGITAL_DEVICES_H_
#define _HC_LIB_DIGITAL_DEVICES_H_

#include "devices/Device.h"
#include "stream/DigitalStream.h"
#include <new>

/// @brief Digital device implementation.
/// @tparam T The type of the value to read or write.
struct DigitalDevice : public IDevice
{
private:
    /// @brief Whether the device is disposed.
    bool m_isDisposed = false;

    /// @brief Clones the stream.
    /// @param source The source device to clone the stream from.
    /// @return A pointer to the cloned stream.
    DigitalStream* clone_stream(const DigitalDevice& source) const
    {
        if(!source.m_isDisposed && source.stream)
        {
            return reinterpret_cast<DigitalStream*>(source.stream->clone());
        }
        return nullptr;
    }

public:
    /// @brief The pin number of the device.
    const uint8_t pin;

    /// @brief The stream of the device.
    DigitalStream* const stream;

    /// @brief Initializes the device.
    DigitalDevice() = delete;

    /// @brief Initializes the device with the given type, pin, and stream.
    /// @param t_type The type of the device.
    /// @param t_pin The pin number of the device.
    /// @param t_stream The stream of the device.
    /// @return A pointer to the cloned device.
    DigitalDevice(const uint8_t t_type, const uint8_t t_pin, DigitalStream* const t_stream) 
        : IDevice(t_type, DeviceCategory::Digital), pin(t_pin), stream(t_stream) {};

    /// @brief Initializes the device with the given source device.
    /// @param source The source device to initialize the device from.
    /// @return A pointer to the cloned device.
    DigitalDevice(const DigitalDevice& source) noexcept
        : IDevice(source), pin(source.pin), stream(clone_stream(source)) 
        {
            m_isDisposed = false;
        };

    /// @brief Initializes the device with the given source device.
    /// @param source The source device to initialize the device from.
    /// @return A pointer to the cloned device.
    DigitalDevice(DigitalDevice&& source) noexcept
        : IDevice(source), pin(source.pin), stream(source.stream) 
        {
            source.m_isDisposed = true;
        };

    /// @brief Destructor.
    virtual ~DigitalDevice()
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
    DigitalDevice& operator= (const DigitalDevice& source) noexcept
    {
        if(this == &source)
        {
            return *this;
        }

        this->~DigitalDevice();
        new (this) DigitalDevice(source);

        return *this;
    }

    /// @brief Assigns the device with the given source device.
    /// @param source The source device to assign the device to.
    /// @return A pointer to the assigned device.
    DigitalDevice& operator= (const DigitalDevice&& source) = delete;    
};

/// @brief Relay device implementation.
/// @tparam T The type of the value to read or write.
struct RelayDevice : public DigitalDevice
{
protected:    
    /// @brief Initializes the device with the given type, pin, and stream.
    /// @param t_type The type of the device.
    /// @param t_pin The pin number of the device.
    /// @param t_stream The stream of the device.
    /// @return A pointer to the cloned device.
    RelayDevice(const uint8_t t_type, const uint8_t t_pin, DigitalStream* const t_stream) 
        : DigitalDevice(t_type, t_pin, t_stream) {};
public:
    /// @brief Initializes the device.
    RelayDevice() = delete;

    /// @brief Initializes the device with the given pin and stream.
    /// @param t_pin The pin number of the device.
    /// @param t_stream The stream of the device.
    /// @return A pointer to the cloned device.
    RelayDevice(const uint8_t t_pin, DigitalStream* const t_stream) 
        : RelayDevice(RELAY_DEVICE_TYPE, t_pin, t_stream) {};

    /// @brief Initializes the device with the given source device.
    /// @param source The source device to initialize the device from.
    /// @return A pointer to the cloned device.
    RelayDevice(const RelayDevice& source) noexcept
        : DigitalDevice(source) {};

    /// @brief Initializes the device with the given source device.
    /// @param source The source device to initialize the device from.
    /// @return A pointer to the cloned device.
    RelayDevice(RelayDevice&& source) noexcept
        : DigitalDevice(dynamic_cast<DigitalDevice&&>(source)) {};

    /// @brief Destructor.
    virtual ~RelayDevice() = default;

    /// @brief Assigns the device with the given source device.
    /// @param source The source device to assign the device to.
    /// @return A pointer to the assigned device.
    RelayDevice& operator= (const RelayDevice& source) noexcept
    {
        if(this != &source)
        {
            DigitalDevice::operator=(source);
        }
        return *this;
    }
};

#endif