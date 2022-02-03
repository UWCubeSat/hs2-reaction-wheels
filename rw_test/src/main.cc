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
#include "msp430fr5994/timer.h"
#include "drv10970.h"

using namespace MSP430FR5994;

volatile bool brakePressed = false;
volatile bool changeDirPressed = false;
volatile bool stopped = false;
volatile bool updateDutyCycle = false;

volatile uint16_t dutyCycle = 0;
volatile uint16_t rpm = 0;
volatile uint8_t pulseCount = 0;
volatile uint32_t lastRotTime = 0;

inline void brake() {
    brakePressed = true;
}

inline void direction() {
    changeDirPressed = true;
}

inline void incrementDutyCycle() {
    if (++dutyCycle > 100) {
        dutyCycle = 100;
    }
    updateDutyCycle = true;
}

inline void decrementDutyCycle() {
    if (--dutyCycle > 100) {
        dutyCycle = 0;
    }
    updateDutyCycle = true;
}

inline void monitorRPM() {
    static const uint8_t POLES = 2;
    static const uint8_t ROTS_TO_COUNT = 3;
    static const uint8_t NUM_PULSES = POLES * ROTS_TO_COUNT;
    static const uint32_t MILL_TO_RPM = 60000 * ROTS_TO_COUNT;
    pulseCount++;
    if (pulseCount == NUM_PULSES) {
       uint32_t currentTime = BSP::MET();
       uint32_t elapsedTime = currentTime - lastRotTime;
       rpm = MILL_TO_RPM / elapsedTime;
       pulseCount = 0;
    }
}

int main() {
    BSP::Init();
    GPIO::Pin &upButtonPin = BSP::gp1;
    GPIO::Pin &downButtonPin = BSP::gp2;
    GPIO::Pin &brakeButtonPin = BSP::gp3;
    GPIO::Pin &dirButtonPin = BSP::gp4;
    DRV10970 motor(BSP::brakePin, BSP::rpmPin, BSP::dirPin, BSP::lockPin,
                   BSP::pwmPin, BSP::pwmTimer, nullptr, monitorRPM);

    upButtonPin.SetMode(GPIO::Direction::INPUT);
    upButtonPin.EnableInterrupt(GPIO::InterruptSource::FALLING, incrementDutyCycle);
    downButtonPin.SetMode(GPIO::Direction::INPUT);
    downButtonPin.EnableInterrupt(GPIO::InterruptSource::FALLING, decrementDutyCycle);
    brakeButtonPin.SetMode(GPIO::Direction::INPUT);
    brakeButtonPin.EnableInterrupt(GPIO::InterruptSource::FALLING, brake);
    dirButtonPin.SetMode(GPIO::Direction::INPUT);
    dirButtonPin.EnableInterrupt(GPIO::InterruptSource::FALLING, direction);

    while (1) {
        if (brakePressed) {
            motor.ToggleBrake();
            brakePressed = false;
        }
        if (changeDirPressed) {
            motor.ToggleDirection();
            changeDirPressed = false;
        }
        if (updateDutyCycle) {
            updateDutyCycle = false;
            motor.SetPWMDutyCycleRaw(dutyCycle);
        }
        __bis_SR_register(LPM0_bits | GIE);
    }
}
