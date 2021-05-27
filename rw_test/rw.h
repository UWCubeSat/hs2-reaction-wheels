/*
 * rw.h
 *
 *  Created on: May 25, 2021
 *      Author: sailedeer
 */

#ifndef RW_H_
#define RW_H_

#include <stdint.h>
#include "PID.h"

namespace ReactionWheel {
// RW Pin Registers
// TODO: define all these with the final pin assignments
#define PWM_PORT_DIR
#define PWM_PORT_SEL0
#define PWM_PORT_SEL1
#define PWM_PORT_PIN

#define BRKMOD_PORT_DIR
#define BRKMOD_PORT_OUT
#define BRKMOD_PORT_SEL0
#define BRKMOD_PORT_SEL1
#define BRKMOD_PORT_PIN

#define RD_PORT_DIR
#define RD_PORT_OUT
#define RD_PORT_SEL0
#define RD_PORT_SEL1
#define RD_PORT_PIN

#define FG_PORT_DIR
#define FG_PORT_OUT
#define FG_PORT_SEL0
#define FG_PORT_SEL1
#define FG_PORT_PIN

#define RD_PORT_DIR
#define RD_PORT_OUT
#define RD_PORT_SEL0
#define RD_PORT_SEL1
#define RD_PORT_PIN

enum class Result {
    NOERROR,
    ERROR,
};

enum class Direction {
    FORWARD,
    BACKWARD,
};

// store any private members of the namespace here so no other TU's can see it
namespace {
    // represents possible states the ReactionWheel can be in
    enum class State {
        START,
        IDLE,
        BRAKING,
        SETTLING_FORWARD,
        SETTLING_BACKWARD,
        FORWARD,
        BACKWARD,
    };
}   // anonymous namespace

typedef struct rw_status_st {
    bool locked;
    bool brake;
    uint32_t lock_events;
    float rpm;
    float pwm_frequency;
    Direction dir;
} Status;

class ReactionWheel {
    public:
        // constructor
        ReactionWheel(PID& pid);

        // destructor
        ~ReactionWheel();

        // copy constructor - disable so we don't reuse pins
        ReactionWheel(const ReactionWheel &rw) = delete;

        // assignment operator - disable so we don't reuse pins
        ReactionWheel& operator =(const ReactionWheel& b) = delete;

        // sets new motor RPM
        Result SetTargetRPM(float RPM);

        // set max and min RPM
        Result SetMaxMin(float min, float max);

        // turns on brake
        Result EnableBrake();

        // turns off brake
        Result DisableBrake();

        // toggle brake
        Result ToggleBrake();

        // sets motor to forward
        Result SetDirectionForward();

        // sets motor to backward
        Result SetDirectionBackward();

        // toggle's motor direction
        Result ToggleDirection();

        // get how many times a lock event has occurred
        uint16_t GetLockEventCount();

        // returns the current state of the ReactionWheel
        State GetReactionWheelState();

        // returns the current status of the ReactionWheel
        Status GetReactionWheelStatus();

        // returns reference to underlying PID
        PID& GetPIDHandle();

        // set PID constants
        void SetGain(float kp, float ki, float kd);
    private:
        // minimum error before we consider the PID settled; tunable?
        static const float MIN_ERROR = 0.05f;

        // current state of the system
        State state = State::START;

        // used by SetTargetRPM to update desired PWM frequency based on new RPM
        Result SetPWMFrequency(uint16_t frequency);

        // tick of underlying state machine
        void Tick();

        // configures MSP430 pins

        /* TODO: lots -
         *      BNO055 IMU driver
         *      Internal State Machine
         *      COSMOS integration - inheritance from COSMOS class? this capability could be cool for future projects
         *      Some way to store information about our system for debugging
         *          Direction
         *          Braking?
         *          RPM
         *          CurrentError
         *          RecentError
         *          TIME?
         *          Locked
         */

        // current status of the reaction wheel system
        Status _rw_status;

        // PID controller used for this wheel
        PID& _pid;

        // BNO055 IMU driver
        // BNO055 imu;  // TODO: write this
};

// PID constants for our controller
// TODO: characterize these constants
#define KD
#define KP
#define KI

// COSMOS stuff
// TODO: learn COSMOS
}   // namespace ReactionWheel

#endif /* RW_H_ */
