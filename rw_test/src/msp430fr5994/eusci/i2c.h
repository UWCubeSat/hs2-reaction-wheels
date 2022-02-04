/*
 * i2c.h
 *
 *  Created on: Feb 3, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_EUSCI_I2C_H_
#define SRC_MSP430FR5994_EUSCI_I2C_H_

namespace MSP430FR5994 {
    namespace eUSCI
    {

        class I2C: public EUSCI_B
        {
            public:
                I2C();
                ~I2C();'

                // EUSCI modules are a shared resource, so we don't want the user to be able to copy it around
                I2C(const I2C &other) = delete;
                I2C(I2C &&other) = delete;
                I2C& operator=(I2C &&other) = delete;
                I2C& operator=(const I2C &other) = delete;

        };

        } /* namespace eUSCI */
    } /* namespace MSP430FR5994 */

#endif /* SRC_MSP430FR5994_EUSCI_I2C_H_ */
