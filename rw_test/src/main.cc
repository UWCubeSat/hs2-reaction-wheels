/*
 * main.cc
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#include <cstdint>

#include "bsp.h"
#include "msp430fr5994/gpio.h"
#include "msp430fr5994/timer.h"
#include "drv10970.h"
#include "pid.h"

using namespace MSP430FR5994;

static const float Kp = 0;
static const float Ki = 0;
static const float Kd = 0;


int main() {
    BSP_Init();

    float setPoint = 0;
    float input = 0;
    float output = 0;
    GPIO::Pin brakePin(GPIO::PORT3, BRKMOD_PIN);
    GPIO::Pin rpmPin(GPIO::PORT1, FG_PIN);
    GPIO::Pin dirPin(GPIO::PORT1, FR_PIN);
    GPIO::Pin lockPin(GPIO::PORT3, RD_PIN);
    GPIO::Pin pwmPin(GPIO::PORT1, PWM_PIN);
    Timer::PWM pwmTimer(0);

    DRV10970 motor(brakePin, rpmPin, dirPin, lockPin, pwmPin, pwmTimer);
    PID pid(setPoint, input, output, Kp, Ki, Kd);

    // BNO055 imu(intrPin);

    while (1) {
        pid.Iterate();
    }
}
