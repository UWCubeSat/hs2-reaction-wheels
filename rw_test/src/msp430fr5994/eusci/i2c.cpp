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
#include "i2c.h"

using namespace MSP430FR5994::eUSCI;

I2C::I2C(I2CHandle handle, I2CMode mode)
: _baseAddress(baseAddress), {

    if (mode == I2CMode::PRIMARY) {
        // set up to be the primary device on the bus
    } else {
        // set up to be a secondary device on the bus
    }

}

bool I2C::Begin() {
    // make sure the bus is actually initialized
    return false;
}

void I2C::Write(uint8_t data, uint8_t addr) {

}

void I2C::Write(uint8_t *buf, size_t n, uint8_t addr) {

}

uint8_t I2C::Read(uint8_t addr) {

}

void I2C::Read(std::vector<uint8_t> &buf, uint8_t addr) {

}
