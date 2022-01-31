/*
 * gpio.h
 *
 *  Created on: Jan 19, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_GPIO_H_
#define MSP430FR5994_GPIO_H_

#include "msp430fr5994.h"
#include "msp430-hsl.h"

namespace MSP430FR5994 {

    namespace GPIO {

        enum class Direction {
            OUTPUT,
            INPUT
        };

        enum class Resistor {
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

        template <uint16_t addr>
        struct Port {
            Register<uint8_t, addr, 0x00> in;
            Register<uint8_t, addr, 0x02> out;
            Register<uint8_t, addr, 0x04> dir;
            Register<uint8_t, addr, 0x06> ren;
            Register<uint8_t, addr, 0x0A> sel0;
            Register<uint8_t, addr, 0x0C> sel1;
            Register<uint8_t, addr, 0x16> selc;
            Register<uint8_t, addr, 0x18> ies;
            Register<uint8_t, addr, 0x1A> ie;
            Register<uint8_t, addr, 0x1C> ifg;
            inline void SetMode(Direction dir, uint8_t pinMask);
            inline void SetFunction(Function func, uint8_t pinMask);
            inline void SetResistor(Resistor res, uint8_t pinMask);
            inline void EnableInterrupt(uint8_t pinMask, InterruptSource src);
            inline void DisableInterrupt(uint8_t pinMask);
        };

        template <uint16_t addr>
        inline void Port<addr>::SetMode(Direction mode, uint8_t pinMask) {
            switch (mode) {
                case Direction::OUTPUT: {
                    dir |= pinMask;
                    break;
                }
                case Direction::INPUT: {
                    dir &= ~(pinMask);
                    ren &= ~(pinMask);
                    break;
                }
            }
        }

        template <uint16_t addr>
        inline void Port<addr>::SetFunction(Function func, uint8_t pinMask) {
            switch (func) {
                case Function::GPIO: {
                    sel0 &= ~(pinMask);
                    sel1 &= ~(pinMask);
                    break;
                }
                case Function::PRIMARY: {
                    sel0 |= pinMask;
                    sel1 &= ~(pinMask);
                    break;
                }
                case Function::SECONDARY: {
                    sel0 &= ~(pinMask);
                    sel1 |= pinMask;
                    break;
                }
                case Function::TERTIARY: {
                    sel0 |= pinMask;
                    sel1 |= pinMask;
                    break;
                }
            }
        }

        template <uint16_t addr>
        inline void Port<addr>::SetResistor(Resistor res, uint8_t pinMask) {
            switch (res) {
                case Resistor::PULLUP: {
                    break;
                }
                case Resistor::PULLDOWN: {
                    break;
                }
            }
        }

        template <uint16_t addr>
        inline void Port<addr>::EnableInterrupt(uint8_t pinMask, InterruptSource src) {
            ie |= pinMask;
            if (src == InterruptSource::FALLING) {
                ies |= pinMask;
            } else {
                ies &= ~(pinMask);
            }
        }

        template <uint16_t addr>
        inline void Port<addr>::DisableInterrupt(uint8_t pinMask) {
            ie &= ~(pinMask);
        }


        template <uint16_t port_addr, uint16_t pinMask>
        struct Pin {
            RegisterBit<uint8_t, port_addr, pinMask> in;
            RegisterBit<uint8_t, port_addr, pinMask> out;
            RegisterBit<uint8_t, port_addr, pinMask> dir;
            RegisterBit<uint8_t, port_addr, pinMask> ren;
            RegisterBit<uint8_t, port_addr, pinMask> sel0;
            RegisterBit<uint8_t, port_addr, pinMask> sel1;
            RegisterBit<uint8_t, port_addr, pinMask> selc;
            RegisterBit<uint8_t, port_addr, pinMask> ies;
            RegisterBit<uint8_t, port_addr, pinMask> ie;
            RegisterBit<uint8_t, port_addr, pinMask> ifg;
        };

        template<uint16_t addr, uint8_t pinMask>
        using PinHandle = Pin<addr, pinMask> &;

        template<uint16_t addr>
        using PortHandle = Port<addr> &;

        extern Port<P1_BASE>        p1;
        extern Port<P2_BASE + 0x1>  p2;
        extern Port<P3_BASE>        p3;
        extern Port<P4_BASE + 0x1>  p4;
        extern Port<P5_BASE>        p5;
        extern Port<P6_BASE + 0x1>  p6;
        extern Port<P7_BASE>        p7;
        extern Port<P8_BASE + 0x1>  p8;
    }   // namespace GPIO
}   // namespace MSP430

#endif /* MSP430FR5994_GPIO_H_ */
