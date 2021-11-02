/*
 * main.cc
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#include "main.h"
#include "bsp.h"
#include "pwm.h"

#include <cstdint>

int main() {
//    BSP_Init();
//    while (1) {
//
//    }
    
    BSP_Init();
    //PWM pwm(PWM_PIN, 1000, 900);
    PWM_TIM_DUTY_CYCLE_CC = 500;
    LED_PORT_OUT |= LED_PIN;
}
