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

/*
 * The following ports must be made available through defines in bsp.h. See the DRV10970 datasheet for functions.
 * https://www.ti.com/lit/ds/symlink/drv10970.pdf?ts=1633914849399&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FDRV10970
 *   PWM:
 *      PWM_PORT_DIR
 *      PWM_PORT_SEL0
 *      PWM_PORT_SEL1
 *      PWM_PIN
 *   Brake Mode:
 *      BRKMOD_PORT_DIR
 *      BRKMOD_PIN
 *   Frequency Indicator:
 *      FG_PORT_DIR
 *      FG_PORT_IES
 *      FG_PORT_IE
 *      FG_PIN
 *   Motor Direction:
 *      FR_PORT_DIR
 *      FR_PIN
 *   Lock Indicator::
 *      RD_PORT_DIR
 *      RD_PORT_IES
 *      RD_PORT_IE
 */

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

        // get the RPM of the motor
        float GetRPM() { return rpm; }

        // triggered by interrupts to update rpm calculation values
        __interrupt void monitorRPM();

    private:
        const uint8_t POLES = 2;
        const float MIL_TO_MIN = 60000;
        const uint8_t ROTS_TO_COUNT = 3;
        Direction dir;

        float pwm_frequency;
        PWM pwm;
        uint32_t lock_events;  // number of detected lock events

        float rpm;
        unsigned long lastRotTime;
        volatile int pulseCount;
};

#endif /* DRV10970_H_ */
