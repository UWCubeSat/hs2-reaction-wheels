/*
 * gpio.cpp
 *
 *  Created on: Jan 19, 2022
 *      Author: sailedeer
 */

#include <cstdint>
#include <cstring>
#include <msp430.h>

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

static inline void safeCall(Pin::CallbackFuncPtr func) {
    if (func) {
        func();
    }
}

static inline uint8_t pinInUse(uint8_t portIndex, uint8_t pinMask) {
    return (pinMask & (pinInUseTbl[portIndex]));
}

static inline void usePin(uint8_t portIndex, uint8_t pinMask) {
    pinInUseTbl[portIndex] |= pinMask;
}

static inline void releasePin(uint8_t portIndex, uint8_t pinMask) {
    pinInUseTbl[portIndex] &= ~(pinMask);
}

static inline gpio_port_ptr_t getBasePtr(uint8_t portIdx) {
    return bases[portIdx];
}

Pin::Pin(Port port, uint8_t pin) {
    _bit = pin;
    _bitMask = (0x1 << _bit);
    _portIdx = (uint8_t) port;
    if (pinInUse(_portIdx, _bitMask)) {
        _status = Pin::IN_USE;
    } else {
        usePin(_portIdx, _bitMask);
        _status = Pin::ATTACHED;
    }
}

inline Pin::~Pin() {
    // detach pins
    if (pinInUse(_portIdx, _bitMask) && (_status == Pin::ATTACHED)) {
        releasePin(_portIdx, _bitMask);
    }
    // force callback to invalid addr
    this->DetachCallback();
}

inline Pin::Direction Pin::GetDirection() const {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (base->dir & this->_bitMask) {
        return Pin::OUTPUT;
    } else {
        return Pin::INPUT;
    }
}

inline void Pin::SetDirection(Pin::Direction dir) {
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

inline void Pin::SetInterruptEventSource(Pin::InterruptSource src) {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (src == Pin::LOW_TO_HIGH_EDGE) {
        base->ies &= ~(this->_bitMask);
    } else {
        base->ies |= this->_bitMask;
    }
}

inline Pin::InterruptSource Pin::GetInterruptEventSource() const {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (base->ies & this->_bitMask) {
        return Pin::HIGH_TO_LOW_EDGE;
    } else {
        return Pin::LOW_TO_HIGH_EDGE;
    }
}

inline void Pin::EnableInterrupt() {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    base->ie |= this->_bitMask;
}

inline void Pin::DisableInterrupt() {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    base->ie &= ~(this->_bitMask);
}

inline void Pin::Write(Pin::Value val) {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (this->GetDirection() == Pin::OUTPUT) {
        if (val == Pin::LOW) {
            base->out &= ~(this->_bitMask);
        } else {
            base->out |= this->_bitMask;
        }
    }
}

inline Pin::Value Pin::Read() const {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (this->GetDirection() == Pin::INPUT) {
        if (base->in & this->_bitMask) {
            return Pin::HIGH;
        } else {
            return Pin::LOW;
        }
    } else {
        if (base->out & this->_bitMask) {
            return Pin::HIGH;
        } else {
            return Pin::LOW;
        }
    }
}

inline void Pin::ToggleOutput() {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (this->GetDirection() == Pin::OUTPUT) {
        base->out ^= this->_bitMask;
    }
}

inline void Pin::SetResistor(Pin::Resistor res) {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (this->GetDirection() == Pin::INPUT) {
        switch (res) {
            case Pin::NO_RESISTOR:
                base->ren &= ~(this->_bitMask);
                break;
            case Pin::PULL_UP:
                base->out |= this->_bitMask;
            case Pin::PULL_DOWN:
                base->ren |= this->_bitMask;
                break;
        }
    }
}

inline Pin::Resistor Pin::GetResistor() {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    uint8_t res = base->ren & this->_bitMask;
    uint8_t out = base->out & this->_bitMask;

    if ((this->GetDirection() == Pin::INPUT) && res) {
        if (out) {
            return Pin::PULL_UP;
        } else {
            return Pin::PULL_DOWN;
        }
    } else {
        return Pin::NO_RESISTOR;
    }
}

inline void Pin::SetFunctionMode(Pin::Function mode) {
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

inline Pin::Function Pin::GetFunctionMode() const {
    gpio_port_ptr_t base = getBasePtr(this->_portIdx);
    if (!(base->sel0 & this->_bitMask) && !(base->sel1 & this->_bitMask)) {
        return Pin::NONE;
    } else if (base->sel0 & this->_bitMask && !(base->sel1 & this->_bitMask)) {
        return Pin::PRIMARY;
    } else if (!(base->sel0 & this->_bitMask) && base->sel1 & this->_bitMask) {
        return Pin::SECONDARY;
    } else {
        return Pin::TERTIARY;
    }
}

inline void Pin::AttachCallback(Pin::CallbackFuncPtr func) {
    if (func) {
        this->EnableInterrupt();
        (portCallbackTbl[this->_portIdx]).tbl[this->_bit] = func;
    }
}

inline void Pin::DetachCallback() {
    (portCallbackTbl[this->_portIdx]).tbl[this->_bit] = INVALID_CB_ADDR;
}

#pragma vector=PORT1_VECTOR
__interrupt void port1_isr() {
    switch(__even_in_range(P1IV, P1IV__P1IFG7))
    {
        case P1IV__NONE:    break;          // Vector  0:  No interrupt
        case P1IV__P1IFG0:  safeCall(portCallbackTbl[0].tbl[0]); break;          // Vector  2:  P1.0 interrupt flag
        case P1IV__P1IFG1:  safeCall(portCallbackTbl[0].tbl[1]); break;          // Vector  4:  P1.1 interrupt flag
        case P1IV__P1IFG2:  safeCall(portCallbackTbl[0].tbl[2]); break;          // Vector  6:  P1.2 interrupt flag
        case P1IV__P1IFG3:  safeCall(portCallbackTbl[0].tbl[3]); break;          // Vector  8:  P1.3 interrupt flag
        case P1IV__P1IFG4:  safeCall(portCallbackTbl[0].tbl[4]); break;          // Vector  10:  P1.4 interrupt flag
        case P1IV__P1IFG5:  safeCall(portCallbackTbl[0].tbl[5]); break;          // Vector  12:  P1.5 interrupt flag
        case P1IV__P1IFG6:  safeCall(portCallbackTbl[0].tbl[6]); break;          // Vector  14:  P1.6 interrupt flag
        case P1IV__P1IFG7:  safeCall(portCallbackTbl[0].tbl[7]); break;          // Vector  16:  P1.7 interrupt flag
        default: break;
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void port2_isr() {
    switch(__even_in_range(P2IV, P2IV__P2IFG7))
    {
        case P2IV__NONE:    break;          // Vector  0:  No interrupt
        case P2IV__P2IFG0:  safeCall(portCallbackTbl[1].tbl[0]); break;          // Vector  2:  P1.0 interrupt flag
        case P2IV__P2IFG1:  safeCall(portCallbackTbl[1].tbl[1]); break;          // Vector  4:  P1.1 interrupt flag
        case P2IV__P2IFG2:  safeCall(portCallbackTbl[1].tbl[2]); break;          // Vector  6:  P1.2 interrupt flag
        case P2IV__P2IFG3:  safeCall(portCallbackTbl[1].tbl[3]); break;          // Vector  8:  P1.3 interrupt flag
        case P2IV__P2IFG4:  safeCall(portCallbackTbl[1].tbl[4]); break;          // Vector  10:  P1.4 interrupt flag
        case P2IV__P2IFG5:  safeCall(portCallbackTbl[1].tbl[5]); break;          // Vector  12:  P1.5 interrupt flag
        case P2IV__P2IFG6:  safeCall(portCallbackTbl[1].tbl[6]); break;          // Vector  14:  P1.6 interrupt flag
        case P2IV__P2IFG7:  safeCall(portCallbackTbl[1].tbl[7]); break;          // Vector  16:  P1.7 interrupt flag
        default: break;
    }
}

#pragma vector=PORT3_VECTOR
__interrupt void port3_isr() {
    switch(__even_in_range(P3IV, P3IV__P3IFG7))
    {
        case P3IV__NONE:    break;          // Vector  0:  No interrupt
        case P3IV__P3IFG0:  safeCall(portCallbackTbl[2].tbl[0]); break;          // Vector  2:  P1.0 interrupt flag
        case P3IV__P3IFG1:  safeCall(portCallbackTbl[2].tbl[1]); break;          // Vector  4:  P1.1 interrupt flag
        case P3IV__P3IFG2:  safeCall(portCallbackTbl[2].tbl[2]); break;          // Vector  6:  P1.2 interrupt flag
        case P3IV__P3IFG3:  safeCall(portCallbackTbl[2].tbl[3]); break;          // Vector  8:  P1.3 interrupt flag
        case P3IV__P3IFG4:  safeCall(portCallbackTbl[2].tbl[4]); break;          // Vector  10:  P1.4 interrupt flag
        case P3IV__P3IFG5:  safeCall(portCallbackTbl[2].tbl[5]); break;          // Vector  12:  P1.5 interrupt flag
        case P3IV__P3IFG6:  safeCall(portCallbackTbl[2].tbl[6]); break;          // Vector  14:  P1.6 interrupt flag
        case P3IV__P3IFG7:  safeCall(portCallbackTbl[2].tbl[7]); break;          // Vector  16:  P1.7 interrupt flag
        default: break;
    }
}

#pragma vector=PORT4_VECTOR
__interrupt void port4_isr() {
    switch(__even_in_range(P4IV, P4IV__P4IFG7))
    {
        case P4IV__NONE:    break;          // Vector  0:  No interrupt
        case P4IV__P4IFG0:  safeCall(portCallbackTbl[3].tbl[0]); break;          // Vector  2:  P1.0 interrupt flag
        case P4IV__P4IFG1:  safeCall(portCallbackTbl[3].tbl[1]); break;          // Vector  4:  P1.1 interrupt flag
        case P4IV__P4IFG2:  safeCall(portCallbackTbl[3].tbl[2]); break;          // Vector  6:  P1.2 interrupt flag
        case P4IV__P4IFG3:  safeCall(portCallbackTbl[3].tbl[3]); break;          // Vector  8:  P1.3 interrupt flag
        case P4IV__P4IFG4:  safeCall(portCallbackTbl[3].tbl[4]); break;          // Vector  10:  P1.4 interrupt flag
        case P4IV__P4IFG5:  safeCall(portCallbackTbl[3].tbl[5]); break;          // Vector  12:  P1.5 interrupt flag
        case P4IV__P4IFG6:  safeCall(portCallbackTbl[3].tbl[6]); break;          // Vector  14:  P1.6 interrupt flag
        case P4IV__P4IFG7:  safeCall(portCallbackTbl[3].tbl[7]); break;          // Vector  16:  P1.7 interrupt flag
        default: break;
    }
}

#pragma vector=PORT5_VECTOR
__interrupt void port5_isr() {
    switch(__even_in_range(P5IV, P5IV__P5IFG7))
    {
        case P5IV__NONE:    break;          // Vector  0:  No interrupt
        case P5IV__P5IFG0:  safeCall(portCallbackTbl[4].tbl[0]); break;          // Vector  2:  P1.0 interrupt flag
        case P5IV__P5IFG1:  safeCall(portCallbackTbl[4].tbl[1]); break;          // Vector  4:  P1.1 interrupt flag
        case P5IV__P5IFG2:  safeCall(portCallbackTbl[4].tbl[2]); break;          // Vector  6:  P1.2 interrupt flag
        case P5IV__P5IFG3:  safeCall(portCallbackTbl[4].tbl[3]); break;          // Vector  8:  P1.3 interrupt flag
        case P5IV__P5IFG4:  safeCall(portCallbackTbl[4].tbl[4]); break;          // Vector  10:  P1.4 interrupt flag
        case P5IV__P5IFG5:  safeCall(portCallbackTbl[4].tbl[5]); break;          // Vector  12:  P1.5 interrupt flag
        case P5IV__P5IFG6:  safeCall(portCallbackTbl[4].tbl[6]); break;          // Vector  14:  P1.6 interrupt flag
        case P5IV__P5IFG7:  safeCall(portCallbackTbl[4].tbl[7]); break;          // Vector  16:  P1.7 interrupt flag
        default: break;
    }
}

#pragma vector=PORT6_VECTOR
__interrupt void port6_isr() {
    switch(__even_in_range(P6IV, P6IV__P6IFG7))
    {
        case P6IV__NONE:    break;          // Vector  0:  No interrupt
        case P6IV__P6IFG0:  safeCall(portCallbackTbl[5].tbl[0]); break;          // Vector  2:  P1.0 interrupt flag
        case P6IV__P6IFG1:  safeCall(portCallbackTbl[5].tbl[1]); break;          // Vector  4:  P1.1 interrupt flag
        case P6IV__P6IFG2:  safeCall(portCallbackTbl[5].tbl[2]); break;          // Vector  6:  P1.2 interrupt flag
        case P6IV__P6IFG3:  safeCall(portCallbackTbl[5].tbl[3]); break;          // Vector  8:  P1.3 interrupt flag
        case P6IV__P6IFG4:  safeCall(portCallbackTbl[5].tbl[4]); break;          // Vector  10:  P1.4 interrupt flag
        case P6IV__P6IFG5:  safeCall(portCallbackTbl[5].tbl[5]); break;          // Vector  12:  P1.5 interrupt flag
        case P6IV__P6IFG6:  safeCall(portCallbackTbl[5].tbl[6]); break;          // Vector  14:  P1.6 interrupt flag
        case P6IV__P6IFG7:  safeCall(portCallbackTbl[5].tbl[7]); break;          // Vector  16:  P1.7 interrupt flag
        default: break;
    }
}

#pragma vector=PORT7_VECTOR
__interrupt void port7_isr() {
    switch(__even_in_range(P7IV, P7IV__P7IFG7))
    {
        case P7IV__NONE:    break;          // Vector  0:  No interrupt
        case P7IV__P7IFG0:  safeCall(portCallbackTbl[6].tbl[0]); break;          // Vector  2:  P1.0 interrupt flag
        case P7IV__P7IFG1:  safeCall(portCallbackTbl[6].tbl[1]); break;          // Vector  4:  P1.1 interrupt flag
        case P7IV__P7IFG2:  safeCall(portCallbackTbl[6].tbl[2]); break;          // Vector  6:  P1.2 interrupt flag
        case P7IV__P7IFG3:  safeCall(portCallbackTbl[6].tbl[3]); break;          // Vector  8:  P1.3 interrupt flag
        case P7IV__P7IFG4:  safeCall(portCallbackTbl[6].tbl[4]); break;          // Vector  10:  P1.4 interrupt flag
        case P7IV__P7IFG5:  safeCall(portCallbackTbl[6].tbl[5]); break;          // Vector  12:  P1.5 interrupt flag
        case P7IV__P7IFG6:  safeCall(portCallbackTbl[6].tbl[6]); break;          // Vector  14:  P1.6 interrupt flag
        case P7IV__P7IFG7:  safeCall(portCallbackTbl[6].tbl[7]); break;          // Vector  16:  P1.7 interrupt flag
        default: break;
    }
}

#pragma vector=PORT8_VECTOR
__interrupt void port8_isr() {
    switch(__even_in_range(P8IV, P8IV__P8IFG7))
    {
        case P8IV__NONE:    break;          // Vector  0:  No interrupt
        case P8IV__P8IFG0:  safeCall(portCallbackTbl[7].tbl[0]); break;          // Vector  2:  P1.0 interrupt flag
        case P8IV__P8IFG1:  safeCall(portCallbackTbl[7].tbl[1]); break;          // Vector  4:  P1.1 interrupt flag
        case P8IV__P8IFG2:  safeCall(portCallbackTbl[7].tbl[2]); break;          // Vector  6:  P1.2 interrupt flag
        case P8IV__P8IFG3:  safeCall(portCallbackTbl[7].tbl[3]); break;          // Vector  8:  P1.3 interrupt flag
        case P8IV__P8IFG4:  safeCall(portCallbackTbl[7].tbl[4]); break;          // Vector  10:  P1.4 interrupt flag
        case P8IV__P8IFG5:  safeCall(portCallbackTbl[7].tbl[5]); break;          // Vector  12:  P1.5 interrupt flag
        case P8IV__P8IFG6:  safeCall(portCallbackTbl[7].tbl[6]); break;          // Vector  14:  P1.6 interrupt flag
        case P8IV__P8IFG7:  safeCall(portCallbackTbl[7].tbl[7]); break;          // Vector  16:  P1.7 interrupt flag
        default: break;
    }
}

