/*
 * bsp.h
 *
 *  Created on: Aug 25, 2021
 *      Author: sailedeer
 */

#ifndef BSP_H_
#define BSP_H_

#include <msp430.h>

/*
** General board pins
*/

// GPIO 1 (inout)
#define GPIO1_PORT_DIR      P1DIR
#define GPIO1_PORT_OUT      P1OUT
#define GPIO1_PORT_SEL0     P1SEL0
#define GPIO1_PORT_SEL1     P1SEL1
#define GPIO1_PORT_PIN      BIT6

// GPIO 2 (inout)
#define GPIO2_PORT_DIR      P3DIR
#define GPIO2_PORT_OUT      P3OUT
#define GPIO2_PORT_SEL0     P3SEL0
#define GPIO2_PORT_SEL1     P3SEL1
#define GPIO2_PORT_PIN      BIT7

// GPIO 3 (inout)
#define GPIO3_PORT_DIR      P3DIR
#define GPIO3_PORT_OUT      P3OUT
#define GPIO3_PORT_SEL0     P3SEL0
#define GPIO3_PORT_SEL1     P3SEL1
#define GPIO3_PORT_PIN      BIT6

// GPIO 4 (inout)
#define GPIO4_PORT_DIR      P3DIR
#define GPIO4_PORT_OUT      P3OUT
#define GPIO4_PORT_SEL0     P3SEL0
#define GPIO4_PORT_SEL1     P3SEL1
#define GPIO4_PORT_PIN      BIT5

// General LED (output)
#define LED_PORT_DIR        P1DIR
#define LED_PORT_OUT        P1OUT
#define LED_PORT_SEL0       P1SEL0
#define LED_PORT_SEL1       P1SEL1
#define LED_PORT_PIN        BIT7


/*
** DRV10970 Pin Definitions
*/

// Variable duty cycle PWM signal (output)
#define PWM_PORT_DIR        P1DIR
#define PWM_PORT_OUT        P1OUT
#define PWM_PORT_SEL0       P1SEL0
#define PWM_PORT_SEL1       P1SEL1
#define PWM_PORT_PIN        BIT0

// Brake Mode Setting signal (output)
#define BRKMOD_PORT_DIR     P3DIR
#define BRKMOD_PORT_OUT     P3OUT
#define BRKMOD_PORT_SEL0    P3SEL0
#define BRKMOD_PORT_SEL1    P3SEL1
#define BRKMOD_PORT_PIN     BIT0

// Frequency Indicator signal (input)
#define FG_PORT_DIR         P1DIR
#define FG_PORT_OUT         P1OUT
#define FG_PORT_SEL0        P1SEL0
#define FG_PORT_SEL1        P1SEL1
#define FG_PORT_PIN         BIT1

// Motor direction control signal (output)
#define FR_PORT_DIR         P1DIR
#define FR_PORT_OUT         P1OUT
#define FR_PORT_SEL0        P1SEL0
#define FR_PORT_SEL1        P1SEL1
#define FR_PORT_PIN         BIT2

// Lock indicator signal (input)
#define RD_PORT_DIR         P3DIR
#define RD_PORT_OUT         P3OUT
#define RD_PORT_SEL0        P3SEL0
#define RD_PORT_SEL1        P3SEL1
#define RD_PORT_PIN         BIT1


/*
**  BNO055 Pin Definitions
*/

// Interrupt signal (input)
#define INT_PORT_DIR     P2DIR
#define INT_PORT_OUT     P2OUT
#define INT_PORT_SEL0    P2SEL0
#define INT_PORT_SEL1    P2SEL1
#define INT_PORT_PIN     BIT2

// Reset signal (output)
#define RST_PORT_DIR     P2DIR
#define RST_PORT_OUT     P2OUT
#define RST_PORT_SEL0    P2SEL0
#define RST_PORT_SEL1    P2SEL1
#define RST_PORT_PIN     BIT2


/*
** PWM Register Definitions
*/

// Timer definitions
#define PWM_PERIOD      TA1CCR0
#define PWM_DUTY_CYCLE  TA1CCR1


/*
** Board-level functions
*/

// initializes board peripherals: I2C Bus, Timers, GPIO, System Clock
void BSP_Init();

// Resets the board and all peripherals
void BSP_Reset();

// returns the current elapsed time since initialization
uint64_t BSP_GetUpTime();

// returns the number of times the board has been reset since power-on
uint16_t BSP_GetResetCount();

#endif /* BSP_H_ */
