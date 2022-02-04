/*
 * main.cc
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#include <cstdint>
#include <msp430.h>

#include "bsp.h"
#include "msp430fr5994/gpio.h"
#include "msp430fr5994/gpio/pin.h"

using namespace MSP430FR5994;

int main() {
    static const uint16_t INCREMENT = BSP::PWM_TIMER_PERIOD / 4;
    static const uint16_t MIN_DUTY_CYCLE = BSP::PWM_TIMER_PERIOD / 10;

    BSP::Init();
    // Set SEL0 = 1, SEL1 = 0 (0b01)
    BSP::pwmPin.SetMode(GPIO::Direction::OUTPUT);
    BSP::pwmPin.SetFunction(GPIO::Function::PRIMARY);
    // ccr0 (period) to 32767 (32768 states), ccr1 (duty cycle) to 10%
    BSP::pwmTimer.ccr0.set(BSP::PWM_TIMER_PERIOD - 1);
    BSP::pwmTimer.ccr1.set(MIN_DUTY_CYCLE);
    BSP::pwmTimer.cctl1.set(OUTMOD_7);
    BSP::pwmTimer.ctl.set(TASSEL__SMCLK | MC__UP | TACLR);

    while (1) {
        for (uint8_t i = 1; i <= 3; i++) {
            BSP::pwmTimer.ccr1.set(i * INCREMENT);
            __delay_cycles(4000000);
        }
        BSP::pwmTimer.ccr1.set(BSP::PWM_TIMER_PERIOD - 1);
        __delay_cycles(8000000);
        for (uint8_t i = 3; i >= 1; i--) {
            BSP::pwmTimer.ccr1.set(i * INCREMENT);
            __delay_cycles(4000000);
        }
        BSP::pwmTimer.ccr1.set(MIN_DUTY_CYCLE);
    }
}
