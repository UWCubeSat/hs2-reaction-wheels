/*
 * pin.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: sailedeer
 */

#include "pin.h"

using namespace MSP430FR5994::GPIO;

static inline uint8_t countBitsInByte(uint8_t s) {
    uint8_t count = 0;
    while (s) {
        count += (s & 0x1);
        s >>= 1;
    }
    return count;
}

Pin::Pin(uint16_t baseAddress, uint8_t pinMask)
: in(baseAddress + OFS_P1IN, pinMask), out(baseAddress + OFS_P1OUT, pinMask),
  dir(baseAddress + OFS_P1DIR, pinMask), ren(baseAddress + OFS_P1REN, pinMask),
  sel0(baseAddress + OFS_P1SEL0, pinMask), sel1(baseAddress + OFS_P1SEL1, pinMask),
  selc(baseAddress + OFS_P1SELC, pinMask), ies(baseAddress + OFS_P1IES, pinMask),
  ie(baseAddress + OFS_P1IE, pinMask), ifg(baseAddress + OFS_P1IFG, pinMask) {
    _pinMask = pinMask;
    _pinIdx = countBitsInByte(pinMask);
    _baseAddress = baseAddress;
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
}

void Pin::DisableInterrupt() {
    ie.clear();
    ies.clear();
}


