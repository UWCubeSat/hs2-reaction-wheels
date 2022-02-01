/*
 * pin.h
 *
 *  Created on: Jan 30, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_GPIO_PIN_H_
#define MSP430FR5994_GPIO_PIN_H_

#include <cstdint>

#include "../msp430fr5994-hsl.h"
#include "../gpio.h"

namespace MSP430FR5994 {
    namespace GPIO {
        class Pin {
            public:
                Pin(uint16_t baseAddress, uint8_t pinMask);
                void SetMode(Direction dir);
                void SetFunction(Function func);
                void SetResistor(Resistor res);
                void EnableInterrupt(InterruptSource src, CallbackFuncPtr func);
                void DisableInterrupt();

                RegisterBit<uint8_t, OFS_P1IN> in;
                RegisterBit<uint8_t, OFS_P1OUT> out;
                RegisterBit<uint8_t, OFS_P1DIR> dir;
                RegisterBit<uint8_t, OFS_P1REN> ren;
                RegisterBit<uint8_t, OFS_P1SEL0> sel0;
                RegisterBit<uint8_t, OFS_P1SEL1> sel1;
                RegisterBit<uint8_t, OFS_P1SELC> selc;
                RegisterBit<uint8_t, OFS_P1IES> ies;
                RegisterBit<uint8_t, OFS_P1IE> ie;
                RegisterBit<uint8_t, OFS_P1IFG> ifg;
            private:
                uint16_t _baseAddress;
                uint8_t _pinMask;
                uint8_t _pinIdx;
        };  // class Pin
    }   // namespace GPIO
}   // namespace MSP430FR5994
#endif /* SRC_MSP430FR5994_GPIO_PIN_H_ */
