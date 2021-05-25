/*
 * rw_test.cpp
 *
 *  Created on: May 19, 2021
 *      Author: sailedeer
 */

#include <msp430.h>

#include "rw_test.h"
#include "PID.h"

int main(void) {
    // initialize the 5 ports we need:
    //  PWM - analog, motor RPM, out
    //  BRKMOD - digital, brakes, out
    //  FR - digital, Forward/Reverse, out
    //  FG - digital, measure RPM, in, <--- tie this to an interrupt
    //  RD - digital, lock condition, in, <---- probably tie this to an interrupt too

    // initialize timers (could also use timer.c)   <--- it's a pretty spiffy little library, so this might be the way to go
    //  PWM Timer
    //  PID Timer
    //  RPM Timer

    // initialize EUSCI bus (or are we using i2c.c?)
    //  an opportunity has presented itself to rewrite the I2C implementation to be asynchronous and interrupt based
}
