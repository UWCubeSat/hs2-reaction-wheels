/*
 * i2c.h
 *
 *  Created on: Feb 3, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_EUSCI_I2C_H_
#define MSP430FR5994_EUSCI_I2C_H_

#include <cstdint>
#include <msp430.h>
#include "../gpio.h"
#include "../eusci.h"

namespace MSP430FR5994 {
    namespace eUSCI {
        class I2C : public eUSCIBase {
            public:
                /*
                 * Construct a new I2C instance which represents the underlying
                 * I2C module specified by handle in Secondary mode with ownAddress.
                 * If successful, the Begin() function will return true.
                 */
//                I2C(I2CHandle handle, uint8_t ownAddress);

                /*
                 * Construct a new I2C instance which represents the underlying
                 * I2C module specified by handle in Primary mode.
                 * If successful, the Begin() function will return true.
                 */
                I2C(I2CHandle handle);

                /*
                 * Relinquish the handle of the underlying I2C module on object
                 * destruction.
                 */
                ~I2C();

                /*
                 * EUSCI modules are a shared resource, so we don't want the user
                 * moving or copying these objects around. Thus all move and copy
                 * functions are deleted.
                 */
                I2C(const I2C &other) = delete;
                I2C(I2C &&other) = delete;
                I2C& operator=(I2C &&other) = delete;
                I2C& operator=(const I2C &other) = delete;

                void Write(uint8_t *buf, uint16_t n, uint8_t addr, bool autoStop);
                void Read(uint8_t *buf, uint16_t n, uint8_t addr, bool autoStop);
                void CombinedWriteRead(uint8_t *rxBuf, uint16_t rxBytes, uint8_t *txBuf, uint16_t txBytes, uint8_t addr);

                /*
                 * TODO: this generates many different instances of the Register class template, so
                 * it will likely be better practice in the future to pass the address into
                 * the constructor and store it as a data member. Need to benchmark the size
                 * of the resultant program.
                 */
                Register<uint16_t, OFS_UCB0CTLW0> ctlw0;
                Register<uint8_t, OFS_UCB0CTLW0_L> ctl0;
                Register<uint8_t, OFS_UCB0CTLW0_H> ctl1;
                Register<uint16_t, OFS_UCB0CTLW1> ctlw1;
                Register<uint16_t, OFS_UCB0BRW> brw;
                Register<uint8_t, OFS_UCB0BRW_L> br0;
                Register<uint8_t, OFS_UCB0BRW_H> br1;
                Register<uint16_t, OFS_UCB0STATW> statw;
                Register<uint8_t, OFS_UCB0STATW_L> stat;
                Register<uint8_t, OFS_UCB0STATW_H> bcnt;
                Register<uint16_t, OFS_UCB0TBCNT> tbcnt;
                Register<uint8_t, OFS_UCB0RXBUF> rx;
                Register<uint8_t, OFS_UCB0TXBUF> tx;
                Register<uint16_t, OFS_UCB0I2COA0> oaddr0;
                Register<uint16_t, OFS_UCB0I2COA1> oaddr1;
                Register<uint16_t, OFS_UCB0I2COA2> oaddr2;
                Register<uint16_t, OFS_UCB0I2COA3> oaddr3;
                Register<uint16_t, OFS_UCB0ADDRX> rxaddr;
                Register<uint16_t, OFS_UCB0ADDMASK> addrmask;
                Register<uint16_t, OFS_UCB0I2CSA> saddr;
                Register<uint16_t, OFS_UCB0IE> ie;
                Register<uint16_t, OFS_UCB0IFG> ifg;
                Register<uint16_t, OFS_UCB0IV> iv;

            private:
                uint8_t _ownAddress;
                I2CMode _mode;
                uint8_t _handle;
                uint8_t _lastOpResult;
                inline void SetAutoStopBytes(uint16_t n) {
                    ctlw0.setBit(UCSWRST);
                    tbcnt.set(n);
                    ctlw0.clearBit(UCSWRST);
                }
        };
    } /* namespace eUSCI */
} /* namespace MSP430FR5994 */

#endif /* MSP430FR5994_EUSCI_I2C_H_ */
