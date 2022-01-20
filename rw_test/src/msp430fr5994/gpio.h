/*
 * gpio.h
 *
 *  Created on: Jan 19, 2022
 *      Author: sailedeer
 */

#ifndef MSP430_GPIO_H_
#define MSP430_GPIO_H_

#include "types.h"

namespace MSP430 {

    namespace GPIO {

        enum Port {
            P1,
            P2,
            P3,
            P4,
            P5,
            P6,
            P7,
            P8,
            PJ
        };

        enum Direction {
            INPUT,
            OUTPUT
        };

        enum Value {
            FLOATING,   // if pull-up/down is disabled,
            LOW,
            HIGH
        };

        enum InterruptSource {
            LOW_TO_HIGH_EDGE,
            HIGH_TO_LOW_EDGE
        };

        extern struct Pin;

        Direction GetPinDirection(Pin);

        void SetPinDirection(Pin, Direction);

        void SetInterruptEventSource(Pin, InterruptSource);

        InterruptSource GetInterruptEventSource(Pin);

        void EnableInterrupt(Pin);

        void DisableInterrupt(Pin);

        void SetPinValue(Pin, Value);

        Value GetPinValue(Pin);

        void TogglePinValue(Pin);

    }   // namespace GPIO

}   // namespace MSP430

#endif /* MSP430_GPIO_H_ */
