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
            PORT1,
            PORT2,
            PORT3,
            PORT4,
            PORT5,
            PORT6,
            PORT7,
            PORT8,
            PORTJ
        };

        class Pin {
            public:
                enum Direction {
                    INPUT = 0x00,
                    OUTPUT
                };

                enum Value {
                    LOW = 0x00,
                    HIGH,

                };

                enum InterruptSource {
                    LOW_TO_HIGH_EDGE,
                    HIGH_TO_LOW_EDGE,
                    INTERRUPT_DISABLED
                };

                enum Function {
                    NONE,
                    PRIMARY,
                    SECONDARY,
                    TERTIARY
                };

                typedef void (*CallbackFuncPtr)();

                Pin();

                // disable copy constructor
                Pin(const Pin&) = delete;

                /*
                 * Marks underlying pin handle as free if it is in use
                 */
                ~Pin();

                bool Attach(Port, uint8_t);

                void Detach();

                Direction GetDirection();

                void SetDirection(Direction);

                void SetInterruptEventSource(InterruptSource);

                InterruptSource GetInterruptEventSource();

                void EnableInterrupt();

                void DisableInterrupt();

                void Write(Value);

                Value Read();

                void ToggleOutput();

                void SetFunctionMode(Function);

                Function GetFunctionMode() const;

                void AttachCallback(CallbackFuncPtr);

                void DetachCallback();

            private:
                uint8_t _bitMask;
                uint8_t _bit;
                uint8_t _portIdx;
                uint8_t _attached;



        };  // class Pin
    }   // namespace GPIO
}   // namespace MSP430

#endif /* MSP430FR5994_GPIO_H_ */
