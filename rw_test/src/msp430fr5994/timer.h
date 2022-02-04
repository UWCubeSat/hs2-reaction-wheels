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
                Register<uint16_t, OFS_TA0CTL> ctl;
                Register<uint16_t, OFS_TA0CCTL0> cctl0;
                Register<uint16_t, OFS_TA0CCTL1> cctl1;
                Register<uint16_t, OFS_TA0R> r;
                Register<uint16_t, OFS_TA0CCR0> ccr0;
                Register<uint16_t, OFS_TA0CCR1> ccr1;
                Register<uint16_t, OFS_TA0IV> iv;
                Register<uint16_t, OFS_TA0EX0> ex0;
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
