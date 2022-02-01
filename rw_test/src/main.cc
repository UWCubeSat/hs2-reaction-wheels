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

volatile uint16_t dutyCycle = 0;
volatile bool brakePressed = false;
volatile bool changeDirPressed = false;

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
}

inline void decrementDutyCycle() {
    if (--dutyCycle > 100) {
        dutyCycle = 0;
    }
}


int main() {
    BSP::Init();
    GPIO::Pin &upButtonPin = BSP::gp1;
    GPIO::Pin &downButtonPin = BSP::gp2;
    GPIO::Pin &brakeButtonPin = BSP::gp3;
    GPIO::Pin &dirButtonPin = BSP::gp4;
    DRV10970 motor(BSP::brakePin, BSP::rpmPin, BSP::dirPin, BSP::lockPin,
                   BSP::pwmPin, BSP::pwmTimer, nullptr, nullptr);

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
        __bis_SR_register(LPM0_bits | GIE);
    }
}
