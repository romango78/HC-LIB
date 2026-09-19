// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file board.h
/// @brief Board-specific definitions.
#ifndef _HC_LIB_BOARD_H_
#define _HC_LIB_BOARD_H_

#include <stdint.h>

#ifndef READ_VCC_CALIBRATION_CONST
/// @brief 1100 mV × 1024. Result of getSupplyVoltage() is millivolts (e.g. 5000).
#define READ_VCC_CALIBRATION_CONST 1126400L
#endif

#if defined(ARDUINO)
#include <Arduino.h>
#endif

namespace board
{
    /// @brief Measures AVcc using the 1.1 V bandgap as the ADC channel.
    /// @return Supply voltage in millivolts (for example 5000 on a 5 V Nano).
    /// @note Restores ADMUX (and ADCSRB MUX5 on Mega / 32U4). Enables the ADC
    /// if it was off. See docs/README.md.
    inline uint16_t getSupplyVoltage()
    {
#if defined(__AVR__)
        // Save mux / enable so analogRead() is not left on the bandgap.
        const uint8_t savedAdmux = ADMUX;
        const bool adcWasEnabled = bit_is_set(ADCSRA, ADEN);
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_AT90USB1286__)
        const uint8_t savedAdcsrb = ADCSRB;
#endif

        // ADEN: turn the ADC on. ADPS2:0 = 111 (÷128) if the core left ADPS at 0.
        ADCSRA |= _BV(ADEN);
        if ((ADCSRA & (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0))) == 0)
        {
            ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
        }

        // ADMUX: AVcc as Vref (REFS0), channel = 1.1 V Vbg. ADLAR stays 0 (10-bit right-adjust).
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
        ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);           // MUX[3:0] = 1110
#elif defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
        ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_AT90USB1286__)
        ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
        ADCSRB &= ~_BV(MUX5);   // MUX5=1 remaps the bandgap; Mega would read garbage
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        ADMUX = _BV(MUX5) | _BV(MUX0);                                    // Vbg on tinyx4
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        ADMUX = _BV(MUX3) | _BV(MUX2);                                    // MUX[3:0] = 1100 (Vbg)
#endif

        delay(2);   // Bandgap / AVcc reference settle after ADMUX change

        // ADSC: dummy conversion (first result after a mux change is undefined).
        ADCSRA |= _BV(ADSC);
        while (bit_is_set(ADCSRA, ADSC))
        {
        }
        (void)ADCL;   // Read ADCL first — latches ADCH
        (void)ADCH;   // Read ADCH — unlocks the pair

        // ADSC: measurement conversion. ADC = 1100 mV × 1024 / AVcc.
        ADCSRA |= _BV(ADSC);
        while (bit_is_set(ADCSRA, ADSC))
        {
        }
        uint16_t adc = ADCL;
        adc |= static_cast<uint16_t>(ADCH) << 8;

        ADMUX = savedAdmux;
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_AT90USB1286__)
        ADCSRB = savedAdcsrb;
#endif
        if (!adcWasEnabled)
        {
            ADCSRA &= ~_BV(ADEN);   // Leave ADEN as we found it
        }

        if (adc == 0)
        {
            return 0;
        }
        return static_cast<uint16_t>(READ_VCC_CALIBRATION_CONST / adc);

#else
        return 5000;   // No ADMUX; assume 5.0 V
#endif
    }
}

#endif
