/*
 * rw.cpp
 *
 *  Created on: May 25, 2021
 *      Author: sailedeer
 *
 *      This file contains a driver for a single Faulhaber motor. It exposes an interface
 *      which can be used to completely control the motor.
 */

#include <stdint.h>
#include "rw.h"
#include "PID.h"

using namespace RW;
using namespace reaction_wheels_pid;

ReactionWheel::ReactionWheel(float& input, float& output, float& setpoint)
    : _rw_status({0}), _pid(input, output, setpoint, Kp, Kd, Ki), _lock_events(0) {

    // initialize PWM timer and pins
}

ReactionWheel::~ReactionWheel() {}

void ReactionWheel::SetTargetRPM(float rpm) {
    _rw_status.rpm = rpm;

    // TODO: update the PWM frequency
}

void ReactionWheel::SetMaxMin(float min, float max) {
    _pid.ClampingOn(min, max);
}

void ReactionWheel::EnableBrake() {
    _rw_status.brake = true;

    // TODO: pull brake pin high
    RW_BRKMOD_OUT |= BRKMOD_BIT;
}

void ReactionWheel::DisableBrake() {
    _rw_status.brake = false;

    // TODO: pull brake pin low
    RW_BRKMOD_OUT &= ~BRKMOD_BIT;
}

void ReactionWheel::ToggleBrake() {

    if (_rw_status.brake) {
        RW_BRKMOD_OUT &= ~BRKMOD_BIT;
    } else {
        RW_BRKMOD_OUT |= BRKMOD_BIT;
    }
    _rw_status.brake = !_rw_status.brake;

    // TODO: flip brake pin

}

void ReactionWheel::SetDirectionForward() {
    _rw_status.dir = Direction::FORWARD;

    // TODO: pull dir pin low
}

void ReactionWheel::SetDirectionBackward() {
    _rw_status.dir = Direction::BACKWARD;

    // TODO: pull dir pin high
}

void ReactionWheel::ToggleDirection() {
    _rw_status.dir = _rw_status.dir == Direction::FORWARD ? Direction::BACKWARD : Direction::FORWARD;

    // TODO: flip dir pin
}
