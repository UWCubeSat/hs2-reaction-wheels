/*
 * timer.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: sailedeer
 */
#include <cstdint>
#include <msp430.h>

#include "timer.h"

using namespace MSP430FR5994::Timer;

Timer_2::Timer_2(uint16_t baseAddress)
: ctl(baseAddress), cctl0(baseAddress),
  cctl1(baseAddress), r(baseAddress), ccr0(baseAddress),
  ccr1(baseAddress), iv(baseAddress), ex0(baseAddress),
  _baseAddress(baseAddress)
{

}

Timer_3::Timer_3(uint16_t baseAddress)
: Timer_2(baseAddress), cctl2(baseAddress), ccr2(baseAddress)
{

}

