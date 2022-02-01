/*
 * gpio.cpp
 *
 *  Created on: Jan 19, 2022
 *      Author: sailedeer
 */

#include <cstdint>

#include "gpio.h"

using namespace MSP430FR5994;

#if !defined(__GPIO_USER_ISR__)

static const uint8_t PIN_COUNT = 8;
static const uint8_t PORT_COUNT = 9;

struct PinCallbackTable {
   GPIO::CallbackFuncPtr tbl[PIN_COUNT];
};

static PinCallbackTable portCallbackTbl[PORT_COUNT] = {0};

static inline void safeCall(GPIO::CallbackFuncPtr func) {
    if (func) {
        func();
    }
}

void GPIO::AttachCallback(uint8_t pinIdx, uint8_t portIdx, GPIO::CallbackFuncPtr func) {
    portCallbackTbl[portIdx].tbl[pinIdx] = func;
}

void GPIO::DetachCallback(uint8_t pinIdx, uint8_t portIdx) {
    portCallbackTbl[portIdx].tbl[pinIdx] = nullptr;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void) {
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) port1_isr() {
#else
#error Compiler not supported!
#endif
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
    __bic_SR_register_on_exit(LPM0_bits);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT2_VECTOR
__interrupt void port2_isr(void) {
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) port2_isr() {
#else
#error Compiler not supported!
#endif
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
    __bic_SR_register_on_exit(LPM0_bits);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT3_VECTOR
__interrupt void port3_isr(void) {
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT3_VECTOR))) port3_isr() {
#else
#error Compiler not supported!
#endif
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
    __bic_SR_register_on_exit(LPM0_bits);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT4_VECTOR
__interrupt void port4_isr(void) {
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT4_VECTOR))) port4_isr() {
#else
#error Compiler not supported!
#endif
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
    __bic_SR_register_on_exit(LPM0_bits);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT5_VECTOR
__interrupt void port5_isr(void) {
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT5_VECTOR))) port5_isr() {
#else
#error Compiler not supported!
#endif
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
    __bic_SR_register_on_exit(LPM0_bits);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT6_VECTOR
__interrupt void port6_isr(void) {
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT6_VECTOR))) port6_isr() {
#else
#error Compiler not supported!
#endif
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
    __bic_SR_register_on_exit(LPM0_bits);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT7_VECTOR
__interrupt void port7_isr(void) {
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT7_VECTOR))) port7_isr() {
#else
#error Compiler not supported!
#endif
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
    __bic_SR_register_on_exit(LPM0_bits);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT8_VECTOR
__interrupt void port8_isr(void) {
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT8_VECTOR))) port8_isr() {
#else
#error Compiler not supported!
#endif
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
    __bic_SR_register_on_exit(LPM0_bits);
}
#else
inline void AttachCallback(uint8_t pin, uint8_t port, CallbackFuncPtr func) {
    __no_operation();
}
inline void DetachCallback(uint8_t pin, uint8_t port) {
    __no_operation();
}
#endif  // __GPIO_USE_USER_DEFINED_ISRS__
