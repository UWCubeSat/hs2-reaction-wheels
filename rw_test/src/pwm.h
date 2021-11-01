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
        // Requirement: period >= duty_cycle
        PWM(uint16_t output_pin, uint16_t period, uint16_t duty_cycle);

        // Delete copy constructor
        PWM(const PWM &) = delete;

        // PWM object destructor
        ~PWM();

        // Sets the PWM frequency
        void SetPeriod(uint16_t period);

        // Gets the PWM frequency
        uint16_t GetPeriod() { return PWM_TIM_PERIOD_CC; }

        // Sets the PWM period in milliseconds
        void SetDutyCycle(uint8 duty_cycle);

        // Gets the PWM period in milliseconds
        uint16_t GetDutyCycle() { return PWM_TIM_DUTY_CYCLE_CC; }


    private:
        void GPIOInit();
        void TimerInit();

};  // class PWM

#endif /* SRC_PWM_H_ */
