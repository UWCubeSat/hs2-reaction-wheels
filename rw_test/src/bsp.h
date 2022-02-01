/*
 * bsp.h
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#ifndef BSP_H_
#define BSP_H_


#include <cstdint>
#include <msp430.h>

#include "msp430fr5994/gpio.h"
#include "msp430fr5994/gpio/pin.h"
#include "msp430fr5994/timer.h"

using namespace MSP430FR5994;

/*
** Board Support Package Interface
*/

namespace BSP {
    void Init();
    void Reset();
    uint32_t GetResetCount();
    void ClearResetCount();
    uint16_t GetResetReason();
    uint64_t MET();

    /*
    ** Convenience defines
    */

    /*
    ** General board pins
    */

    // GPIO 1 (inout)
    static const uint8_t GPIO_1_BASE    = static_cast<uint8_t>(GPIO::PortHandle::P1);
    static const uint8_t GPIO_1_PIN     = BIT6;

    // GPIO 2 (inout)
    static const uint8_t GPIO_2_BASE    = static_cast<uint8_t>(GPIO::PortHandle::P3);
    static const uint8_t GPIO_2_PIN     = BIT7;

    // GPIO 3 (inout)
    static const uint8_t GPIO_3_BASE    = static_cast<uint8_t>(GPIO::PortHandle::P3);
    static const uint8_t GPIO_3_PIN     = BIT6;

    // GPIO 4 (inout)
    static const uint8_t GPIO_4_BASE    = static_cast<uint8_t>(GPIO::PortHandle::P4);
    static const uint8_t GPIO_4_PIN     =  BIT5;

    // General LED (output)
    static const uint8_t LED_BASE       = static_cast<uint8_t>(GPIO::PortHandle::P1);
    static const uint8_t LED_PIN        = BIT7;


    /*
    ** DRV10970 Pin Definitions
    */

    // Variable duty cycle PWM signal (output)
    static const uint8_t PWM_BASE       = static_cast<uint8_t>(GPIO::PortHandle::P1);
    static const uint8_t PWM_PIN        = BIT0;

    // Brake Mode Setting signal (output)
    static const uint8_t BRKMOD_BASE    = static_cast<uint8_t>(GPIO::PortHandle::P3);
    static const uint8_t BRKMOD_PIN     = BIT0;

    // Frequency Indicator signal (input)
    static const uint8_t FG_BASE        = static_cast<uint8_t>(GPIO::PortHandle::P1);
    static const uint8_t FG_PIN         = BIT1;

    // Motor direction control signal (output)
    static const uint8_t FR_BASE        = static_cast<uint8_t>(GPIO::PortHandle::P1);
    static const uint8_t FR_PIN         = BIT2;

    // Lock indicator signal (input)
    static const uint8_t RD_BASE        = static_cast<uint8_t>(GPIO::PortHandle::P3);
    static const uint8_t RD_PIN         = BIT1;


    /*
    **  BNO055 Pin Definitions
    */

    // Interrupt signal (input)
    static const uint8_t INT_BASE       = static_cast<uint8_t>(GPIO::PortHandle::P3);
    static const uint8_t INT_PIN        = BIT2;

    // Reset signal (output)
    static const uint8_t RST_BASE       = static_cast<uint8_t>(GPIO::PortHandle::P4);
    static const uint8_t RST_PIN        = BIT7;


    /*
    ** PWM Register Definitions
    */

    // Timer definitions
    static const uint8_t PWM_TIM_BASE   = static_cast<uint8_t>(TA0_BASE);
    static const uint16_t PWM_TIMER_PERIOD = 533; // timer frequency of ~15 kHz when using SMCLK @ 8 MHz

    /*
    ** External Crystal Pin Definitions
    */

    static const uint8_t LFXT_BASE      = static_cast<uint8_t>(GPIO::PortHandle::PJ);
    static const uint8_t LFXT_PIN       = BIT4;

    static const uint8_t HFXT_BASE      = static_cast<uint8_t>(GPIO::PortHandle::PJ);
    static const uint8_t HFXT_PIN       = BIT6;

    /*
     * Pins broken out
     */

    static GPIO::Pin gp1(GPIO_1_BASE, GPIO_1_PIN);
    static GPIO::Pin gp2(GPIO_2_BASE, GPIO_2_PIN);
    static GPIO::Pin gp3(GPIO_3_BASE, GPIO_3_PIN);
    static GPIO::Pin gp4(GPIO_4_BASE, GPIO_4_PIN);

    static GPIO::Pin ledPin(LED_BASE, LED_PIN);

    static GPIO::Pin pwmPin(PWM_BASE, PWM_PIN);
    static GPIO::Pin brakePin(BRKMOD_BASE, BRKMOD_PIN);
    static GPIO::Pin rpmPin(FG_BASE, FG_PIN);
    static GPIO::Pin dirPin(FR_BASE, FR_PIN);
    static GPIO::Pin lockPin(RD_BASE, RD_PIN);

    static GPIO::Pin intPin(INT_BASE, INT_PIN);
    static GPIO::Pin rstPin(RST_BASE, RST_PIN);

    static GPIO::Pin lfxtPin(LFXT_BASE, LFXT_PIN);
    static GPIO::Pin hfxtPin(HFXT_BASE, HFXT_PIN);

    /*
     * Timer required for PWM output signal
     */

    static Timer::Timer_3 pwmTimer(PWM_TIM_BASE);
}   // namespace BSP

#endif /* BSP_H_ */
