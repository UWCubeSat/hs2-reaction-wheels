//#include <msp430.h>
#include <msp430fr5994.h>
#include <stdint.h>

//scale the input linearly so it fits
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// disable the Watchdog timer for our convenience
	
	P1DIR |= BIT2; //Set pin 1.2 to the output direction
	P1SEL0 |= BIT2; //Select pin 1.2 as our PWM output
	TA0CCR0 = 1000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 us
	TA0CCTL1 = OUTMOD_7; //set output mode to mode 7 "reset-set"
	TA0CCR1 = 500; //The period in microseconds per second that the power is on
	TA0CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, MC_! tells it to count up to the value in TA0CCR0
	__bis_SR_register(LPM0_bits); //Switch to low power mode 0

    // Configure ADC12
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // Sampling time, S&H=16, ADC12 on
    ADC12CTL1 = ADC12SHP;                   // Use sampling timer
    ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
    ADC12MCTL0 |= ADC12INCH_1;              // A1 ADC input select; Voltage reference=AVCC
    ADC12IER0 |= ADC12IE0;                  // Enable ADC conversion complete interrupt
    ADC12MCTL0 |= ADC12INCH_1 | ADC12VRSEL_1; // A1 ADC input select; Voltage reference=1.2V

    uint16_t pwmInput; //write mapping function to clamp between 0 and 1000

    // interrupts the preferred way for mega-learning mode
    // loops (polling) easier
    while(1) {
        if(ADC12IFGR0 & ADC12IFG0) { // go to interrupt flag register, and check if the 0th bit is set
            pwmInput = ADC12MEM0;
            pwmInput = map(pwmInput, 0, 4095, 0, 1000);
            TA0CCR1 = pwmInput;
        }
    }
}
