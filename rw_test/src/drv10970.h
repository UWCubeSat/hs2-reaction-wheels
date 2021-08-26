/*
 * drv10970.h
 *
 *  Created on: May 25, 2021
 *      Author: sailedeer
 */

#ifndef DRV10970_H_
#define DRV10970_H_

#include "bsp.h"

#include <cstdint>

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

//enum class Result {
//    NOERROR,
//    ERROR,
//};
//
//enum class Direction {
//    FORWARD = 0,
//    BACKWARD,
//};
//
//typedef struct rw_status_st {
//    bool locked;
//    bool brake;
//    uint32_t lock_events;
//    float rpm;
//    float pwm_frequency;
//    float error;
//    Direction dir;
//} Status;
//
//class ReactionWheel {
//    public:
//        // constructor
//        ReactionWheel(float& input, float& output, float& setpoint);
//
//        // destructor
//        ~ReactionWheel();
//
//        // copy constructor - disable so we don't reuse pins
//        ReactionWheel(const ReactionWheel &rw) = delete;
//
//        // assignment operator - disable so we don't reuse pins
//        ReactionWheel& operator =(const ReactionWheel& b) = delete;
//
//        // sets new motor RPM
//        void SetTargetRPM(float rpm);
//
//        // set max and min RPM
//        void SetMaxMin(float min, float max);
//
//        // turns on brake
//        void EnableBrake();
//
//        // turns off brake
//        void DisableBrake();
//
//        // toggle brake
//        void ToggleBrake();
//
//        // sets motor to forward
//        void SetDirectionForward();
//
//        // sets motor to backward
//        void SetDirectionBackward();
//
//        // toggle's motor direction
//        void ToggleDirection();
//
//        // get how many times a lock event has occurred
//        uint16_t GetLockEventCount() {return _lock_events; }
//
//        // returns the current status of the ReactionWheel
//        Status GetReactionWheelStatus() { return _rw_status; }
//    private:
//        static const float Kp = 0.0f;
//        static const float Ki = 0.0f;
//        static const float Kd = 0.0f;
//
//        // used by SetTargetRPM to update desired PWM frequency based on new RPM
//        void SetPWMFrequency(uint16_t frequency);
//
//        // Ticks system one step forward
//        float Tick(float setpoint);
//
//        // configures MSP430 pins
//
//        /* TODO: lots -
//         *      BNO055 IMU driver
//         *      Internal State Machine
//         *      COSMOS integration - inheritance from COSMOS class? this capability could be cool for future projects
//         *      Some way to store information about our system for debugging
//         *          Direction
//         *          Braking?
//         *          RPM
//         *          CurrentError
//         *          RecentError
//         *          TIME?
//         *          Locked
//         */
//
//        // current status of the reaction wheel system
//        Status _rw_status;
//
//        // number of detected lock events
//        uint32_t _lock_events;
//};

#endif /* DRV10970_H_ */
