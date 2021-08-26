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
 * PUC events
 */
#pragma PERSISTENT(local_reset_count)
static uint32 local_reset_count;

// update each time we call BSP_Init()
static uint16 reset_reason;

static void I2CInit() {

}

static void TimerInit() {

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
    CSCTL2 = SELA__LFXTCLK | SELS__HFXTCLK | SELM__HFXTCLK; // Set ACLK=LFXT SMCLK=MCLK=HFXT
    CSCTL3 = DIVA__4 | DIVS__4 | DIVM__4;   // Set all dividers
    CSCTL1 = DCOFSEL_6;                     // set DCO = 8MHz

    // let DCO settle -- necessary since we're using HFXT/LFXT?
    __delay_cycles(60);
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // set all dividers to 1 for 16 MHz operation

    // turn both external oscillator sources on
    CSCTL4 &= ~(LFXTOFF | HFXTOFF);

    do {
        CSCTL5 &= ~(LFXTOFFG | HFXTOFFG);   // clear HFXt/LFXT fault flags
        SFRIFG1 &= ~OFIFG;                  // clear fault flag
    } while (SFRIFG1 & OFIFG);              // test fault flag

    CSCTL0_H = 0;                           // Lock CS registers
    return;
}


void BSP_Init() {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    local_reset_count++;

    reset_reason = SYSRSTIV;

    ClockInit();
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
