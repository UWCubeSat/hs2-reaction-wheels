/*
 * gpio.cpp
 *
 *  Created on: Jan 19, 2022
 *      Author: sailedeer
 */

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif
#include <msp430.h>
#ifdef __cplusplus
}
#endif

#include <msp430fr5994/gpio.h>

using namespace MSP430;

// represents a GPIO port base
static struct GPIOPort {
    // generates padding of n bytes; with id as an arbitrary unique
    // identifier for each instance of padding per namespace
    // intended to be used to align bytes in register structs
    #define __PADDING(id, n) char __PAD_##id[n]
    uint8_t in;        // (0x0000)
    __PADDING(0, 0x1);
    uint8_t out;       // (0x0002)
    __PADDING(1, 0x1);
    uint8_t dir;       // (0x0004)
    __PADDING(2, 0x1);
    uint8_t ren;       // (0x0006)
    __PADDING(3, 0x3);
    uint8_t sel0;      // (0x000A)
    __PADDING(4, 0x1);
    uint8_t sel1;      // (0x000C)
    __PADDING(5, 0x9);
    uint8_t selc;      // (0x0016)
    __PADDING(6, 0x1);
    uint8_t ies;       // (0x0018)
    __PADDING(7, 0x1);
    uint8_t ie;        // (0x001A)
    __PADDING(8, 0x1);
    uint8_t ifg;       // (0x001C)
};

static volatile *GPIOBase bases[9] = {
    (struct GPIOPx *)  P1_BASE,
    (struct GPIOPx *) (P2_BASE + 0x1),  // offset by 1 byte
    (struct GPIOPx *)  P3_BASE,
    (struct GPIOPx *) (P4_BASE + 0x1),  // offset by 1 byte
    (struct GPIOPx *)  P5_BASE,
    (struct GPIOPx *) (P6_BASE + 0x1),  // offset by 1 byte
    (struct GPIOPx *)  P7_BASE,
    (struct GPIOPx *) (P8_BASE + 0x1),  // offset by 1 byte
    (struct GPIOPx *)  PJ_BASE
};

GPIO::Direction GPIO::GetPinDirection(GPIO::Pin pin) {
    GPIOBase *base = bases[pin.base];
}


