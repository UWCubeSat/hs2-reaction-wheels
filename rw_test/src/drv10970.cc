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
#include <msp430fr5994/timer.h>

using namespace MSP430FR5994;

DRV10970::DRV10970(GPIO::Pin brakePin, GPIO::Pin freqPin,
                   GPIO::Pin dirPin, GPIO::Pin lockPin,
                   GPIO::Pin pwmPin, Timer::Handle pwmTimer) {

    // Initialize class variables
    _dir = Direction::FORWARD;
    _pwm_frequency = 0;  // TODO set to appropriate value
    _rpm = 0;
    _lock_events = 0;
    _brakePin = brakePin;
    _freqPin = freqPin;
    _dirPin = dirPin;
    _lockPin = lockPin;
    _pwmPin = pwmPin;
    _pwmTimer = pwmTimer;

    // Initialize pins
    GPIO::SetPinDirection(brakePin, GPIO::OUTPUT);

    GPIO::SetPinDirection(freqPin, GPIO::INPUT);
    GPIO::SetInterruptEventSource(freqPin, GPIO::LOW_TO_HIGH_EDGE);
    GPIO::EnableInterrupt(freqPin);

    GPIO::SetPinDirection(dirPin, GPIO::OUTPUT);

    GPIO::SetPinDirection(lockPin, GPIO::INPUT);
    GPIO::SetInterruptEventSource(lockPin, GPIO::LOW_TO_HIGH_EDGE);
    GPIO::EnableInterrupt(lockPin);

    // Initialize DRV10970 output values
    // TODO: PWM stuff
    DisableBrake();
    SetDirectionForward();
}

void DRV10970::EnableBrake() {
//    BRKMOD_PORT_OUT |= BRKMOD_PIN;
    GPIO::SetPinValue(_brakePin, GPIO::HIGH);
}

void DRV10970::DisableBrake() {
//    BRKMOD_PORT_OUT &= ~BRKMOD_PIN;
    GPIO::SetPinValue(_brakePin, GPIO::LOW);
}

void DRV10970::ToggleBrake() {
//    BRKMOD_PORT_OUT ^= BRKMOD_PIN;
    GPIO::TogglePinValue(_brakePin);
}

void DRV10970::SetDirectionForward() {
//    FR_PORT_OUT &= ~FR_PIN;
    GPIO::SetPinValue(_dirPin, GPIO::LOW);
}

void DRV10970::SetDirectionBackward() {
//    FR_PORT_OUT |= FR_PIN;
    GPIO::SetPinValue(_dirPin, GPIO::HIGH);
}

void DRV10970::ToggleDirection() {
//    FR_PORT_OUT ^= FR_PIN;
    if (_dir == FORWARD) {
        DRV10970::SetDirectionForward();
    } else {
        DRV10970::SetDirectionBackward();
    }
}

void DRV10970::SetPWMFrequency(uint16_t frequency) {

}

void DRV10970::SetPWMDutyCycle(uint8_t dutyCycle) {

}

//#pragma vector=FG_PORT_VECTOR
//__interrupt void DRV10970::monitorRPM() {
//    _disable_interrupt();
//    _pulseCount++;
//    if (_pulseCount == POLES * ROTS_TO_COUNT) {
//        unsigned long currentTime = BSP_GetMET();
//        unsigned long elapsedTime = currentTime - _lastRotTime;
//        _rpm = MIL_TO_MIN / ((float) elapsedTime / ROTS_TO_COUNT);
//    }
//    _enable_interrupt();
//}
