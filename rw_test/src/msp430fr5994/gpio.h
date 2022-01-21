/*
 * gpio.h
 *
 *  Created on: Jan 19, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_GPIO_H_
#define MSP430FR5994_GPIO_H_

#include "types.h"

namespace MSP430FR5994 {

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

        enum PinID {
            PIN0 = 0x1,
            PIN1,
            PIN2,
            PIN3,
            PIN4,
            PIN5,
            PIN6,
            PIN7,
        };

        enum Direction {
            INPUT,
            OUTPUT
        };

        enum Value {
            LOW,
            HIGH
        };

        enum InterruptSource {
            LOW_TO_HIGH_EDGE,
            HIGH_TO_LOW_EDGE
        };

        enum Function {
            NONE,
            PRIMARY,
            SECONDARY,
            TERTIARY
        };

        struct Pin {
            Pin(PinID pinId, Port base) {
                id = pinId;
                port = base;
            }
            Pin(const Pin& rhs) {
                id = rhs.id;
                port = rhs.port;
            }
            PinID id;
            Port port;
        };

        typedef void (*callbackFunctionPtr)();

        Direction GetPinDirection(Pin);

        void SetPinDirection(Pin, Direction);

        void SetInterruptEventSource(Pin, InterruptSource);

        InterruptSource GetInterruptEventSource(Pin);

        void EnableInterrupt(Pin);

        void DisableInterrupt(Pin);

        void SetPinValue(Pin, Value);

        Value GetPinValue(Pin);

        void TogglePinValue(Pin);

        void SetPinFunctionMode(Pin, Function);

        Function GetPinFunctionMode(Pin);

        void AttachInterruptToPin(const Pin&, callbackFunctionPtr);

        void DetachInterruptFromPin(const Pin&);

    }   // namespace GPIO

}   // namespace MSP430

#endif /* MSP430FR5994_GPIO_H_ */
