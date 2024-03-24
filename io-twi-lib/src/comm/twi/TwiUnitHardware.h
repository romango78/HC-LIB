// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _TWI_UNIT_HARDWARE_H_
#define _TWI_UNIT_HARDWARE_H_

#include <inttypes.h>
#include "iotwierrdef.h"

#define TWI_DISABLED_STATE 0
#define TWI_NON_ADDRESSED_STATE 1
#define TWI_MT_MODE_STATE 2
#define TWI_MR_MODE_STATE 3
#define TWI_ST_MODE_STATE 4
#define TWI_SR_MODE_STATE 5

#define TWI_WRITE_MODE 0
#define TWI_READ_MODE 1

void twi_unit_enable();
void twi_unit_disable();
void twi_unit_release();
uint8_t twi_unit_getState();

void twi_unit_setAddress(const uint8_t t_address);

err_t twi_unit_start(const uint8_t t_address, const uint8_t t_readWriteMode);
err_t twi_unit_stop();

err_t twi_unit_transmitt(const uint8_t t_data);

void twi_unit_send_ack();
void twi_unit_send_nack();

// Register events
void twi_unit_onDataReceived(void (*) ());
void twi_unit_onDataRequesting(void (*) ());
void twi_unit_onDataSent(void (*) ());

#endif