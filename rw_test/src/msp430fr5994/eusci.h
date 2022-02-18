/*
 * timer.h
 *
 *  Created on: Jan 31, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_TIMER_H_
#define MSP430FR5994_TIMER_H_

#include <cstdint>
#include <msp430.h>

#include "msp430fr5994-hsl.h"

namespace MSP430FR5994 {
    namespace eUSCI {
        enum class eUSCIHandle {
            A0 = 0x00,
            A1 = 0x01,
            A2 = 0x02,
            A3 = 0x03,
            B0 = 0x04,
            B1 = 0x05,
            B2 = 0x06,
            B3 = 0x07,
        };

        // ugly casts since we want clean type conversion
        enum class I2CHandle {
            I2C0 = static_cast<uint8_t>(eUSCIHandle::B0),
            I2C1 = static_cast<uint8_t>(eUSCIHandle::B1),
            I2C2 = static_cast<uint8_t>(eUSCIHandle::B2),
            I2C3 = static_cast<uint8_t>(eUSCIHandle::B3)
        };

        enum class I2CMode {
            PRIMARY = 0x00,
            SECONDARY = 0x01
        };

        // ugly casts since we want clean type conversion
        enum class SPIHandle {
            SPI0 = static_cast<uint8_t>(eUSCIHandle::A0),
            SPI1 = static_cast<uint8_t>(eUSCIHandle::A1),
            SPI2 = static_cast<uint8_t>(eUSCIHandle::A2),
            SPI3 = static_cast<uint8_t>(eUSCIHandle::A3),
            SPI4 = static_cast<uint8_t>(eUSCIHandle::B0),
            SPI5 = static_cast<uint8_t>(eUSCIHandle::B1),
            SPI6 = static_cast<uint8_t>(eUSCIHandle::B2),
            SPI7 = static_cast<uint8_t>(eUSCIHandle::B3)
        };

        // ugly casts since we want clean type conversion
        enum class UARTHandle {
            UART0 = static_cast<uint8_t>(eUSCIHandle::A0),
            UART1 = static_cast<uint8_t>(eUSCIHandle::A1),
            UART2 = static_cast<uint8_t>(eUSCIHandle::A2),
            UART3 = static_cast<uint8_t>(eUSCIHandle::A3),
        };

        // having an entire class for this seems overkill, but I want to be able to
        // read and write to a "global constant" availability table for each module
        // that is only accessible to the various eUSCI modules
        class eUSCIBase {
            public:
                eUSCIBase(eUSCIHandle handle) : _baseHandle(static_cast<uint8_t>(handle)) { }
            protected:
                inline bool ClaimBus() {
                    _attached = !(_inUseTbl & (0x1 << _baseHandle));
                    if (_attached) {
                        _inUseTbl |= (0x1 << _baseHandle);
                    }
                    return _attached;
                }

                inline void ReleaseBus() {
                    if (_attached) {
                        _inUseTbl &= ~(0x1 << _baseHandle);
                    }
                }

                inline uint16_t BaseAddress() {
                    return _eusciBaseAddresses[_baseHandle];
                }

                bool _attached;
                uint32_t _totalTxBytes;
                uint32_t _totalRxBytes;
                uint32_t _errorCount;
                uint8_t _baseHandle;

                static uint8_t _inUseTbl;
                static uint16_t const _eusciBaseAddresses[8];
        };
    }   // namespace eUSCI
}   // namespace MSP430FR5994
#endif /* MSP430FR5994_TIMER_H_ */
