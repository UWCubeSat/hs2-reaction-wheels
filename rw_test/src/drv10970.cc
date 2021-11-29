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
#include "bsp.h"

DRV10970::DRV10970() {
    // Initialize class variables
    dir = Direction::FORWARD;
    pwm_frequency = 0;  // TODO set to appropriate value
    rpm = 0;
    lock_events = 0;

    // Initialize pins

    // break mode pin = output
    BRKMOD_PORT_DIR |= BRKMOD_PIN;

    // freq indicator pin = input
    FG_PORT_DIR &= ~(FG_PIN);
    FG_PORT_IES &= ~(FG_PIN);  // low to high edge interrupt
    FG_PORT_IE |= FG_PIN;  // enable interrupt on FG_PIN

    // motor direction pin = output
    FR_PORT_DIR |= FR_PIN;

    // lock indicator = input
    RD_PORT_DIR &= ~(RD_PIN);
    RD_PORT_IES &= ~(RD_PIN);
    RD_PORT_IE |= RD_PIN;

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

#pragma vector=FG_PORT_VECTOR
__interrupt void DRV10970::monitorRPM() {
    _disable_interrupt();
    pulseCount++;
    if (pulseCount == POLES * ROTS_TO_COUNT) {
        unsigned long currentTime = BSP_GetMET();
        unsigned long elapsedTime = currentTime - lastRotTime;
        rpm = MIL_TO_MIN / ((float) elapsedTime / ROTS_TO_COUNT);
    }
    _enable_interrupt();
}
