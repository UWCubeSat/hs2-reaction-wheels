/*
 * drv10970.cc
 *
 *  Created on: May 25, 2021
 *      Author: sailedeer
 *
 *      This file contains a driver for a single Faulhaber motor. It exposes an interface
 *      which can be used to completely control the motor.
 */

#include "drv10970.h"
#include <cstdint>

DRV10970::DRV10970() {
    // Initialize class variables
    dir = Direction::FORWARD;
    pwm_frequency = 0;  // TODO set to appropriate value
    rpm = 0;
    lock_events = 0;

    // Initialize DRV10970 port IO
    PWM_PORT_DIR |= PWM_PIN;
    BRKMOD_PORT_DIR |= BRKMOD_PIN;
    FG_PORT_DIR &= ~FG_PIN;
    FR_PORT_DIR |= FR_PIN;
    RD_PORT_DIR &= ~RD_PIN;

    // Initialize pull-up resistors on inputs
    FG_PORT_REN |= FG_PIN;
    FG_PORT_OUT |= FG_PIN;
    RD_PORT_REN |= RD_PIN;
    RD_PORT_OUT |= RD_PIN;

    // Initialize PWM port
    PWM_PORT_SEL0 |= PWM_PIN;

    // Initialize DRV10970 output values
    // TODO: PWM stuff
    DisableBrake();
    SetDirectionForward();
}

void DRV10970::EnableBrake() {
    BRKMOD_PORT_OUT |= BRKMOD_PIN;
}

void DRV10970::DisableBrake() {
    BRKMOD_PORT_OUT &= ~BRKMOD_PIN;
}

void DRV10970::ToggleBrake() {
    BRKMOD_PORT_OUT ^= BRKMOD_PIN;
}

void DRV10970::SetDirectionForward() {
    FR_PORT_OUT &= ~FR_PIN;
}

void DRV10970::SetDirectionBackward() {
    FR_PORT_OUT |= FR_PIN;
}

void DRV10970::ToggleDirection() {
    FR_PORT_OUT ^= FR_PIN;
}

void DRV10970::SetPWMFrequency(uint16_t frequency) {

}
