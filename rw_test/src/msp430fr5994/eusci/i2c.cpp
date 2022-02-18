/*
 * i2c.cpp
 *
 *  Created on: Feb 3, 2022
 *      Author: sailedeer
 */

#include <cstdint>
#include <msp430.h>
#include <vector>
#include "../eusci.h"
#include "../gpio.h"
#include "i2c.h"

using namespace MSP430FR5994::eUSCI;

static const uint8_t NO_ERROR = 0;
static const uint8_t BUS_BUSY_ERROR = 1;
static const uint8_t RECEIVED_NACK = 2;

I2C::I2C(I2CHandle handle)
: eUSCIBase(static_cast<eUSCIHandle>(handle)), ctlw0(BaseAddress()), ctl0(BaseAddress()),
  ctl1(BaseAddress()), ctlw1(BaseAddress()), brw(BaseAddress()), br0(BaseAddress()), br1(BaseAddress()),
  statw(BaseAddress()), stat(BaseAddress()), bcnt(BaseAddress()), tbcnt(BaseAddress()), rx(BaseAddress()),
  tx(BaseAddress()), oaddr0(BaseAddress()), oaddr1(BaseAddress()), oaddr2(BaseAddress()), oaddr3(BaseAddress()),
  rxaddr(BaseAddress()), addrmask(BaseAddress()), saddr(BaseAddress()), ie(BaseAddress()), ifg(BaseAddress()),
  iv(BaseAddress()), _ownAddress(0), _mode(I2CMode::PRIMARY), _handle(_baseHandle - 4) {
    ctlw0.setBit(UCSWRST);
    ctlw0.setBit(UCMODE_3);
    ctlw0.clearBit(UCSLA10);
    ctlw0.clearBit(UCMM);
    ctlw0.setBit(UCMST);
    ctlw1.setBit(UCASTP_2);
    ctlw0.clearBit(UCSWRST);
}

void I2C::Write(uint8_t *buf, uint16_t n, uint8_t addr, bool autoStop = false) {
    if (autoStop) {
        SetAutoStopBytes(n);
    }

    // we're transmitting
    ctlw0.setBit(UCTR);

    // set start bit and wait for hardware to send address
    ctlw0.setBit(UCTXSTT);
    while (ctlw0.getBit(UCTXSTT)) { }

    // write our bytes
    for (uint16_t i = 0; i < n && !ifg.getBit(UCSTPIFG); i++) {
        // check the NACK flag
        if (ifg.getBit(UCNACKIFG)) {
            // set stop bit, and wait for stop condition
            ctlw0.setBit(UCTXSTP);
            while (ctlw0.getBit(UCTXSTP)) { }
            _lastOpResult = RECEIVED_NACK;
            return;

        }
        tx.set(buf[i]);
        while (!ifg.getBit(UCTXIFG0)) { }   // wait for byte to be sent
    }

    _lastOpResult = NO_ERROR;
}
void I2C::Read(uint8_t *buf, uint16_t n, uint8_t addr, bool autoStop = false) {
    if (autoStop) {
        SetAutoStopBytes(n);
    }

    // we're receiving
    ctlw0.clearBit(UCTR);
    saddr.set(addr);

    // set start bit and wait for hardware to send address
    ctlw0.setBit(UCTXSTT);
    while (ctlw0.getBit(UCTXSTT)) { }

    // read our bytes
    for (uint16_t i = 0; i < n && !ifg.getBit(UCSTPIFG); i++) {
        // check the NACK flag
        if (ifg.getBit(UCNACKIFG)) {
            // set stop bit, and wait for stop condition
            ctlw0.setBit(UCTXSTP);
            while (ctlw0.getBit(UCTXSTP)) { }
            _lastOpResult = RECEIVED_NACK;
            return;

        }
        while (!ifg.getBit(UCRXIFG0)) { }   // wait for byte to be received
        buf[i] = rx.get();
    }

    _lastOpResult = NO_ERROR;
}

void I2C::CombinedWriteRead(uint8_t *rxBuf, uint16_t rxBytes, uint8_t *txBuf, uint16_t txBytes, uint8_t addr) {
    if (statw.getBit(UCBBUSY)) {
        _lastOpResult = BUS_BUSY_ERROR;
        return;
    }

    // set auto-stop
    ctlw0.setBit(UCSWRST);
    tbcnt.set(rxBytes + txBytes);
    ctlw0.clearBit(UCSWRST);

    Write(txBuf, txBytes, addr);

    if (_lastOpResult == NO_ERROR) {
        Read(rxBuf, rxBytes, addr);
    }
    return;
}
