// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file DigitalStream.h
/// @brief Digital stream implementation.
#ifndef _HC_LIB_DIGITAL_STREAM_H_
#define _HC_LIB_DIGITAL_STREAM_H_

#include <inttypes.h>
#include "adapter/IPortAdapter.h"
#include "IPortStream.h"

/// @brief Digital stream implementation.
/// @tparam T The type of the value to read or write.
class DigitalStream : public IPortStream<uint8_t>
{
    private:
        IPortAdapter<uint8_t>* const m_adapter;
    protected:
        /// @brief Initializes the stream.
        DigitalStream() : m_adapter(nullptr) {};

    public:
        /// @brief Initializes the stream with the given adapter.
        /// @param t_adapter The adapter to initialize the stream with.
        /// @return A pointer to the cloned stream.
        DigitalStream(IPortAdapter<uint8_t>* t_adapter) 
            : IPortStream(), m_adapter(t_adapter) {};
        virtual ~DigitalStream()
        {
            if(m_adapter)
            {
                delete m_adapter;
            }
        };

        /// @brief Begins the stream.
        /// @param t_mode The mode to begin the stream in.
        void begin(const StreamMode t_mode) override;

        /// @brief Reads the value from the stream.
        /// @return The value read from the stream.
        uint8_t read() override;

        /// @brief Writes the value to the stream.
        /// @param t_data The value to write to the stream.
        void write(const uint8_t t_data) override;

        /// @brief Gets the state of the pin.
        /// @return The state of the pin.
        uint8_t getState() override;

        /// @brief Clones the stream.
        /// @return A pointer to the cloned stream.
        IStream<uint8_t>* clone() const override;
};

#endif