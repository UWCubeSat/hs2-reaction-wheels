//#include <msp430.h>
#include <msp430fr5994.h>
#include <stdint.h>
#include <stdio.h>

//scale the input linearly so it fits
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


uint16_t pwmInput; //write mapping function to clamp between 0 and 1000

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // disable the Watchdog timer for our convenience

    P1DIR |= BIT2; //Set pin 1.2 to the output direction
    P1SEL0 |= BIT2 | BIT3; //Select pin 1.2 as our PWM output
    P1SEL1 |= BIT3; // config pin 1.3 to adc input
    TA0CCR0 = 1000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 us
    TA0CCTL1 = OUTMOD_7; //set output mode to mode 7 "reset-set"
    TA0CCR1 = 500; //The period in microseconds per second that the power is on
    TA0CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, MC_! tells it to count up to the value in TA0CCR0
    __bis_SR_register(LPM0_bits); //Switch to low power mode 0

    // Configure ADC12
    ADC12CTL0 &= ~ADC12ENC;                 // Disable ADC12
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // Sampling time, S&H=16, ADC12 on
    ADC12CTL1 = ADC12SHP;                   // Use sampling timer
    ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
    ADC12IER0 |= ADC12IE0;                  // Enable ADC conversion complete interrupt
    ADC12MCTL0 |= ADC12INCH_3 | ADC12VRSEL_3; // A1 ADC input select; Voltage reference=1.2V
    ADC12CTL0 |= ADC12ENC;

    // interrupts the preferred way for mega-learning mode
    // loops (polling) easier
    while(1) {
        ADC12CTL0 |= ADC12SC;               // Sampling and conversion start
        TA0CCR1 = map(pwmInput, 0, 4095, 0, 1000);
        printf("%d\n", pwmInput);
        __no_operation();                   // SET BREAKPOINT HERE
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_B_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_B_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADC12IV, ADC12IV__ADC12RDYIFG))
    {
        case ADC12IV__NONE:        break;   // Vector  0:  No interrupt
        case ADC12IV__ADC12OVIFG:  break;   // Vector  2:  ADC12MEMx Overflow
        case ADC12IV__ADC12TOVIFG: break;   // Vector  4:  Conversion time overflow
        case ADC12IV__ADC12HIIFG:  break;   // Vector  6:  ADC12BHI
        case ADC12IV__ADC12LOIFG:  break;   // Vector  8:  ADC12BLO
        case ADC12IV__ADC12INIFG:  break;   // Vector 10:  ADC12BIN
        case ADC12IV__ADC12IFG0:            // Vector 12:  ADC12MEM0 Interrupt
            pwmInput = ADC12MEM0;             // read out the result register
            __no_operation();                   // SET BREAKPOINT HERE
            __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
            break;                          // Clear CPUOFF bit from 0(SR)
        case ADC12IV__ADC12IFG1:   break;   // Vector 14:  ADC12MEM1
        case ADC12IV__ADC12IFG2:   break;   // Vector 16:  ADC12MEM2
        case ADC12IV__ADC12IFG3:   break;   // Vector 18:  ADC12MEM3
        case ADC12IV__ADC12IFG4:   break;   // Vector 20:  ADC12MEM4
        case ADC12IV__ADC12IFG5:   break;   // Vector 22:  ADC12MEM5
        case ADC12IV__ADC12IFG6:   break;   // Vector 24:  ADC12MEM6
        case ADC12IV__ADC12IFG7:   break;   // Vector 26:  ADC12MEM7
        case ADC12IV__ADC12IFG8:   break;   // Vector 28:  ADC12MEM8
        case ADC12IV__ADC12IFG9:   break;   // Vector 30:  ADC12MEM9
        case ADC12IV__ADC12IFG10:  break;   // Vector 32:  ADC12MEM10
        case ADC12IV__ADC12IFG11:  break;   // Vector 34:  ADC12MEM11
        case ADC12IV__ADC12IFG12:  break;   // Vector 36:  ADC12MEM12
        case ADC12IV__ADC12IFG13:  break;   // Vector 38:  ADC12MEM13
        case ADC12IV__ADC12IFG14:  break;   // Vector 40:  ADC12MEM14
        case ADC12IV__ADC12IFG15:  break;   // Vector 42:  ADC12MEM15
        case ADC12IV__ADC12IFG16:  break;   // Vector 44:  ADC12MEM16
        case ADC12IV__ADC12IFG17:  break;   // Vector 46:  ADC12MEM17
        case ADC12IV__ADC12IFG18:  break;   // Vector 48:  ADC12MEM18
        case ADC12IV__ADC12IFG19:  break;   // Vector 50:  ADC12MEM19
        case ADC12IV__ADC12IFG20:  break;   // Vector 52:  ADC12MEM20
        case ADC12IV__ADC12IFG21:  break;   // Vector 54:  ADC12MEM21
        case ADC12IV__ADC12IFG22:  break;   // Vector 56:  ADC12MEM22
        case ADC12IV__ADC12IFG23:  break;   // Vector 58:  ADC12MEM23
        case ADC12IV__ADC12IFG24:  break;   // Vector 60:  ADC12MEM24
        case ADC12IV__ADC12IFG25:  break;   // Vector 62:  ADC12MEM25
        case ADC12IV__ADC12IFG26:  break;   // Vector 64:  ADC12MEM26
        case ADC12IV__ADC12IFG27:  break;   // Vector 66:  ADC12MEM27
        case ADC12IV__ADC12IFG28:  break;   // Vector 68:  ADC12MEM28
        case ADC12IV__ADC12IFG29:  break;   // Vector 70:  ADC12MEM29
        case ADC12IV__ADC12IFG30:  break;   // Vector 72:  ADC12MEM30
        case ADC12IV__ADC12IFG31:  break;   // Vector 74:  ADC12MEM31
        case ADC12IV__ADC12RDYIFG: break;   // Vector 76:  ADC12RDY
        default: break;
    }
}
