/*
 * rw_test.h
 *
 *  Created on: May 19, 2021
 *      Author: sailedeer
 */

#ifndef RW_TEST_H_
#define RW_TEST_H_

// RW Pin Registers
// TODO: define all these with the final pin assignments
#define PWM_PORT_DIR
#define PWM_PORT_SEL0
#define PWM_PORT_SEL1
#define PWM_PORT_PIN

#define BRKMOD_PORT_DIR
#define BRKMOD_PORT_OUT
#define BRKMOD_PORT_SEL0
#define BRKMOD_PORT_SEL1
#define BRKMOD_PORT_PIN

#define RD_PORT_DIR
#define RD_PORT_OUT
#define RD_PORT_SEL0
#define RD_PORT_SEL1
#define RD_PORT_PIN

#define FG_PORT_DIR
#define FG_PORT_OUT
#define FG_PORT_SEL0
#define FG_PORT_SEL1
#define FG_PORT_PIN

#define RD_PORT_DIR
#define RD_PORT_OUT
#define RD_PORT_SEL0
#define RD_PORT_SEL1
#define RD_PORT_PIN

// PID constants for our controller
// TODO: characterize these constants
#define KD
#define KP
#define KI

// COSMOS stuff
// TODO: learn COSMOS :eyes:

void rwInit();

#endif /* RW_TEST_H_ */
