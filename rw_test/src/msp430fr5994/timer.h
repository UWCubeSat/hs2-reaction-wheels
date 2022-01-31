/*
 * timer.h
 *
 *  Created on: Jan 31, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_TIMER_H_
#define MSP430FR5994_TIMER_H_

#include <cstdint>
#include <msp430.h>
#include "msp430fr5994-hsl.h"

namespace MSP430FR5994 {
    namespace Timer {
        class Timer_2 {
            public:
                Timer_2(uint16_t baseAddress);
                Register<uint16_t, 0x00> ctl;
                Register<uint16_t, 0x02> cctl0;
                Register<uint16_t, 0x04> cctl1;
                Register<uint16_t, 0x10> r;
                Register<uint16_t, 0x12> ccr0;
                Register<uint16_t, 0x14> ccr1;
                Register<uint16_t, 0x2E> iv;
                Register<uint16_t, 0x20> ex0;
            private:
                uint16_t _baseAddress;
        };  // class Timer_2

        class Timer_3 : public Timer_2 {
            public:
                Timer_3(uint16_t baseAddress);
                Register<uint16_t, 0x06> cctl2;
                Register<uint16_t, 0x16> ccr2;
        };  // class Timer_3
    }   // namespace Timer
}   // namespace MSP430FR5994
#endif /* MSP430FR5994_TIMER_H_ */
