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

using namespace MSP430FR5994::GPIO;

constexpr uint8_t PIN_COUNT = 8;
constexpr uint8_t PORT_COUNT = 9;

typedef struct {
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
} gpio_port_s, *gpio_port_ptr_t;

typedef struct {
   Pin::CallbackFuncPtr tbl[PIN_COUNT];
} cb_func_ptr_tbl_t;


// array of pointers to GPIO
static volatile gpio_port_ptr_t bases[PORT_COUNT] = {
    (gpio_port_ptr_t)  P1_BASE,
    (gpio_port_ptr_t) (P2_BASE + 0x1),  // even, offset by 1 byte
    (gpio_port_ptr_t)  P3_BASE,
    (gpio_port_ptr_t) (P4_BASE + 0x1),  // even, offset by 1 byte
    (gpio_port_ptr_t)  P5_BASE,
    (gpio_port_ptr_t) (P6_BASE + 0x1),  // even, offset by 1 byte
    (gpio_port_ptr_t)  P7_BASE,
    (gpio_port_ptr_t) (P8_BASE + 0x1),  // even, offset by 1 byte
    (gpio_port_ptr_t)  PJ_BASE
};

static uint8_t pinInUseTbl[PIN_COUNT] = {0};

static cb_func_ptr_tbl_t portCallbackTbl[PORT_COUNT] = {0};

static inline gpio_port_ptr_t getBasePtr(uint8_t portIdx) {
    return bases[portIdx];
}

Pin::Direction Pin::GetDirection() {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (base->dir & this->_bitMask) {
        return Pin::OUTPUT;
    } else {
        return Pin::INPUT;
    }
}

void Pin::SetDirection(Pin::Direction dir) {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (dir == Pin::OUTPUT) {
        base->dir |= this->_bitMask;
    } else {
        base->sel0 &= ~(this->_bitMask);
        base->sel1 &= ~(this->_bitMask);
        base->dir &= ~(this->_bitMask);
        base->ren &= ~(this->_bitMask);
    }
}

void Pin::SetInterruptEventSource(Pin::InterruptSource src) {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (src == Pin::LOW_TO_HIGH_EDGE) {
        base->ies &= ~(this->_bitMask);
    } else {
        base->ies |= this->_bitMask;
    }
}

Pin::InterruptSource Pin::GetInterruptEventSource() {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (base->ies & this->_bitMask) {
        return Pin::HIGH_TO_LOW_EDGE;
    } else {
        return Pin::LOW_TO_HIGH_EDGE;
    }
}

void Pin::EnableInterrupt() {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    base->ie |= this->_bitMask;
}

void Pin::DisableInterrupt() {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    base->ie &= ~(this->_bitMask);
}

void Pin::Write(Pin::Value val) {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (val == Pin::LOW) {
        base->out &= ~(this->_bitMask);
    } else {
        base->out |= this->_bitMask;
    }
}

Pin::Value Pin::Read() {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (this->GetDirection() == Pin::INPUT && (base->in & this->_bitMask)) {
        return Pin::HIGH;
    } else {
        return Pin::LOW;
    }
}

void Pin::ToggleOutput() {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (this->GetDirection() == Pin::OUTPUT)
    base->out ^= this->_bitMask;
}

void Pin::SetFunctionMode(Pin::Function mode) {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    switch (mode) {
        case Pin::NONE:
            base->sel0 &= ~(this->_bitMask);
            base->sel1 &= ~(this->_bitMask);
            break;
        case Pin::PRIMARY:
            base->sel0 |= this->_bitMask;
            base->sel1 &= ~(this->_bitMask);
            break;
        case Pin::SECONDARY:
            base->sel0 &= ~(this->_bitMask);
            base->sel1 |= this->_bitMask;
            break;
        case Pin::TERTIARY:
            base->sel0 |= this->_bitMask;
            base->sel1 |= this->_bitMask;
            break;
    }
}

Pin::Function Pin::GetFunctionMode() const {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (!(base->sel0 & this->_bitMask) && !(base->sel1 & this->_bitMask)) {
        return Pin::NONE;
    } else if (base->sel0 & this->_bitMask && ~(base->sel1 & this->_bitMask)) {
        return Pin::PRIMARY;
    } else if (!(base->sel0 & this->_bitMask) && base->sel1 & this->_bitMask) {
        return Pin::SECONDARY;
    } else {
        return Pin::TERTIARY;
    }
}

void Pin::AttachCallback(Pin::CallbackFuncPtr func) {
    if (func) {
        (portCallbackTbl[this->_portIdx]).tbl[this->_bit] = func;
    }
}

void Pin::DetachCallback() {
    (portCallbackTbl[this->_portIdx]).tbl[this->_bit] = nullptr;
}
