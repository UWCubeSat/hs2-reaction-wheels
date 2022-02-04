/*
 * bsp.cc
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#include <msp430.h>

#include "bsp.h"
#include "msp430fr5994/gpio/pin.h"
#include "msp430fr5994/gpio/port.h"

/*
 * store reset counts in flash so that we can retrieve them between
 * reset events
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma PERSISTENT
#elif defined(__GNUC__)
__attribute__ ((persistent))
#else
#error Compiler not supported!
#endif
uint32_t local_reset_count = 0;

uint16_t reset_reason;

using namespace MSP430FR5994;
using namespace BSP;

static void PeriphInit() {
    // Force all outputs to be 0, so we don't get spurious signals when we unlock
    GPIO::p1.out.set(0);
    GPIO::p2.out.set(0);
    GPIO::p3.out.set(0);
    GPIO::p4.out.set(0);
    GPIO::p5.out.set(0);
    GPIO::p6.out.set(0);
    GPIO::p7.out.set(0);
    GPIO::p8.out.set(0);

    // BNO055 pins

    // interrupt pin = input
    intPin.SetMode(GPIO::Direction::INPUT);
    intPin.EnableInterrupt(GPIO::InterruptSource::RISING, nullptr);

    // reset pin = output
    rstPin.SetMode(GPIO::Direction::OUTPUT);

    // Extra GPIOs

    // GPIO 1 = output
    gp1.SetMode(GPIO::Direction::OUTPUT);

    // GPIO 2 = output
    gp2.SetMode(GPIO::Direction::OUTPUT);

    // GPIO 3 = output
    gp3.SetMode(GPIO::Direction::OUTPUT);

    // GPIO 4 = output
    gp4.SetMode(GPIO::Direction::OUTPUT);

    // Status LED pin = output
    ledPin.SetMode(GPIO::Direction::OUTPUT);

}

static void ClockInit() {
    // TODO: figure out what's going on with HFXT
    // configure HF clock source pins
//    hfxtPin.SetFunction(GPIO::Function::PRIMARY);

    // configure LF clock source pins
    lfxtPin.SetFunction(GPIO::Function::PRIMARY);

    // configure clock sources
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers

    // our external HF clock is 16 MHz, set HFFREQ accordingly
//    CSCTL4 |= HFFREQ_2;     // > 8 MHz to 16 MHz

    // ensure bypass is off
//    CSCTL4 &= ~(LFXTBYPASS | HFXTBYPASS);
    CSCTL4 &= ~(LFXTBYPASS);

    CSCTL1 = DCOFSEL_0;                     // Set DCO = 1MHz
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK; // Set ACLK=LFXT SMCLK=MCLK=HFXT
    CSCTL3 = DIVA__4 | DIVS__4 | DIVM__4;   // Set all dividers
    CSCTL1 = DCOFSEL_6;                     // set DCO = 8MHz

    // let DCO settle (not sure why 60, blame the HS1 engineers)
    __delay_cycles(60);
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // set dividers for 32 kHz/8 MHz/8 MHz operation

    // turn both external oscillator sources on
//    CSCTL4 &= ~(LFXTOFF | HFXTOFF);
    CSCTL4 &= ~(LFXTOFF);

    // ensure we don't have any faults
    do {
//        CSCTL5 &= ~(LFXTOFFG | HFXTOFFG);   // clear HFXT/LFXT fault flags
        CSCTL5 &= ~(LFXTOFFG);
        SFRIFG1 &= ~OFIFG;                  // clear fault flag
    } while (SFRIFG1 & OFIFG);              // test fault flag

    CSCTL0_H = 0;                           // Lock CS registers
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
}

void BSP::Init() {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    local_reset_count++;

    reset_reason = SYSRSTIV;

    // initialize clock and timer resources
    ClockInit();
    RTCInit();

    // initialize digital I/O resources
    PeriphInit();

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}

void BSP::Reset() {

}

uint32_t BSP::GetResetCount() {
    return local_reset_count;
}

uint16_t BSP::GetResetReason() {
    return reset_reason;
}

void BSP::ClearResetCount() {
    local_reset_count = 0;
}

// TODO: I have no idea if this works smh my head
uint64_t BSP::MET() {
    return ((uint64_t)RTCCNT1) | ((uint64_t)RTCCNT2 << 8) | ((uint64_t)RTCCNT3 << 16) | ((uint64_t)RTCCNT4 << 24);
}
