/*
 * drv10970.cc
 *
 *  Created on: May 25, 2021
 *      Author: sailedeer
 *
 *      This file contains a driver for DRV10970 Motor Driver IC. It exposes an interface
 *      which can be used to completely control a compatible BLDC motor.
 */

#include <cstdint>
#include <msp430.h>
#include "bsp.h"
#include "msp430fr5994/gpio.h"
#include "msp430fr5994/gpio/pin.h"
#include "msp430fr5994/gpio/port.h"
#include "msp430fr5994/timer.h"
#include "drv10970.h"

using namespace MSP430FR5994;

DRV10970::DRV10970(GPIO::Pin &brakePin, GPIO::Pin &rpmPin, GPIO::Pin &dirPin,
                   GPIO::Pin &lockPin, GPIO::Pin &pwmPin, Timer::Timer_3 &pwmTimer,
                   GPIO::CallbackFuncPtr lockCallback, GPIO::CallbackFuncPtr rpmCallback)
: _brakePin(brakePin), _rpmPin(rpmPin), _dirPin(dirPin), _lockPin(lockPin),
  _pwmPin(pwmPin), _pwmTimer(pwmTimer) {

    // Initialize pins
    _brakePin.SetMode(GPIO::Direction::OUTPUT);

    _rpmPin.SetMode(GPIO::Direction::INPUT);
    _rpmPin.EnableInterrupt(GPIO::InterruptSource::RISING, rpmCallback);

    _dirPin.SetMode(GPIO::Direction::OUTPUT);

    _lockPin.SetMode(GPIO::Direction::INPUT);
    _lockPin.EnableInterrupt(GPIO::InterruptSource::RISING, lockCallback);

    _pwmPin.SetMode(GPIO::Direction::OUTPUT);
    // sets the pin as an output for the capture/compare registers
    _pwmPin.SetFunction(GPIO::Function::PRIMARY);

    // Initialize DRV10970 output values
    _pwmTimer.ccr0.set(BSP::PWM_TIMER_PERIOD - 1);
    SetPWMDutyCycle(0);
    DisableBrake();
    SetDirectionForward();
}

void DRV10970::EnableBrake() {
//    BRKMOD_PORT_OUT |= BRKMOD_PIN;
    _brakePin.out.set();
}

void DRV10970::DisableBrake() {
//    BRKMOD_PORT_OUT &= ~BRKMOD_PIN;
    _brakePin.out.clear();
}

void DRV10970::ToggleBrake() {
//    BRKMOD_PORT_OUT ^= BRKMOD_PIN;
    _brakePin.out.toggle();
}

void DRV10970::SetDirectionForward() {
//    FR_PORT_OUT &= ~FR_PIN;
    _dirPin.out.clear();
}

void DRV10970::SetDirectionBackward() {
//    FR_PORT_OUT |= FR_PIN;
    _dirPin.out.set();
}

void DRV10970::ToggleDirection() {
//    FR_PORT_OUT ^= FR_PIN;
    if (_dir == Direction::FORWARD) {
        DRV10970::SetDirectionBackward();
    } else {
        DRV10970::SetDirectionForward();
    }
}

void DRV10970::SetPWMDutyCycle(uint8_t dutyCycle) {
    if (dutyCycle > 100) {
        dutyCycle = 100;
    }
    // don't worry about dutyCycle < 0 since this is an unsigned byte
    if (dutyCycle < 10) {
        // anything less than 10% is interpreted as 10%
        dutyCycle = 10;
    }
    if (dutyCycle == 0) {
        // this puts the device in sleep mode
        _pwmTimer.ccr1.set(0);
    } else {
        _pwmTimer.ccr1.set((uint16_t)((dutyCycle * BSP::PWM_TIMER_PERIOD) / 100 - 1));
    }
}

void DRV10970::SetPWMDutyCycleRaw(uint16_t dutyCycle) {
    if (dutyCycle > BSP::PWM_TIMER_PERIOD - 1) {
        dutyCycle = BSP::PWM_TIMER_PERIOD - 1;
    }
    if (dutyCycle < BSP::PWM_TIMER_PERIOD / 10) {
        dutyCycle = BSP::PWM_TIMER_PERIOD / 10;
    }
    // don't worry about dutyCycle < 0 since this is an unsigned short
    if (dutyCycle == 0) {
        _pwmTimer.ccr1.set(0);
    } else {
        _pwmTimer.ccr1.set(dutyCycle);
    }
}
