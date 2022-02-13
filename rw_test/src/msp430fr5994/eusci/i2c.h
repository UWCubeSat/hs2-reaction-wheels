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
                 * I2C module specified by handle in either Primary or Secondary mode.
                 * If successful, the Begin() function will return true.
                 */
                I2C(I2CHandle handle, I2CMode mode);

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

                bool Begin();
                void Write(uint8_t data, uint8_t addr);
                void Write(uint8_t *buf, size_t n, uint8_t addr);
                uint8_t Read(uint8_t addr);
                void Read(uint8_t *buf, size_t n, uint8_t addr);
                void End();

                /* TODO: this generates many different instances of the Register class template, so
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
                uint8_t _ownI2CAddress;
                I2CMode _mode;
                I2CHandle _handle;

//                Register<uint16_t> ctlw0;
//                Register<uint8_t> ctl0;
//                Register<uint8_t> ctl1;
//                Register<uint16_t> ctlw1;
//                Register<uint16_t> brw;
//                Register<uint8_t> br0;
//                Register<uint8_t> br1;
//                Register<uint16_t> statw;
//                Register<uint8_t> stat;
//                Register<uint8_t> bcnt;
//                Register<uint16_t> tbcnt;
//                Register<uint8_t> rx;
//                Register<uint8_t> tx;
//                Register<uint16_t> oaddr0;
//                Register<uint16_t> oaddr1;
//                Register<uint16_t> oaddr2;
//                Register<uint16_t> oaddr3;
//                Register<uint16_t> rxaddr;
//                Register<uint16_t> addrmask;
//                Register<uint16_t> saddr;
//                Register<uint16_t> ie;
//                Register<uint16_t> ifg;
//                Register<uint16_t> iv;
        };
    } /* namespace eUSCI */
} /* namespace MSP430FR5994 */

#endif /* MSP430FR5994_EUSCI_I2C_H_ */
