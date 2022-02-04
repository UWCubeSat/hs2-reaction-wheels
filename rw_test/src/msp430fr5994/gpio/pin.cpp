/*
 * pin.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: sailedeer
 */

#include "pin.h"

using namespace MSP430FR5994::GPIO;

static uint16_t portAddresses[9] = {
    P1_BASE,
    P2_BASE + 0x1,
    P3_BASE,
    P4_BASE + 0x1,
    P5_BASE,
    P6_BASE + 0x1,
    P7_BASE,
    P8_BASE + 0x1,
    PJ_BASE
};

static inline uint8_t countBitsInByte(uint8_t s) {
    uint8_t count = 0;
    while (s) {
        count += (s & 0x1);
        s >>= 1;
    }
    return count;
}

Pin::Pin(uint8_t portHandle, uint8_t pinMask)
: in(portAddresses[portHandle], pinMask), out(portAddresses[portHandle], pinMask),
  dir(portAddresses[portHandle], pinMask), ren(portAddresses[portHandle], pinMask),
  sel0(portAddresses[portHandle], pinMask), sel1(portAddresses[portHandle], pinMask),
  selc(portAddresses[portHandle], pinMask), ies(portAddresses[portHandle], pinMask),
  ie(portAddresses[portHandle], pinMask), ifg(portAddresses[portHandle], pinMask) {
    _pinMask = pinMask;
    _pinIdx = countBitsInByte(pinMask);
    _baseAddress = portAddresses[portHandle];
    _portIdx = portHandle;
}

void Pin::SetMode(Direction mode) {
    switch (mode) {
        case Direction::OUTPUT: {
            dir.set();
            break;
        }
        case Direction::INPUT: {
            dir.clear();
            ren.clear();
            break;
        }
    }
}

void Pin::SetFunction(Function func) {
    switch (func) {
        case Function::GPIO: {
            sel0.clear();
            sel1.clear();
            break;
        }
        case Function::PRIMARY: {
            sel0.set();
            sel1.clear();
            break;
        }
        case Function::SECONDARY: {
            sel0.clear();
            sel1.set();
            break;
        }
        case Function::TERTIARY: {
            sel0.set();
            sel1.set();
            break;
        }
    }
}

void Pin::SetResistor(Resistor res) {
    switch (res) {
        case Resistor::NONE: {
            ren.clear();
            break;
        }
        case Resistor::PULLUP: {
            ren.set();
            out.set();
            break;
        }
        case Resistor::PULLDOWN: {
            ren.set();
            out.clear();
            break;
        }
    }
}

void Pin::EnableInterrupt(InterruptSource src, CallbackFuncPtr func) {
    ie.set();
    if (src == InterruptSource::FALLING) {
        ies.set();
    } else {
        ies.clear();
    }
    AttachCallback(_portIdx, _portIdx, func);
}

void Pin::DisableInterrupt() {
    ie.clear();
    ies.clear();
    DetachCallback(_pinIdx, _portIdx);
}


