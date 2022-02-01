/*
 * gpio.h
 *
 *  Created on: Jan 19, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_GPIO_H_
#define MSP430FR5994_GPIO_H_

#include <msp430fr5994.h>

#include "msp430fr5994-hsl.h"

namespace MSP430FR5994 {
    namespace GPIO {
        typedef void (*CallbackFuncPtr)();

        enum class Direction {
           OUTPUT,
           INPUT
        };

        enum class Resistor {
           NONE,
           PULLUP,
           PULLDOWN
        };

        enum class InterruptSource {
           RISING,
           FALLING
        };

        enum class Function {
           GPIO,
           PRIMARY,
           SECONDARY,
           TERTIARY
        };

        enum class PortHandle {
            P1 = 0x00,
            P2 = 0x01,
            P3 = 0x02,
            P4 = 0x03,
            P5 = 0x04,
            P6 = 0x05,
            P7 = 0x06,
            P8 = 0x07,
            PJ = 0x08
        };

        enum class PinHandle {
            PIN0 = BIT0,
            PIN1 = BIT1,
            PIN2 = BIT2,
            PIN3 = BIT3,
            PIN4 = BIT4,
            PIN5 = BIT5,
            PIN6 = BIT6,
            PIN7 = BIT7
        };

        void AttachCallback(uint8_t pinIdx, uint8_t portIdx, CallbackFuncPtr);
        void DetachCallback(uint8_t pinIdx, uint8_t portIdx);
    }   // namespace GPIO
}   // namespace MSP430FR5994
#endif /* MSP430FR5994_GPIO_H_ */
