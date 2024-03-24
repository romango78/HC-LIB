// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

// TWI Control Register (TWCR)
// ---------------------------
// Bit 7 - TWINT: TWI Interrupt flag
// This bit is set HIGH when the TWI module has finished working in the background 
// and expects a response from the software. As long as the TWINT bit is set the SCL line 
// will be held low. This allows the software to ensure that the data is processed 
// before the next data bit is sent/received. The TWINT flag must be cleared by software 
// by writing a logic 1 to it. If interrupts are enabled and the TWIE flag is also set, 
// then the MCU will jump to the TWI interrupt vector when TWINT gets set.
//
// Bit 6 – TWEA: TWI Enable Acknowledge Bit
// The TWEA bit controls the generation of the acknowledged pulse. If the 
// TWEA bit is written to one, the ACK pulse is generated on the TWI bus. 
// By writing the TWEA bit to zero, the device can be disconnected from 
// the 2-wire serial bus temporarily.
// 
// Bit 5 - TWSTA: Start condition
// it's written to logic 1 when it is desired to transmit a START condition. 
// If the bus is not currently free, the TWI module will wait until a STOP is detected 
// before transmitting the START and taking control of the bus. After the START condition 
// has been sent, the TWSTA flag should be cleared by software.
//
// Bit 4 - TWSTO: Stop condition
// It's written to a logic 1 when it is desired to send a STOP condition. 
// Unlike the START condition, the TWSTO flag will be cleared after the STOP condition is sent.
//
// Bit 2 – TWEN: TWI Enable Bit
// The TWEN bit enables I2C / TWI operation and activates the TWI interface. 
// If this bit is written to zero, the TWI is switched off and all TWI 
// transmissions are terminated, regardless of any ongoing operation.
//
// Bit 0 – TWIE: TWI Interrupt Enable
// When this bit is written to one will cause the MCU to jump to the 
// TWI interrupt vector when the TWINT flag is set.

// TWI Status Register (TWSR)
// --------------------------
// Bits 3:7 (TWS3:TWS7) are the status code bits.
// The following status codes are possible:
// 0x08 - Start sent
//
// Bit 2 is reserved and is not used.
//
// Bits 0:1 (TWPS0:TWPS1) are the prescaler bits. The prescale value is a two bit binary number, 
// giving it a value between 0 and 4. This prescale value is the exponent to a base of 4 
// in the SCL frequency calculation.

// TWI Data Register (TWDR)
// -----------------------
// This is the sift register for sending and receiving data. In receive mode, 
// when data is available it can be read from this register. When in transmit mode, 
// a data byte is loaded into this register to be shifted along the data line.

#include "TwiUnit.h"

/*#if !defined(ARDUINO)

void twi_unit_enable() {}
void twi_unit_disable() {}
void twi_unit_setAddress(const uint8_t t_address) {}
err_t twi_unit_start(const uint8_t t_address, const uint8_t t_readWriteMode)
{
    return NO_ERROR;
}
void twi_unit_stop() {}

#endif

#if defined(ARDUINO)*/

#include <Arduino.h>
#include <compat/twi.h>
#include "TwiBuffer.h"

// 100kHz
#define SCL_FREQ 100000L
#define GetTWBR(prescale) (((F_CPU / SCL_FREQ) - 16) / (2 * prescale))

// A timeout value in microseconds, if zero then timeout checking is disabled.
#define TWI_TIMEOUT 25000

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// Set TWI Enable Bit, clear TWI Interrupt flag and 
// enable interrupts
#define _TWTransmit (_BV(TWEN) | _BV(TWINT) | _BV(TWIE))

// Shared variables
static volatile uint8_t g_twi_state = TWI_DISABLED_STATE;

// Events
static void (*DataReceivedEvent) ();
static void (*DataSentEvent) ();
static void (*DataRequestingEvent) ();

bool is_timeout_occurred(uint32_t start)
{
    return (TWI_TIMEOUT > 0ul) && ((micros() - start) > TWI_TIMEOUT);
}

bool is_request_processed()
{
    return TWCR & _BV(TWINT);
}

err_t get_error()
{
    if (TWCR & _BV(TWINT))
    {
        // Some error occurred
        return IO_TWI_ERROR + TW_STATUS;
    }
    else
    {
        // The timeout occurred and START signal was not set        
        return TIMEOUT_OCCURRED_TWI_ERROR;
    }
}

void wait()
{
    uint32_t start = micros();
    while(!is_request_processed() && !is_timeout_occurred(start));
}

void twi_unit_enable()
{
    if(g_twi_state != TWI_DISABLED_STATE)
    {
        return;
    }

    // Activate internal pullups for twi.
    digitalWrite(SCL, 1);
    digitalWrite(SDA, 1);

    // The SCL Frequency for TWI is calculated by the following formula:
    // SCL frequency = CPU Clock frequency / (16 + 2 * (TWBR) * (Prescaler Value))
        
    // Initialize twi prescaler
    // Set Bits 0:1 (TWPS0:TWPS1) of TWI Status Register (TWSR)
    //  TWPS1   TWPS0	Prescaler Value
    //  -----   -----   ---------------
    //  0	    0	    1
    //  0	    1	    4
    //  1	    0	    16
    //  1	    1	    64
    cbi(TWSR, TWPS0);
    cbi(TWSR, TWPS1);

    // Initialize TWI bit rate
    TWBR = GetTWBR(1);

    // Enable twi module, acks, and twi interrupt
    // Set: TWI Enable Acknowledge Bit, TWI Enable Bit
    TWCR = _BV(TWEA) | _BV(TWIE) | _BV(TWEN);

    g_twi_state = TWI_NON_ADDRESSED_STATE;
}

void twi_unit_disable()
{
    // Disable twi module, acks, and twi interrupt
    TWCR &= ~(_BV(TWEN) | _BV(TWIE) | _BV(TWEA));

    // Deactivate internal pullups for twi.
    digitalWrite(SDA, 0);
    digitalWrite(SCL, 0);

    g_twi_state = TWI_DISABLED_STATE;
}

uint8_t twi_unit_getState()
{
    return g_twi_state;
}

void twi_unit_setAddress(const uint8_t t_address)
{
    // Set twi slave address
    TWAR = t_address << 1;
}

err_t twi_unit_start(const uint8_t t_address, const uint8_t t_readWriteMode)
{
    if(g_twi_state != TWI_NON_ADDRESSED_STATE)
    {
        return INVALID_OPERATION_ERROR;
    }

    // Save slave device address and R/W bit in the static variable
    uint8_t slarw = 0x01 & t_readWriteMode;
    slarw |= t_address << 1;

    // A START condition is sent by writing the following value to TWCR:
    // TWEN must be set to enable the 2-wire Serial Interface, 
    // TWSTA must be written to one to transmit a START condition and 
    // TWINT must be written to one to clear the TWINT Flag.
    TWCR = _TWTransmit | _BV(TWSTA);

    // Wait when start condition will be executed on BUS
    // When START condition has been sent, TWINT Flag is set by hardware
    // and the status register TWSR will have a value of 0x08 in its upper 5 bits.
    wait();

    if(TW_STATUS == TW_START)
    {
        // In order to enter MR/MT mode, SLA+R/W must be transmitted.
        // Thereafter the TWINT flag should be cleared (by writing it to one) to continue the transfer. 
        TWDR = slarw;
        TWCR = _TWTransmit;
    }
    else 
    {
        return get_error();
    }

    // Wait for SLA+R/W has been transmitted.
    // When SLA+R/W have been transmitted and an acknowledgement bit 
    // has been received, TWINT is set again and check the status 
    // code in TWSR. 
    // Possible status codes in Master mode are 0x18 (0x40), 0x20 (0x48), or 0x38.
    wait();

    switch(TW_STATUS)
    {
        case TW_MT_SLA_ACK:
        case TW_MR_SLA_ACK:
            // SLA+R/W has been transmitted and ACK has been received
            g_twi_state = t_readWriteMode == TWI_READ_MODE 
                ? TWI_MR_MODE_STATE : TWI_MT_MODE_STATE;
            return NO_ERROR;
        case TW_MT_SLA_NACK:        
        case TW_MR_SLA_NACK:
            // SLA+R/W has been transmitted and NOT ACK has been received
            return DEVICE_NOT_FOUND_TWI_ERROR;
        case TW_MT_ARB_LOST: 
            //  Arbitration lost in SLA+R/W
            twi_unit_release();
            return ARBITRATION_LOST_TWI_ERROR;
    }

    return INVALID_OPERATION_ERROR;
}

err_t twi_unit_stop()
{
    if(g_twi_state != TWI_MT_MODE_STATE && g_twi_state != TWI_MR_MODE_STATE)
    {
        return INVALID_OPERATION_ERROR;
    }
    // A STOP condition is generated by writing the following value to TWCR:
    // TWEN must be set to enable the 2-wire Serial Interface, 
    // TWSTO must be written to one to transmit a STOP condition and 
    // TWINT must be written to one to clear the TWINT Flag.
    TWCR = _TWTransmit | _BV(TWSTO);

    wait();

    g_twi_state = TWI_NON_ADDRESSED_STATE;
    return NO_ERROR;
}

err_t twi_unit_transmitt(const uint8_t t_data)
{
    if(g_twi_state != TWI_MT_MODE_STATE)
    {
        return INVALID_OPERATION_ERROR;
    }
    // Copy data to DATA register and ack
    TWDR = t_data;
    TWCR = _TWTransmit;

    // Wait for data has been transmitted.
    // When data have been transmitted and an acknowledgement bit 
    // has been received, TWINT is set again and check the status 
    // code in TWSR. 
    // Possible status codes in Master mode are 0x28, 0x30 or 0x38.    
    wait();

    switch(TW_STATUS)
    {
        case TW_MT_DATA_ACK:
            // Data was transmitted and ACK has been received (0x28)
            return NO_ERROR;
        case TW_MT_DATA_NACK:
            // Data was not transmitted and NOT ACK has been received (0x30)
            return TRANSMITTION_TWI_ERROR;
        case TW_MT_ARB_LOST:
            // Arbitration lost in data bytes (0x38)
            twi_unit_release();    
            return ARBITRATION_LOST_TWI_ERROR;
    }

    return INVALID_OPERATION_ERROR;
}

void twi_unit_send_ack()
{
    // Set: TWI Enable Acknowledge Bit, TWI Enable Bit, TWI Interrupt Enable,
    // TWI Interrupt flag
    TWCR = _TWTransmit | _BV(TWEA);
}

void twi_unit_send_nack()
{
    // Set: TWI Enable Acknowledge Bit = 0, TWI Enable Bit, TWI Interrupt Enable,
    // TWI Interrupt flag
    TWCR = _TWTransmit;
}

void twi_unit_release()
{
    // Set: TWI Enable Acknowledge Bit, TWI Enable Bit, TWI Interrupt Enable,
    // TWI Interrupt flag
    TWCR = _TWTransmit | _BV(TWEA);

    g_twi_state = TWI_NON_ADDRESSED_STATE;
}

void twi_unit_onDataReceived(void (*handler) ())
{
    DataReceivedEvent = handler;
}

void twi_unit_onDataRequesting(void (*handler) ())
{
    DataRequestingEvent = handler;
}

void twi_unit_onDataSent(void (*handler) ())
{
    DataSentEvent = handler;
}

// Processing
void communicationFinishedHandler()
{
    auto state = g_twi_state;
    twi_unit_release();
    switch(state)
    {
        case TWI_ST_MODE_STATE:
            if(DataSentEvent)
            {
                DataSentEvent();
            }
            break;
        case TWI_SR_MODE_STATE:
            if(DataReceivedEvent)
            {
                DataReceivedEvent();
            }
            break;
    }    
}

void dataReadHandler()
{
    switch(g_twi_state)
    {
        case TWI_SR_MODE_STATE:
            {
                uint8_t data = TWDR;
                TwiBuffer.inboxWrite(data);
            }
            twi_unit_send_ack();
            break;
        default:
            twi_unit_send_nack();
            break;
    }
}

void dataTransmittHandler()
{
    switch(g_twi_state)
    {
        case TWI_ST_MODE_STATE:
            {
                auto data = TwiBuffer.outboxRead();
                if(data.hasValue())
                {
                    TWDR = data.getValue();
                    twi_unit_send_ack();
                }
                else
                {
                    twi_unit_send_nack();
                }
            }
            break;
        default:
            twi_unit_send_nack();
            break;                
    }    
}

void slaveReadyToReadHandler()
{
    switch(g_twi_state)
    {
        case TWI_NON_ADDRESSED_STATE:
            twi_unit_send_ack();
            g_twi_state = TWI_SR_MODE_STATE;
            break;
        default:
            twi_unit_send_nack();
            break;
    }
}

void slaveReadyToTransmittHandler()
{
    switch(g_twi_state)
    {
        case TWI_NON_ADDRESSED_STATE:
            twi_unit_send_ack();
            g_twi_state = TWI_ST_MODE_STATE;
            if(DataRequestingEvent)
            {
                DataRequestingEvent();
            }
            break;
        default:
            twi_unit_send_nack();
            break;
    }
}

// Interrupt 
ISR(TWI_vect)
{
    switch(TW_STATUS)
    {
        /////////////////////////
        // Slave Receiver Mode //
        /////////////////////////

        // Arbitration lost in SLA+R/W as Master
        case TW_SR_ARB_LOST_GCALL_ACK:
        // General call address has been received; 
        // ACK has been returned (0x70)
        case TW_SR_GCALL_ACK:
            if (!(TWAR & _BV(TWGCE)))
            {
                // Slave doesn't initialized to work with General call
                twi_unit_send_nack();
                break;
            }
        // Arbitration lost in SLA+R/W as Master (0x68)
        case TW_SR_ARB_LOST_SLA_ACK:
        // Own SLA+W has been received
        // and ACK was returned (0x60)
        case TW_SR_SLA_ACK:
            slaveReadyToReadHandler();
            break;
        // Previously addressed with general call; 
        // data has been received; ACK has been returned (0x90)
        case TW_SR_GCALL_DATA_ACK:
            if (!(TWAR & _BV(TWGCE)))
            {
                // Slave doesn't initialized to work with General call
                twi_unit_send_nack();
                break;
            }
        // Previously addressed with own SLA+W; data has been received
        // and ACK was returned (0x80)
        case TW_SR_DATA_ACK:
            dataReadHandler();
            break;
        // Previously addressed with general call; 
        // data has been received; NOT ACK has been returned (0x98)
        case TW_SR_GCALL_DATA_NACK:
        // Previously addressed with own SLA+W; data has been received
        // and NOT ACK was returned (0x88)
        case TW_SR_DATA_NACK:
            twi_unit_send_nack();
            g_twi_state = TWI_NON_ADDRESSED_STATE;
            break;
        // A STOP condition or repeated START condition has been received
        // while still addressed as Slave (0xA0)
        case TW_SR_STOP:
            communicationFinishedHandler();
            break;

        ////////////////////////////
        // Slave Transmitter Mode //
        ////////////////////////////

        // Arbitration lost in SLA+R/W as Master (0xB0)
        case TW_ST_ARB_LOST_SLA_ACK:
        // Own SLA+R has been received
        // and ACK was returned (0xA8)
        case TW_ST_SLA_ACK:
            slaveReadyToTransmittHandler();
            break;
        // Data byte in TWDR has been transmitted; 
        // ACK has been received (0xB8)
        case TW_ST_DATA_ACK:
            dataTransmittHandler();
            break;
        // Data byte in TWDR has been transmitted; 
        // NOT ACK has been received (0xC0)
        case TW_ST_DATA_NACK:
        // Last data byte in TWDR has been
        // transmitted (TWEA = “0”); ACK
        // has been received (0xC8)
        case TW_ST_LAST_DATA:
            communicationFinishedHandler();
            break;
    }
}

//#endif