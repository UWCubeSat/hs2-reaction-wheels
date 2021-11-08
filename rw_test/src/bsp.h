/*
 * bsp.h
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#ifndef BSP_H_
#define BSP_H_

#include "types.h"

#include <cstdint>
#include <msp430.h>

/*
** Convenience defines
*/

/*
** General board pins
*/

// GPIO 1 (inout)
#define GPIO1_PORT_DIR      P1DIR
#define GPIO1_PORT_OUT      P1OUT
#define GPIO1_PORT_SEL0     P1SEL0
#define GPIO1_PORT_SEL1     P1SEL1
#define GPIO1_PIN      BIT6

// GPIO 2 (inout)
#define GPIO2_PORT_DIR      P3DIR
#define GPIO2_PORT_OUT      P3OUT
#define GPIO2_PORT_SEL0     P3SEL0
#define GPIO2_PORT_SEL1     P3SEL1
#define GPIO2_PIN      BIT7

// GPIO 3 (inout)
#define GPIO3_PORT_DIR      P3DIR
#define GPIO3_PORT_OUT      P3OUT
#define GPIO3_PORT_SEL0     P3SEL0
#define GPIO3_PORT_SEL1     P3SEL1
#define GPIO3_PIN      BIT6

// GPIO 4 (inout)
#define GPIO4_PORT_DIR      P3DIR
#define GPIO4_PORT_OUT      P3OUT
#define GPIO4_PORT_SEL0     P3SEL0
#define GPIO4_PORT_SEL1     P3SEL1
#define GPIO4_PIN      BIT5

// General LED (output)
#define LED_PORT_DIR        P1DIR
#define LED_PORT_OUT        P1OUT
#define LED_PORT_SEL0       P1SEL0
#define LED_PORT_SEL1       P1SEL1
#define LED_PIN        BIT7


/*
** DRV10970 Pin Definitions
*/

// Variable duty cycle PWM signal (output)
#define PWM_PORT_DIR        P1DIR
#define PWM_PORT_OUT        P1OUT
#define PWM_PORT_SEL0       P1SEL0
#define PWM_PORT_SEL1       P1SEL1
#define PWM_PIN             BIT0

// Brake Mode Setting signal (output)
#define BRKMOD_PORT_DIR     P3DIR
#define BRKMOD_PORT_OUT     P3OUT
#define BRKMOD_PIN          BIT0

// Frequency Indicator signal (input)
#define FG_PORT_DIR         P1DIR
#define FG_PORT_OUT         P1OUT
#define FG_PORT_SEL0        P1SEL0
#define FG_PORT_SEL1        P1SEL1
#define FG_PORT_IES         P1IES
#define FG_PORT_IE          P1IE
#define FG_PIN              BIT1

// Motor direction control signal (output)
#define FR_PORT_DIR         P1DIR
#define FR_PORT_OUT         P1OUT
#define FR_PORT_IN          P1IN
#define FR_PORT_REN         P1REN
#define FR_PORT_SEL0        P1SEL0
#define FR_PORT_SEL1        P1SEL1
#define FR_PIN              BIT2

// Lock indicator signal (input)
#define RD_PORT_DIR         P3DIR
#define RD_PORT_OUT         P3OUT
#define RD_PORT_IN          P3IN
#define RD_PORT_REN         P3REN
#define RD_PORT_SEL0        P3SEL0
#define RD_PORT_SEL1        P3SEL1
#define RD_PORT_IES         P3IES
#define RD_PORT_IE          P3IE
#define RD_PIN              BIT1


/*
**  BNO055 Pin Definitions
*/

// Interrupt signal (input)
#define INT_PORT_DIR        P2DIR
#define INT_PORT_OUT        P2OUT
#define INT_PORT_SEL0       P2SEL0
#define INT_PORT_SEL1       P2SEL1
#define INT_PORT_IES        P2IES
#define INT_PORT_IE         P2IE
#define INT_PIN             BIT2

// Reset signal (output)
#define RST_PORT_DIR        P2DIR
#define RST_PORT_OUT        P2OUT
#define RST_PORT_SEL0       P2SEL0
#define RST_PORT_SEL1       P2SEL1
#define RST_PIN             BIT2


/*
** PWM Register Definitions
*/

// Timer definitions
#define PWM_TIM_PERIOD_CC       TA0CCR0
#define PWM_TIM_DUTY_CYCLE_CC   TA0CCR1
#define PWM_TIM_CCTL1           TA0CCTL1
#define PWM_TIM_CTL             TA0CTL


/*
** RPM Counter Definitions
*/

// Timer definitions
#define RPM_TIM_PERIOD_CC       TA1CCR0

/*
** I2C Definitions
*/

#define I2C_INT_DIR         P7DIR
#define I2C_INT_SEL0        P7SEL0
#define I2C_INT_SEL1        P7SEL1
#define I2C_INT_SDA_PIN     BIT0
#define I2C_INT_SCL_PIN     BIT1

#define I2C_EXT_DIR         P5DIR
#define I2C_EXT_SEL0        P5SEL0
#define I2C_EXT_SEL1        P5SEL1
#define I2C_EXT_SDA_PIN     BIT0
#define I2C_EXT_SCL_PIN     BIT1

#define I2C_BASE_CLOCK_FREQ 16000000 // 16 MHz

#define OWN_ADDRESS         0x48

/*
** External Crystal Pin Definitions
*/

#define LFXT_SEL0           PJSEL0
#define LFXT_SEL1           PJSEL1
#define LFXT_BIT            BIT4

#define HFXT_SEL0           PJSEL0
#define HFXT_SEL1           PJSEL1
#define HFXT_BIT            BIT6


/*
** Board Support Package Interface
*/

// initializes board peripherals: I2C Bus, Timers, Clocks
void BSP_Init();

// Resets the board and all peripherals
void BSP_Reset();

// returns the number of times the board has been reset since power-on
uint32 BSP_GetResetCount();

// clears reset count
void BSP_ClearResetCount();

// returns reason for last reset
uint16 BSP_GetResetReason();

// returns time since last reset in milliseconds
uint64 BSP_GetMET();

#endif /* BSP_H_ */
