/*
 * port.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: sailedeer
 */

#include <cstdint>
#include <msp430.h>
#include "../gpio.h"
#include "port.h"

using namespace MSP430FR5994::GPIO;

Port::Port(uint16_t baseAddress)
: in(baseAddress + OFS_P1IN), out(baseAddress + OFS_P1OUT), dir(baseAddress + OFS_P1DIR),
  ren(baseAddress + OFS_P1REN), sel0(baseAddress + OFS_P1SEL0), sel1(baseAddress + OFS_P1SEL1),
  selc(baseAddress + OFS_P1SELC), ies(baseAddress + OFS_P1IES), ie(baseAddress + OFS_P1IE),
  ifg(baseAddress + OFS_P1IFG) { }

void Port::SetMode(Direction mode, uint8_t pinMask) {
    switch (mode) {
        case Direction::OUTPUT: {
            dir |= pinMask;
            break;
        }
        case Direction::INPUT: {
            dir &= ~(pinMask);
            ren &= ~(pinMask);
            break;
        }
    }
}

void Port::SetFunction(Function func, uint8_t pinMask) {
    switch (func) {
        case Function::GPIO: {
            sel0 &= ~(pinMask);
            sel1 &= ~(pinMask);
            break;
        }
        case Function::PRIMARY: {
            sel0 |= pinMask;
            sel1 &= ~(pinMask);
            break;
        }
        case Function::SECONDARY: {
            sel0 &= ~(pinMask);
            sel1 |= pinMask;
            break;
        }
        case Function::TERTIARY: {
            sel0 |= pinMask;
            sel1 |= pinMask;
            break;
        }
    }
}

void Port::SetResistor(Resistor res, uint8_t pinMask) {
    switch (res) {
        case Resistor::NONE: {
            ren &= ~(pinMask);
            break;
        }
        case Resistor::PULLUP: {
            ren = pinMask;
            out |= pinMask;
            break;
        }
        case Resistor::PULLDOWN: {
            ren |= pinMask;
            out &= ~(pinMask);
            break;
        }
    }
}

void Port::EnableInterrupt(uint8_t pinMask, InterruptSource src) {
    ie |= pinMask;
    if (src == InterruptSource::FALLING) {
        ies |= pinMask;
    } else {
        ies &= ~(pinMask);
    }
}

void Port::DisableInterrupt(uint8_t pinMask) {
    ie &= ~(pinMask);
}

