#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

//scale the input linearly so it fits
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


volatile uint16_t pwmInput = 0;

void ClockInit() {
    // configure clock sources
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO = 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;// Set ACLK=VLO SMCLK=DCO
    CSCTL3 = DIVA__8 | DIVS__8 | DIVM__8;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers
}

void GPIOInit() {
    // Configure GPIO
    P1OUT &= ~BIT0;                         // Clear LED to start
    P1DIR |= BIT0 | BIT2;                   // Set P1.0/P1.2 to output
    P1SEL0 |= BIT2 | BIT3;                  // Select pin 1.2 as our PWM output
    P1SEL1 |= BIT3;                         // config pin 1.3 as analog input
}

void TimerInit() {
    // configure Timer0_A
    TA1CCR0 = 1000 - 1;                     // Set period to 1 ms
    TA1CCTL1 = OUTMOD_7;                    // set output mode to mode 7 "reset-set"
    TA1CCR1 = 0;                            // Duty cycle of 0%
    TA1CTL = TASSEL__SMCLK | MC__UP | TACLR;// SMCLK, up mode, clear TAR
}

void ADCInit() {
    // Initialize ADC12_A
    ADC12CTL0 &= ~ADC12ENC;                  // Disable ADC12
    ADC12CTL0 |= ADC12SHT0_8 | ADC12ON;      // Set sample time
    ADC12CTL1 |= ADC12SHP;                   // Enable sample timer
    ADC12CTL3 |= ADC12TCMAP;                 // Enable internal temperature sensor
    ADC12MCTL0 |= ADC12INCH_3;               // ADC Input P1.3, Vref = AVCC
    ADC12IER0 |= 0x001;                      // ADC_IFG upon conv result-ADCMEMO

    while(!(REFCTL0 & REFGENRDY));          // Wait for reference generator
                                            // to settle
    ADC12CTL0 |= ADC12ENC;
}

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // disable the Watchdog timer for our convenience

    // Initialize the shared reference module
    // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
    while(REFCTL0 & REFGENBUSY);            // If ref generator busy, WAIT
    REFCTL0 |= REFVSEL_0 + REFON;           // Enable internal 1.2V reference

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    ClockInit();

    GPIOInit();

    TimerInit();

    ADCInit();

    unsigned int new_duty_cycle = 0;
    // hang in LPM0 until IE
    // then update our period
    while(1) {
        ADC12CTL0 |= ADC12SC;               // Sampling and conversion start
        __bis_SR_register(LPM0_bits | GIE);
        new_duty_cycle = map(pwmInput, 0, 4095, 0, 1000);
        TA1CCR1 = new_duty_cycle;
        __no_operation();
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
            if (pwmInput >= 0x0AAB)
                P1OUT |= BIT0;              // P1.0 = 1
            else
                P1OUT &= ~BIT0;             // P1.0 = 0
            __bic_SR_register_on_exit(LPM0_bits | GIE);
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
