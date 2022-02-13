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

bool eUSCIBase::IsAvailable() {
    int handle = static_cast<int>(_baseHandle);
    return static_cast<bool>((_inUseTbl & (0x1 << handle)));
}

