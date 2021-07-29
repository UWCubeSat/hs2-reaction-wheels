/*
 * rw_test.cpp
 *
 *  Created on: May 19, 2021
 *      Author: sailedeer
 */

#include <msp430.h>

#include "rw_test.h"
#include "PID.h"
#include "rw.h"

using namespace reaction_wheels_pid;

int main(void) {
    // TODO: fix all this nonsense to use real stuff
    float input = 0.0f;     // this will be tied to our current RPM from FG
    float output = 0.0f;    // this will be the new RPM
    float setpoint = 0.0f;  // based on some dynamics model of the system
    float kp = 0.0f;        // need to tune these
    float kd = 0.0f;
    float ki = 0.0f;

    PID pid(input, output, setpoint, kp, kd, ki);

    // constructs a ReactionWheel object to control the Faulhaber motors
    RW::ReactionWheel rw();

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

    // initialize EUSCI bus B (or are we using i2c.c?)
    //  an opportunity has presented itself to rewrite the I2C implementation to be asynchronous and interrupt based
}
