/*
 * timer.cpp
 *
 *  Created on: Jan 31, 2022
 *      Author: sailedeer
 */
#include <cstdint>
#include <msp430.h>

#include "eusci.h"

using namespace MSP430FR5994::eUSCI;

uint8_t eUSCIBase::_inUseTbl = 0;
uint16_t const eUSCIBase::_eusciBaseAddresses[8] = {
    EUSCI_A0_BASE,
    EUSCI_A1_BASE,
    EUSCI_A2_BASE,
    EUSCI_A3_BASE,
    EUSCI_B0_BASE,
    EUSCI_B1_BASE,
    EUSCI_B2_BASE,
    EUSCI_B3_BASE
};


