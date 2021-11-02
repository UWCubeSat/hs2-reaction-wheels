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

uint16_t reset_reason;

static void PeriphInit() {
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
    FG_PORT_IES &= ~(FG_PIN);
    FG_PORT_IE |= FG_PIN;

    // motor direction pin = output
    FR_PORT_DIR |= FR_PIN;

    // lock indicator = input
    RD_PORT_DIR &= ~(RD_PIN);
    RD_PORT_IES &= ~(RD_PIN);
    RD_PORT_IE |= RD_PIN;


    // BNO055 pins

    // interrupt pin = input
    INT_PORT_DIR &= ~(INT_PIN);
    INT_PORT_IES &= ~(INT_PIN);
    INT_PORT_IE |= INT_PIN;

    // reset pin = output
    RST_PORT_DIR |= RST_PIN;


    // Extra GPIOs

    // GPIO 1 = output
    GPIO1_PORT_DIR |= GPIO1_PIN;

    // GPIO 2 = output
    GPIO2_PORT_DIR |= GPIO2_PIN;

    // GPIO 3 = output
    GPIO3_PORT_DIR |= GPIO3_PIN;

    // GPIO 4 = output
    GPIO4_PORT_DIR |= GPIO4_PIN;

    // Status LED pin = output
    LED_PORT_DIR |= LED_PIN;

}

static void I2CInit() {
    // I2C 1 (External, secondary)
    UCB1CTLW0 |= UCSWRST;                       // hold in reset
    UCB1CTLW0 |= (UCMODE_3 | UCSYNC | UCSSEL__SMCLK);           // secondary on bus
    UCB1I2COA0 = (OWN_ADDRESS | UCOAEN);         // own (secondary) address
    UCB1BRW = 160;                              // SMCLK / 160 (100 kHz)
    UCB1CTLW1 = (UCASTP_2 | UCCLTO_3);          // auto stop assertion + timeout

    // configure pins
    I2C_EXT_SEL0 |= (I2C_EXT_SDA_PIN | I2C_EXT_SCL_PIN);
    I2C_EXT_SEL1 &= ~(I2C_EXT_SDA_PIN | I2C_EXT_SCL_PIN);

    // enable interrupts
    UCB2IE &= ~(UCRXIE0 | UCTXIE0);

    // enable bus
    UCB2CTLW0 &= ~(UCSWRST);

    // I2C 2 (Internal, primary)
    UCB2CTLW0 |= UCSWRST;                       // hold in reset
    UCB2CTLW0 |= (UCMODE_3 | UCMST | UCSYNC | UCSSEL__SMCLK);   // primary on bus
    UCB2BRW = 40;                               // SMCLK / 40 (400 kHz)
    UCB2CTLW1 = (UCASTP_2 | UCCLTO_3);          // auto stop assertion + timeout

    // configure pins
    I2C_INT_SEL0 |= (I2C_INT_SDA_PIN | I2C_INT_SCL_PIN);
    I2C_INT_SEL1 &= ~(I2C_INT_SDA_PIN | I2C_INT_SCL_PIN);

    // enable interrupts
    UCB2IE &= ~(UCRXIE0 | UCTXIE0);

    // enable bus
    UCB2CTLW0 &= ~(UCSWRST);

    return;
}

static void TimerInit() {
    // initialize PWM timer
    PWM_TIM_PERIOD_CC = 1000 - 1;   // PWM frequency starts at 16 kHz (15 kHz to 100 kHz for DRV10970)
    PWM_TIM_CCTL1 = OUTMOD_7;       // reset-set
    PWM_TIM_DUTY_CYCLE_CC = 0;      // duty-cycle of 0%
    PWM_TIM_CTL = TASSEL__SMCLK | MC__UP | TACLR;  // SMCLK, up mode, clear TAR

    // initialize RPM timer
    // TODO: RPM Timer - extrapolate RPM from ticks received in some period
    return;
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

    // let DCO settle (not sure why 60, blame the HS1 engineers)
    __delay_cycles(60);
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // set dividers for 8 MHz/16 MHz/16 MHz operation

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
    RTCPS0CTL = RT0PSDIV__8;                    // ACLK, /8
    RTCPS1CTL = RT1SSEL1 | RT1PSDIV__16;        // out from RT0PS, /16; increment ~= 4 ms

    // clear count registers
    RTCCNT1 = 0;
    RTCCNT2 = 0;
    RTCCNT3 = 0;
    RTCCNT4 = 0;

    RTCCTL13 &= ~(RTCHOLD);                 // Start RTC
    RTCCTL0_H = 0;                          // lock RTC
    return;
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
    PeriphInit();

    // initialize serial resources
    I2CInit();

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
    return;
}

void BSP_Reset() {

}

uint32 BSP_GetResetCount() {
    return local_reset_count;
}

uint16 BSP_GetResetReason() {
    return reset_reason;
}

void BSP_ClearResetCount() {
    local_reset_count = 0;
    return;
}

// TODO: I have no idea if this works smh my head
uint64 BSP_GetMET() {
    return ((uint64)RTCCNT1) | ((uint64)RTCCNT2 << 8) | ((uint64)RTCCNT3 << 16) | ((uint64)RTCCNT4 << 24);
}

//I2CResult BSP_I2C_BeginTransmission(I2CBus bus, uint8 addr) {
//    return I2C_NO_ERROR;
//}
//
//I2CResult BSP_I2C_Write(I2CBus bus, uint8 byte) {
//    return I2C_NO_ERROR;
//}
//
//I2CResult BSP_I2C_Read(I2CBus bus, uint8 *byte) {
//
//}
//
//I2CResult BSP_I2C_EndTransmission(I2CBus bus);
//
//static I2CResult BSP_I2C_TransmitAndReceive(I2CBus bus, uint8 addr, uint8 * w_buf, uint8 w_bytes, uint8 * r_buf, uint8 r_bytes) {
//    if (BSP_I2C_BusBusy(bus)) return I2C_BUS_BUSY;
//
//    BSP_I2C_Disable(bus);
//
//    // ensures that STOP condition is sent when communication is finished
//    BSP_I2C_SetAutoStopByteCount(bus, r_bytes + w_bytes);
//
//    BSP_I2C_Enable(bus);
//
//    // prepare our
//}
//
//// check if a transaction is ongoing
//static uint8 BSP_I2C_BusBusy(I2CBus bus) {
//    if (bus == I2C_EXTERNAL_BUS) {
//        return (UCB1STATW & UCBBUSY);
//    } else if (bus == I2C_INTERNAL_BUS) {
//        return (UCB2STATW & UCBBUSY);
//    }
//}
//
//// enable the bus
//static void BSP_I2C_Enable(I2CBus bus) {
//    if (bus == I2C_EXTERNAL_BUS) {
//        UCB1CTLW0 &= ~UCSWRST;
//    } else if (bus == I2C_INTERNAL_BUS) {
//        UCB2CTLW0 &= ~UCSWRST;
//    }
//}
//
//// disable the bus
//static void BSP_I2C_Disable(I2CBus bus) {
//    if (bus == I2C_EXTERNAL_BUS) {
//        UCB1CTLW0 |= UCSWRST;
//    } else if (bus == I2C_INTERNAL_BUS) {
//        UCB2CTLW0 |= UCSWRST;
//    }
//}
//
//// external (secondary) I2C interrupt service routine
//#pragma vector = USCI_B1_VECTOR
//__interrupt void USCI_B1_ISR(void) {
//    switch(__even_in_range(UCB1IV, UCIV__UCBIT9IFG)) {
//        case UCIV__NONE: // Vector 0: No interrupts
//            break;
//        case UCIV__UCALIFG: // Vector 2: ALIFG
//            break;
//        case UCIV__UCNACKIFG: // Vector 4: NACKIFG
//            break;
//        case UCIV__UCSTTIFG: // Vector 6: STTIFG
//            break;
//        case UCIV__UCSTPIFG: // Vector 8: STPIFG
//            break;
//        case UCIV__UCRXIFG3: // Vector 10: RXIFG3
//            break;
//        case UCIV__UCTXIFG3: // Vector 12: TXIFG3
//            break;
//        case UCIV__UCRXIFG2: // Vector 14: RXIFG2
//            break;
//        case UCIV__UCTXIFG2: // Vector 16: TXIFG2
//            break;
//        case UCIV__UCRXIFG1: // Vector 18: RXIFG1
//            break;
//        case UCIV__UCTXIFG1: // Vector 20: TXIFG1
//            break;
//        case UCIV__UCRXIFG0: // Vector 22: RXIFG0
//            // shift byte out of register into our target buffer
//            break;
//        case UCIV__UCTXIFG0: // Vector 24: TXIFG0
//            break;
//        case UCIV__UCBCNTIFG: // Vector 26: BCNTIFG
//            break;
//        case UCIV__UCCLTOIFG: // Vector 28: clock low time-out
//            break;
//        case UCIV__UCBIT9IFG: // Vector 30: 9th bit
//            break;
//        default: break;
//    }
//}
//
//// internal (primary) I2C interrupt service routine
//#pragma vector = USCI_B2_VECTOR
//__interrupt void USCI_B2_ISR(void) {
//    switch(__even_in_range(UCB2IV, UCIV__UCBIT9IFG)) {
//        case UCIV__NONE: // Vector 0: No interrupts
//            break;
//        case UCIV__UCALIFG: // Vector 2: ALIFG
//            break;
//        case UCIV__UCNACKIFG: // Vector 4: NACKIFG
//            break;
//        case UCIV__UCSTTIFG: // Vector 6: STTIFG
//            break;
//        case UCIV__UCSTPIFG: // Vector 8: STPIFG
//            break;
//        case UCIV__UCRXIFG3: // Vector 10: RXIFG3
//            break;
//        case UCIV__UCTXIFG3: // Vector 12: TXIFG3
//            break;
//        case UCIV__UCRXIFG2: // Vector 14: RXIFG2
//            break;
//        case UCIV__UCTXIFG2: // Vector 16: TXIFG2
//            break;
//        case UCIV__UCRXIFG1: // Vector 18: RXIFG1
//            break;
//        case UCIV__UCTXIFG1: // Vector 20: TXIFG1
//            break;
//        case UCIV__UCRXIFG0: // Vector 22: RXIFG0
//            // shift bits out of read register
//            break;
//        case UCIV__UCTXIFG0: // Vector 24: TXIFG0
//            //
//            break;
//        case UCIV__UCBCNTIFG: // Vector 26: BCNTIFG
//            break;
//        case UCIV__UCCLTOIFG: // Vector 28: clock low time-out
//            break;
//        case UCIV__UCBIT9IFG: // Vector 30: 9th bit
//            break;
//        default: break;
//    }
//}
