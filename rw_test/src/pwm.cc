/*
 * pwm.cc
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#include "pwm.h"

#include <cstdint>

PWM::PWM(uint16_t output_pin, uint16_t frequency, uint16_t duty_cycle) {
    _output_pin = output_pin;
    SetFrequency(frequency);
    SetDutyCycle(duty_cycle);

    PWM_PORT_DIR |= PWM_PIN;
    PWM_PORT_SEL0 |= PWM_PIN;
}

void PWM::SetFrequency(uint16_t frequency) {
    _frequency = frequency;
}

void PWM::SetDutyCycle(uint16_t duty_cycle) {
    _period = duty_cycle;
}

void PWM::ClockInit() {

}

void PWM::GPIOInit() {

}

void PWM::TimerInit() {

}
