/*
 * gpio.h
 *
 *  Created on: Jan 19, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_GPIO_H_
#define MSP430FR5994_GPIO_H_

namespace MSP430FR5994 {

    namespace GPIO {

        template <typename size, base_addr, off>
        struct io_reg_st {
            typedef size * instance_ptr_t;
            static inline instance_ptr_t instance() {
                return
            }
        };

        template <typename addr>
        struct gpio_port_st {

        };



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

                enum Resistor {
                    PULL_DOWN,
                    PULL_UP,
                    NO_RESISTOR
                };

                enum Status {
                    UNSUPPORTED,
                    ATTACHED,
                    IN_USE
                };

                typedef void (*CallbackFuncPtr)();

                static const uint32_t INVALID_CB_ADDR = 0;

                Pin(Port, uint8_t);

                // disable copy constructor
                Pin(const Pin&) = delete;

                /*
                 * Marks underlying pin handle as free if it is in use
                 */
                ~Pin();

                inline Direction GetDirection() const;

                inline void SetDirection(Direction);

                inline void SetInterruptEventSource(InterruptSource);

                inline InterruptSource GetInterruptEventSource() const;

                inline void EnableInterrupt();

                inline void DisableInterrupt();

                inline void Write(Value);

                inline Value Read() const;

                inline void ToggleOutput();

                inline void SetResistor(Resistor);

                inline Resistor GetResistor();

                inline void SetFunctionMode(Function);

                inline Function GetFunctionMode() const;

                inline void AttachCallback(CallbackFuncPtr);

                inline void DetachCallback();

                inline Status GetStatus() const { return (Status)_status; }

            private:
                uint8_t _bitMask;
                uint8_t _bit;
                uint8_t _portIdx;
                uint8_t _status;

        };  // class Pin
    }   // namespace GPIO
}   // namespace MSP430

#endif /* MSP430FR5994_GPIO_H_ */
