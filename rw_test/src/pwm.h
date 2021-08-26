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
        /*
        **  Constructs a new PWM object.
        */
        PWM(uint16 output_pin, uint16 frequency, uint16 duty_cycle);

        /*
        ** Initializes PWM object.
        */
        void Init();

        /*
        ** Sets the PWM frequency.
        */
        void SetFrequency(uint16 freq);

        /*
        ** Gets the PWM frequency in milliseconds
        */
        uint16_t GetFrequency();

        /*
        ** Sets the PWM period in milliseconds
        */
        void SetPeriod(uint16 period);

        /*
        ** Gets the PWM period in milliseconds
        */
        uint16_t GetPeriod();

        /*
        ** Sets the PWM period in milliseconds
        */
        void SetDutyCycle(uint8 duty_cycle);


    private:
        void GPIOInit();

        void TimerInit();

        uint16 _frequency;
        uint16 _period;

        uint16 _out_pin_base;

};  // class PWM

#endif /* SRC_PWM_H_ */
