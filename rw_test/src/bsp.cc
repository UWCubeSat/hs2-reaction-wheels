/*
 * bsp.cc
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#include "bsp.h"

#include <msp430.h>

/*
 * store reset counts in flash so that we can retrieve them between
 * reset events
 */
#pragma PERSISTENT
uint32 local_reset_count = 0;

// update each time we call BSP_Init()
static uint16 reset_reason;

static void GPIOInit() {
    // Force all outputs to be 0, so we don't get spurious signals when we unlock
    P1OUT = 0;
    P2OUT = 0;
    P3OUT = 0;
    P4OUT = 0;
    P5OUT = 0;
    P6OUT = 0;
    P7OUT = 0;
    P8OUT = 0;
    PJOUT = 0;

    // DRV10970 pins

    // enable PWM pin for TA0.1 = output
    PWM_PORT_DIR |= PWM_PIN;
    PWM_PORT_SEL0 |= PWM_PIN;
    PWM_PORT_SEL1 &= ~(PWM_PIN);

    // break mode pin = output
    BRKMOD_PORT_DIR |= BRKMOD_PIN;

    // freq indicator pin = input
    FG_PORT_DIR &= ~(FG_PIN);
    // TODO: set up an interrupt here so we can capture the current RPM


}

static void I2CInit() {

}

static void TimerInit() {
    // initialize PWM timer
}

static void ClockInit() {
    // configure HF clock source pins
    HFXT_SEL0 &= ~(HFXT_BIT);
    HFXT_SEL1 &= ~(HFXT_BIT);
    HFXT_SEL0 |= HFXT_BIT;

    // configure LF clock source pins
    LFXT_SEL0 &= ~(LFXT_BIT);
    LFXT_SEL1 &= ~(LFXT_BIT);
    LFXT_SEL0 |= LFXT_BIT;

    // configure clock sources
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers

    // our external HF clock is 16 MHz, set HFFREQ accordingly
    CSCTL4 |= HFFREQ_2;     // > 8 MHz to 16 MHz

    // ensure bypass is off
    CSCTL4 &= ~(LFXTBYPASS | HFXTBYPASS);

    CSCTL1 = DCOFSEL_0;                     // Set DCO = 1MHz
    CSCTL2 = SELA__VLOCLK | SELS__HFXTCLK | SELM__HFXTCLK; // Set ACLK=VLO SMCLK=MCLK=HFXT
    CSCTL3 = DIVA__4 | DIVS__4 | DIVM__4;   // Set all dividers
    CSCTL1 = DCOFSEL_6;                     // set DCO = 8MHz

    // let DCO settle
    __delay_cycles(60);
    CSCTL3 = DIVA__1 | DIVS__16 | DIVM__1;   // set all dividers to 1 for 16 MHz/8 MHz/1 MHz operation

    // turn both external oscillator sources on
    CSCTL4 &= ~(LFXTOFF | HFXTOFF);

    // ensure we don't have any faults
    do {
        CSCTL5 &= ~(LFXTOFFG | HFXTOFFG);   // clear HFXT/LFXT fault flags
        SFRIFG1 &= ~OFIFG;                  // clear fault flag
    } while (SFRIFG1 & OFIFG);              // test fault flag

    CSCTL0_H = 0;                           // Lock CS registers
    return;
}

static void RTCInit() {
    RTCCTL0_H = RTCKEY_H;                       // Unlock RTC

    RTCCTL0_L = RTCTEVIE_L;                     // RTC event interrupt enable
    RTCCTL1 = RTCSSEL_2 | RTCTEV_3 | RTCHOLD;   // Counter Mode, RTC1PS, 8-bit ovf, 32-bit interrupt
    RTCPS0CTL = RT0PSDIV1 | RT1PSDIV1;          // ACLK, /8
    RTCPS1CTL = RT1SSEL1 | RT1PSDIV__16;        // out from RT0PS, /16; increment ~= 4 ms

    RTCCNT1 = 0;
    RTCCNT2 = 0;
    RTCCNT3 = 0;
    RTCCNT4 = 0;

    RTCCTL13 &= ~(RTCHOLD);                 // Start RTC
}


void BSP_Init() {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    local_reset_count++;

    reset_reason = SYSRSTIV;

    // initialize clock and timer resources
    ClockInit();
    RTCInit();
    TimerInit();

    // initialize digital I/O resources
    GPIOInit();

    // initialize serial resources
    I2CInit();

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}

uint32 BSP_GetResetCount() {
    return local_reset_count;
}

uint16 BSP_GetResetReason() {
    return reset_reason;
}

void BSP_ClearResetCount() {
    local_reset_count = 0;
}

uint64 BSP_GetElapsedTime() {
    // TODO: set up RTC so that we can get a MET
    return 0;
}
