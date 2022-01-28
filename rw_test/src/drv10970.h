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

using namespace MSP430FR5994;

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


class DRV10970 {
    public:

        enum Direction {
            FORWARD = 0,
            BACKWARD = 1,
        };
        // Constructor
        DRV10970(GPIO::Pin&, GPIO::Pin&, GPIO::Pin&, GPIO::Pin&, GPIO::Pin&);

        // Delete copy constructor
        DRV10970(const DRV10970 &) = delete;

        // Destructor
        ~DRV10970();

        // turns on brake
        inline void EnableBrake();

        // turns off brake
        inline void DisableBrake();

        // toggle brake
        inline void ToggleBrake();

        // get brake status
        inline uint16_t GetBrake() { return BRKMOD_PORT_OUT & BRKMOD_PIN; }

        // sets motor to forward
        inline void SetDirectionForward();

        // sets motor to backward
        inline void SetDirectionBackward();

        // toggle's motor direction
        inline void ToggleDirection();

        // get current motor direction
        inline uint8_t GetDirection() { return FR_PORT_OUT & FR_PIN; }

        // get the current lock status
        inline uint8_t GetLockStatus() { return ~(RD_PORT_IN & RD_PIN); }

        // get how many times a lock event has occurred
        inline uint8_t GetLockEventCount() { return _lock_events; }

        // update PWM frequency
        inline void SetPWMFrequency(uint16_t frequency);

        // update PWM duty cycle
        inline void SetPWMDutyCycle(uint8_t duty_cycle);

        // get the RPM of the motor
        inline float GetRPM() { return _rpm; }

    private:
        const uint8_t POLES = 2;
        const float MIL_TO_MIN = 60000;
        const uint8_t ROTS_TO_COUNT = 3;
        Direction _dir;

        uint32_t _lock_events;  // number of detected lock events
        uint32_t _error_count;

        GPIO::Pin &_brakePin;
        GPIO::Pin &_dirPin;
        GPIO::Pin &_rpmPin;
        GPIO::Pin &_lockPin;
        GPIO::Pin &_pwmPin;

        float _rpm;
        unsigned long _lastRotTime;
        volatile int _pulseCount;


        // optional
        void (*_lockIndicatorCallback)();
};

#endif /* DRV10970_H_ */
