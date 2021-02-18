#include <msp430.h> 


int getDesiredValue(void) {
    int num;
    //Get value from potentiometer
    //set num = potentiometer reading
    return num;
}


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// disable the Watchdog timer for our convenience
	
	P1DIR |= BIT2; //Select pin 1.2 as our PWM output
	P1SEL |= BIT2 //Select pin 1.2 as our PWM output
	TA0CCR0 = 1000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 us
	TA0CCTL1 = OUTMOD_7;
	TA0CCR1 = 500; //The period in microseconds per second that the power is on /////Change this
	//while loop
	    //int desired value = getDesiredValue
	    //TA0CCR1 = desired value
	TA0CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, MC_! tells it to count up to the value in TA0CCR0
	__bis_SR_register(LPM0_bits); //Switch to low power mode 0
}
