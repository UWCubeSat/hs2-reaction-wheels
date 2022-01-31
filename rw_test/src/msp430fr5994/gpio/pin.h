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

                RegisterBit<uint8_t> in;
                RegisterBit<uint8_t> out;
                RegisterBit<uint8_t> dir;
                RegisterBit<uint8_t> ren;
                RegisterBit<uint8_t> sel0;
                RegisterBit<uint8_t> sel1;
                RegisterBit<uint8_t> selc;
                RegisterBit<uint8_t> ies;
                RegisterBit<uint8_t> ie;
                RegisterBit<uint8_t> ifg;
            private:
                uint16_t _baseAddress;
                uint8_t _pinMask;
                uint8_t _pinIdx;
        };  // class Pin
    }   // namespace GPIO
}   // namespace MSP430FR5994
#endif /* SRC_MSP430FR5994_GPIO_PIN_H_ */
