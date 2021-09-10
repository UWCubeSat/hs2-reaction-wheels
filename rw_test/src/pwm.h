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
        ** Sets the PWM frequency.
        */
        void SetFrequency(uint16 freq);

        /*
        ** Sets the PWM period in milliseconds
        */
        static void SetDutyCycle(uint8 duty_cycle);


    private:
        void GPIOInit();

        void TimerInit();

        static uint16 _frequency;
        static uint16 _period;

};  // class PWM

#endif /* SRC_PWM_H_ */
