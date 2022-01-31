/*
 * port.h
 *
 *  Created on: Jan 30, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_GPIO_PORT_H_
#define MSP430FR5994_GPIO_PORT_H_

#include <cstdint>

#include "../msp430fr5994-hsl.h"
#include "../gpio.h"

namespace MSP430FR5994 {
    namespace GPIO {
        class Port {
            public:
                Port(uint16_t baseAddress);
                void SetMode(Direction dir, uint8_t pinMask);
                void SetFunction(Function func, uint8_t pinMask);
                void SetResistor(Resistor res, uint8_t pinMask);
                void EnableInterrupt(uint8_t pinMask, InterruptSource src);
                void DisableInterrupt(uint8_t pinMask);

                Register<uint8_t, 0x00> in;
                Register<uint8_t, 0x02> out;
                Register<uint8_t, 0x04> dir;
                Register<uint8_t, 0x06> ren;
                Register<uint8_t, 0x0A> sel0;
                Register<uint8_t, 0x0C> sel1;
                Register<uint8_t, 0x16> selc;
                Register<uint8_t, 0x18> ies;
                Register<uint8_t, 0x1A> ie;
                Register<uint8_t, 0x1C> ifg;
            private:
                uint16_t _baseAddress;
        };  // class Port
    }   // namespace GPIO
}   // namespace MSP430FR5994
#endif /* MSP430FR5994_GPIO_PORT_H_ */
