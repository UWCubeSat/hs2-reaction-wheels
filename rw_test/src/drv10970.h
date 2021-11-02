/*
 * drv10970.h
 *
 *  Created on: May 25, 2021
 *      Author: sailedeer
 */

#ifndef DRV10970_H_
#define DRV10970_H_

#include "bsp.h"
#include "pwm.h"

#include <cstdint>

//enum class Result {
//    NOERROR,
//    ERROR,
//};

enum class Direction {
    FORWARD = 0,
    BACKWARD = 1,
};


class DRV10970 {
    public:
        // Constructor
        DRV10970();

        // Delete copy constructor
        DRV10970(const DRV10970 &) = delete;

        // Destructor
        ~DRV10970();

        // turns on brake
        void EnableBrake();

        // turns off brake
        void DisableBrake();

        // toggle brake
        void ToggleBrake();

        // get brake status
        uint16_t GetBrake() { return BRKMOD_PORT_OUT & BRKMOD_PIN; }

        // sets motor to forward
        void SetDirectionForward();

        // sets motor to backward
        void SetDirectionBackward();

        // toggle's motor direction
        void ToggleDirection();

        // get current motor direction
        uint16_t GetDirection() { return FR_PORT_OUT & FR_PIN; }

        // get the current lock status
        uint16_t GetLockStatus() { return ~(RD_PORT_IN & RD_PIN); }

        // get how many times a lock event has occurred
        uint16_t GetLockEventCount() { return lock_events; }

        // update PWM frequency
        void SetPWMFrequency(uint16_t frequency);

    private:
        const uint8_t POLES = 2;
        const float MIL_TO_MIN = 60000;
        const uint8_t ROTS_TO_COUNT = 3;
        Direction dir;

        float pwm_frequency;
        float rpm;
        uint32_t lock_events;  // number of detected lock events
};

#endif /* DRV10970_H_ */
