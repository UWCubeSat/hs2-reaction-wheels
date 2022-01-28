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
            private:
                reg_st<uint8_t, addr, 0x00> in;
                reg_st<uint8_t, addr, 0x02> out;
                reg_st<uint8_t, addr, 0x04> dir;
                reg_st<uint8_t, addr, 0x06> ren;
                reg_st<uint8_t, addr, 0x0A> sel0;
                reg_st<uint8_t, addr, 0x0C> sel1;
                reg_st<uint8_t, addr, 0x16> selc;
                reg_st<uint8_t, addr, 0x18> ies;
                reg_st<uint8_t, addr, 0x1A> ie;
                reg_st<uint8_t, addr, 0x1C> ifg;

            public:
                inline void setMode(Direction dir, uint8_t pinMask);
                inline void setFunction(Function func, uint8_t pinMask);
                inline void setResistor(Resistor res, uint8_t pinMask);
        };

        template <uint16_t addr>
        inline void Port<addr>::setMode(Direction mode, uint8_t pinMask) {
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
        inline void Port<addr>::setFunction(Function func, uint8_t pinMask) {
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
        inline void Port<addr>::setResistor(Resistor res, uint8_t pinMask) {
            switch (res) {
                case Resistor::PULLUP: {
                    break;
                }
                case Resistor::PULLDOWN: {
                    break;
                }
            }
        }

        template <uint16_t addr, uint8_t pinMask>
        struct Pin {
            bit_st<uint8_t, addr, pinMask> in;
            bit_st<uint8_t, addr, pinMask> out;
            bit_st<uint8_t, addr, pinMask> dir;
            bit_st<uint8_t, addr, pinMask> ren;
            bit_st<uint8_t, addr, pinMask> sel0;
            bit_st<uint8_t, addr, pinMask> sel1;
            bit_st<uint8_t, addr, pinMask> selc;
            bit_st<uint8_t, addr, pinMask> ies;
            bit_st<uint8_t, addr, pinMask> ie;
            bit_st<uint8_t, addr, pinMask> ifg;
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
