/*
 * drv10970.cc
 *
 *  Created on: May 25, 2021
 *      Author: sailedeer
 *
 *      This file contains a driver for DRV10970 Motor Driver IC. It exposes an interface
 *      which can be used to completely control a compatible motor.
 */

#include <cstdint>
#include <msp430fr5994/gpio.h>
#include <bsp.h>
#include <drv10970.h>

using namespace MSP430FR5994;

DRV10970::DRV10970(GPIO::Pin &brakePin, GPIO::Pin &rpmPin,
                   GPIO::Pin &dirPin, GPIO::Pin &lockPin,
                   GPIO::Pin &pwmPin) :
                           _brakePin(brakePin), _rpmPin(rpmPin),
                           _dirPin(dirPin), _lockPin(lockPin),
                           _pwmPin(pwmPin) {

    // Initialize pins
    _brakePin.SetDirection(GPIO::Pin::OUTPUT);

    _rpmPin.SetDirection(GPIO::Pin::INPUT);
    _rpmPin.SetInterruptEventSource(GPIO::Pin::LOW_TO_HIGH_EDGE);
    _rpmPin.EnableInterrupt();

    _dirPin.SetDirection(GPIO::Pin::OUTPUT);

    _lockPin.SetDirection(GPIO::Pin::INPUT);
    _lockPin.SetInterruptEventSource(GPIO::Pin::LOW_TO_HIGH_EDGE);
    _lockPin.EnableInterrupt();

    // Initialize DRV10970 output values
    // TODO: PWM stuff
    DisableBrake();
    SetDirectionForward();
}

void DRV10970::EnableBrake() {
//    BRKMOD_PORT_OUT |= BRKMOD_PIN;
    _brakePin.Write(GPIO::Pin::HIGH);
}

void DRV10970::DisableBrake() {
//    BRKMOD_PORT_OUT &= ~BRKMOD_PIN;
    _brakePin.Write(GPIO::Pin::LOW);
}

void DRV10970::ToggleBrake() {
//    BRKMOD_PORT_OUT ^= BRKMOD_PIN;
    _brakePin.ToggleOutput();
}

void DRV10970::SetDirectionForward() {
//    FR_PORT_OUT &= ~FR_PIN;
    _dirPin.Write(GPIO::Pin::LOW);
}

void DRV10970::SetDirectionBackward() {
//    FR_PORT_OUT |= FR_PIN;
    _dirPin.Write(GPIO::Pin::HIGH);
}

void DRV10970::ToggleDirection() {
//    FR_PORT_OUT ^= FR_PIN;
    if (_dir == FORWARD) {
        DRV10970::SetDirectionBackward();
    } else {
        DRV10970::SetDirectionForward();
    }
}

void DRV10970::SetPWMDutyCycle(uint8_t dutyCycle) {
    __no_operation();
    // _pwmTimer.SetCC(dutyCycle);
}
