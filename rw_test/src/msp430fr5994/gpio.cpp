/*
 * gpio.cpp
 *
 *  Created on: Jan 19, 2022
 *      Author: sailedeer
 */

#include <cstdint>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include <msp430.h>
#ifdef __cplusplus
}
#endif

#include "gpio.h"

using namespace MSP430FR5994;

static constexpr uint8_t PIN_COUNT = 8;
static constexpr uint8_t PORT_COUNT = 9;

// anonymous namespace for translation-unit local typedefs
namespace {
    typedef volatile struct {
        // generates padding of n bytes; with id as an arbitrary unique
        // identifier for each instance of padding per namespace
        // intended to be used to align bytes in register structs
        #define __PADDING(id, n) char __PAD_##id[n]
        uint8_t in;        // (0x0000)
        __PADDING(0, 0x1);
        uint8_t out;       // (0x0002)
        __PADDING(1, 0x1);
        uint8_t dir;       // (0x0004)
        __PADDING(2, 0x1);
        uint8_t ren;       // (0x0006)
        __PADDING(3, 0x3);
        uint8_t sel0;      // (0x000A)
        __PADDING(4, 0x1);
        uint8_t sel1;      // (0x000C)
        __PADDING(5, 0x9);
        uint8_t selc;      // (0x0016)
        __PADDING(6, 0x1);
        uint8_t ies;       // (0x0018)
        __PADDING(7, 0x1);
        uint8_t ie;        // (0x001A)
        __PADDING(8, 0x1);
        uint8_t ifg;       // (0x001C)
    } GPIOBase;

    typedef struct {
        GPIO::callbackFunctionPtr tbl[PIN_COUNT];
    } CallbackFuncPtrTable;

};  // namespace

// array of pointers to GPIO
static volatile GPIOBase * bases[PORT_COUNT] = {
    (GPIOBase *)  P1_BASE,
    (GPIOBase *) (P2_BASE + 0x1),  // even, offset by 1 byte
    (GPIOBase *)  P3_BASE,
    (GPIOBase *) (P4_BASE + 0x1),  // even, offset by 1 byte
    (GPIOBase *)  P5_BASE,
    (GPIOBase *) (P6_BASE + 0x1),  // even, offset by 1 byte
    (GPIOBase *)  P7_BASE,
    (GPIOBase *) (P8_BASE + 0x1),  // even, offset by 1 byte
    (GPIOBase *)  PJ_BASE
};

static CallbackFuncPtrTable portCallbackTbl[PORT_COUNT];

static inline uint8_t getPinBitMask(GPIO::PinID id) {
    return (0x1 << id);
}

static inline GPIOBase *getBase(GPIO::Port port) {
    return bases[port];
}

GPIO::Direction GPIO::GetPinDirection(const GPIO::Pin &pin) {
    GPIOBase *base = getBase(pin.port);
    if (base->dir & getPinBitMask(pin.id)) {
        return GPIO::OUTPUT;
    } else {
        return GPIO::INPUT;
    }
}

void GPIO::SetPinDirection(const GPIO::Pin &pin, GPIO::Direction dir) {
    GPIOBase *base = getBase(pin.port);
    if (dir == GPIO::OUTPUT) {
        base->dir |= getPinBitMask(pin.id);
    } else {
        base->sel0 &= ~(getPinBitMask(pin.id));
        base->sel1 &= ~(getPinBitMask(pin.id));
        base->dir &= ~(getPinBitMask(pin.id));
        base->ren &= ~(getPinBitMask(pin.id));
    }
}

void GPIO::SetInterruptEventSource(const GPIO::Pin &pin, GPIO::InterruptSource src) {
    GPIOBase *base = getBase(pin.port);
    if (src == GPIO::LOW_TO_HIGH_EDGE) {
        base->ies &= ~(getPinBitMask(pin.id));
    } else {
        base->ies |= getPinBitMask(pin.id);
    }
}

GPIO::InterruptSource GPIO::GetInterruptEventSource(const GPIO::Pin &pin) {
    GPIOBase *base = getBase(pin.port);
    if (base->ies & getPinBitMask(pin.id)) {
        return GPIO::HIGH_TO_LOW_EDGE;
    } else {
        return GPIO::LOW_TO_HIGH_EDGE;
    }
}

void GPIO::EnableInterrupt(const GPIO::Pin &pin) {
    GPIOBase *base = getBase(pin.port);
    base->ie |= getPinBitMask(pin.id);
}

void GPIO::DisableInterrupt(const GPIO::Pin &pin) {
    GPIOBase *base = getBase(pin.port);
    base->ie &= ~(getPinBitMask(pin.id));
}

void GPIO::SetPinValue(const GPIO::Pin &pin, GPIO::Value val) {
    GPIOBase *base = getBase(pin.port);
    if (val == GPIO::LOW) {
        base->out &= ~(getPinBitMask(pin.id));
    } else {
        base->out |= getPinBitMask(pin.id);
    }
}

GPIO::Value GPIO::GetPinValue(const GPIO::Pin &pin) {
    GPIOBase *base = getBase(pin.port);
    if (base->out & getPinBitMask(pin.id)) {
        return GPIO::HIGH;
    } else {
        return GPIO::LOW;
    }
}

void GPIO::TogglePinValue(const GPIO::Pin &pin) {
    GPIOBase *base = getBase(pin.port);
    base->out ^= getPinBitMask(pin.id);
}

void GPIO::SetPinFunctionMode(const GPIO::Pin &pin, GPIO::Function mode) {
    GPIOBase *base = getBase(pin.port);
    switch (mode) {
        case GPIO::NONE:
            base->sel0 &= ~(getPinBitMask(pin.id));
            base->sel1 &= ~(getPinBitMask(pin.id));
            break;
        case GPIO::PRIMARY:
            base->sel0 |= getPinBitMask(pin.id);
            base->sel1 &= ~(getPinBitMask(pin.id));
            break;
        case GPIO::SECONDARY:
            base->sel0 &= ~(getPinBitMask(pin.id));
            base->sel1 |= getPinBitMask(pin.id);
            break;
        case GPIO::TERTIARY:
            base->sel0 |= getPinBitMask(pin.id);
            base->sel1 |= getPinBitMask(pin.id);
            break;
    }
}

GPIO::Function GPIO::GetPinFunctionMode(const GPIO::Pin &pin) {
    GPIOBase *base = getBase(pin.port);
    if (!(base->sel0 & getPinBitMask(pin.id)) && !(base->sel1 & getPinBitMask(pin.id))) {
        return GPIO::NONE;
    } else if (base->sel0 & getPinBitMask(pin.id) && ~(base->sel1 & getPinBitMask(pin.id))) {
        return GPIO::PRIMARY;
    } else if (!(base->sel0 & getPinBitMask(pin.id)) && base->sel1 & getPinBitMask(pin.id)) {
        return GPIO::SECONDARY;
    } else {
        return GPIO::TERTIARY;
    }
}

void GPIO::AttachInterruptToPin(const GPIO::Pin &pin, GPIO::callbackFunctionPtr func) {
    if (func != nullptr) {
        (portCallbackTbl[pin.port]).tbl[pin.id] = func;
    }
}

void GPIO::DetachInterruptFromPin(const GPIO::Pin &pin) {
    (portCallbackTbl[pin.port]).tbl[pin.id] = nullptr;
}
