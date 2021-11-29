/*
 * pwm.cc
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#include "pwm.h"
#include "bsp.h"

#include <cstdint>

PWM::PWM(uint16_t output_pin, uint16_t period, uint16_t duty_cycle) {
    GPIOInit();
    TimerInit();
    SetPeriod(period);
    SetDutyCycle(duty_cycle);
}

void PWM::SetPeriod(uint16_t period) {
    PWM_TIM_PERIOD_CC = period;
}

void PWM::SetDutyCycle(uint16_t duty_cycle) {
    PWM_TIM_DUTY_CYCLE_CC = duty_cycle;
}

void PWM::GPIOInit() {
    // enable PWM pin for TA0.1 = output
    PWM_PORT_DIR |= PWM_PIN;
    PWM_PORT_SEL0 |= PWM_PIN;
    PWM_PORT_SEL1 &= ~(PWM_PIN);
}

void PWM::TimerInit() {
    // timer frequency: 32 KHz / 2 = 16 KHz
    // time for one clock cycle: 0.0625 ms
    PWM_TIM_CCTL1 = OUTMOD_7;  // reset/set output mode
    PWM_TIM_CTL  = TASSEL_2 + ID_1 + MC_1;  // ACLK / 2, up mode, clear, no interrupts
}
