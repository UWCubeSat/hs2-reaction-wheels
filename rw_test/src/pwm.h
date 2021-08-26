/*
 * pwm.h
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#ifndef PWM_H_
#define PWM_H_

#include <cstdint>

class PWM {
    public:
        /*
        **  Constructs a new PWM object.
        */
        PWM(uint32_t output_pin, uint16_t period, uint16_t duty_cycle);

        /*
        ** Sets the PWM frequency
        */
        void SetFrequency(uint16_t freq);

        /*
        ** Gets the PWM frequency in milliseconds
        */
        uint16_t GetFrequency();

        /*
        ** Sets the PWM period in milliseconds
        */
        void SetPeriod(uint16_t period);

        /*
        ** Gets the PWM period in milliseconds
        */
        uint16_t GetPeriod();

        void SetDutyCycle(uint16_t duty_cycle);

        uint16_t GetDutyCycle();

    private:
        void ClockInit();

        void GPIOInit();

        void TimerInit();

        uint16_t _frequency;
        uint16_t _period;

};  // class PWM

#endif /* SRC_PWM_H_ */
