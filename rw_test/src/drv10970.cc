/*
 * drv10970.cc
 *
 *  Created on: May 25, 2021
 *      Author: sailedeer
 *
 *      This file contains a driver for a single Faulhaber motor. It exposes an interface
 *      which can be used to completely control the motor.
 */

#include <msp430fr5994/gpio.h>
#include <cstdint>

#include "bsp.h"
#include "drv10970.h"
#include "msp430/timer.h"

DRV10970::DRV10970(MSP430::GPIO::Pin brakePin, MSP430::GPIO::Pin freqPin,
                   MSP430::GPIO::Pin dirPin, MSP430::GPIO::Pin lockPin,
                   MSP430::GPIO::Pin pwmPin, MSP430::Timer pwmTimer) {

    // Initialize class variables
    dir = Direction::FORWARD;
    pwm_frequency = 0;  // TODO set to appropriate value
    rpm = 0;
    lock_events = 0;
    _brakePin = brakePin;
    _freqPin = freqPin;
    _dirPin = dirPin;
    _lockPin = lockPin;
    _pwmPin = pwmPin;
    _pwmTimer = pwmTimer;

    // Initialize pins

    // break mode pin = output
//    BRKMOD_PORT_DIR |= BRKMOD_PIN;
    MSP430::GPIO::SetPinDirection(brakePin, MSP430::GPIO::OUTPUT);

    // freq indicator pin = input
//    FG_PORT_DIR &= ~(FG_PIN);
    MSP430::GPIO::SetPinDirection(freqPin, MSP430::GPIO::INPUT);
//    FG_PORT_IES &= ~(FG_PIN);  // low to high edge interrupt
    MSP430::GPIO::SetInterruptEventSource(freqPin, MSP430::GPIO::LOW_TO_HIGH_EDGE);
//    FG_PORT_IE |= FG_PIN;  // enable interrupt on FG_PIN
    MSP430::GPIO::EnableInterrupt(freqPin);

    // motor direction pin = output
//    FR_PORT_DIR |= FR_PIN;
    MSP430::GPIO:SetPinDirection(dirPin, MSP430::GPIO::OUTPUT);

    // lock indicator = input
//    RD_PORT_DIR &= ~(RD_PIN);
    MSP430::GPIO::SetPinDirection(lockPin, MSP430::GPIO::INPUT);
//    RD_PORT_IES &= ~(RD_PIN);
    MSP430::GPIO::SetInterruptEventSource(lockPin, MSP430::GPIO::LOW_TO_HIGH_EDGE);
//    RD_PORT_IE |= RD_PIN;
    MSP430::GPIO::EnableInterrupt(lockPin);

    // Initialize DRV10970 output values
    // TODO: PWM stuff
    DisableBrake();
    SetDirectionForward();
}

void DRV10970::EnableBrake() {
//    BRKMOD_PORT_OUT |= BRKMOD_PIN;
    MSP430::GPIO::SetPinValue(_brakePin, MSP430::GPIO::HIGH);
}

void DRV10970::DisableBrake() {
//    BRKMOD_PORT_OUT &= ~BRKMOD_PIN;
    MSP430::GPIO::SetPinValue(_brakePin, MSP430::GPIO::LOW);
}

void DRV10970::ToggleBrake() {
//    BRKMOD_PORT_OUT ^= BRKMOD_PIN;
    MSP430::GPIO::TogglePinValue(_brakePin);
}

void DRV10970::SetDirectionForward() {
//    FR_PORT_OUT &= ~FR_PIN;
    MSP430::GPIO::SetValue(_dirPin, MSP430::GPIO::LOW);
}

void DRV10970::SetDirectionBackward() {
//    FR_PORT_OUT |= FR_PIN;
    MSP430::GPIO::SetValue(_dirPin, MSP430::GPIO::HIGH);
}

void DRV10970::ToggleDirection() {
//    FR_PORT_OUT ^= FR_PIN;
    MSP430::GPIO::
}

void DRV10970::SetPWMFrequency(uint16_t frequency) {

}

void DRV10970::SetPWMDutyCycle(uint8_t dutyCycle) {

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
