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

        inline void AttachCallback(uint8_t, uint8_t, CallbackFuncPtr);
        inline void DetachCallback(uint8_t, uint8_t);
    }   // namespace GPIO
}   // namespace MSP430FR5994
#endif /* MSP430FR5994_GPIO_H_ */
