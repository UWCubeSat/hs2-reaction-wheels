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

volatile uint16_t dutyCycle = 0;

inline void incrementDutyCycle() {
    if (++dutyCycle > 100) {
        dutyCycle = 100;
    }
}

inline void decrementDutyCycle() {
    if (--dutyCycle > 100) {
        dutyCycle = 100;
    }
}

int main() {
    BSP::Init();
    // Set SEL0 = 1, SEL1 = 0
    uint16_t dutyCycle = 0;
    BSP::pwmPin.SetFunction(GPIO::Function::PRIMARY);
    // ccr0 (period) to 32767 (32678 states), ccr1 (duty cycle) to 0%
    BSP::pwmTimer.ccr0.set(BSP::PWM_TIMER_PERIOD - 1);
    GPIO::Pin &upButtonPin = BSP::gp1;
    GPIO::Pin &downButtonPin = BSP::gp2;

    upButtonPin.SetMode(GPIO::Direction::INPUT);
    upButtonPin.EnableInterrupt(GPIO::InterruptSource::FALLING, incrementDutyCycle);
    downButtonPin.SetMode(GPIO::Direction::INPUT);
    downButtonPin.EnableInterrupt(GPIO::InterruptSource::FALLING, decrementDutyCycle);

    while (1) {
        // set the duty cycle and wait for input from a button to increment
        BSP::pwmTimer.ccr1.set(dutyCycle);
        __bis_SR_register(LPM0_bits | GIE);
    }
}
