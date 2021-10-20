/*
 * pwm.h
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#ifndef PWM_H_
#define PWM_H_

#include "types.h"

class PWM {
    public:
        // Constructs a new PWM object
        PWM(uint16_t output_pin, uint16_t frequency, uint16_t duty_cycle);

        // Delete copy constructor
        PWM(const PWM &) = delete;

        // PWM object destructor
        ~PWM();

        // Sets the PWM frequency
        void SetFrequency(uint16_t frequency);

        // Gets the PWM frequency
        uint16_t GetFrequqncy() { return _frequency; }

        // Sets the PWM period in milliseconds
        void SetDutyCycle(uint8 duty_cycle);

        // Gets the PWM period in milliseconds
        uint16_t GetDutyCycle() { return _period; }


    private:
        void ClockInit();
        void GPIOInit();
        void TimerInit();

        uint16_t _output_pin;
        uint16_t _frequency;
        uint16_t _period;

};  // class PWM

#endif /* SRC_PWM_H_ */
