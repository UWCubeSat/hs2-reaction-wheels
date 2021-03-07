/*
 * adc.c
 *
 *  Created on: Mar 6, 2021
 *      Author: Logan
 *      copy of example code to enable adc12
 */
#include <msp430.h>

void main(void) {
    // Configure ADC12
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // Sampling time, S&H=16, ADC12 on
    ADC12CTL1 = ADC12SHP;                   // Use sampling timer
    ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
    ADC12MCTL0 |= ADC12INCH_1;              // A1 ADC input select; Voltage reference=AVCC
    ADC12IER0 |= ADC12IE0;                  // Enable ADC conversion complete interrupt

}



