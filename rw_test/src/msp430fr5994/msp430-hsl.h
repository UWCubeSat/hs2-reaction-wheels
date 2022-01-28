/*
 * msp430fr5994.h
 *
 *  Created on: Jan 27, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_MSP430FR5994_H_
#define MSP430FR5994_MSP430FR5994_H_

#include "gpio.h"

namespace MSP430FR5994 {

    template <typename width, uint16_t addr, uint8_t bitMask>
    struct bit_st {
        public:
            inline void set();
            inline void clear();
            inline void toggle();
            inline width get();

        private:
            typedef volatile width & instance_ref_t;
            typedef volatile width * instance_ptr_t;
            inline instance_ref_t instance() {
                return *((instance_ptr_t)(addr));
            }
    };

    template <typename width, uint16_t base_addr, uint8_t off>
    struct reg_st {
        public:
            inline void setBit(width bitMask);
            inline void clearBit(width bitMask);
            inline width getBit(width bitMask);
            inline void toggleBit(width bitMask);

            inline void operator |=(width mask) {
                instance() |= mask;
            }
            inline void operator &=(width mask) {
                instance() &= mask;
            }
            inline void operator ^=(width mask) {
                instance() ^= mask;
            }
        private:
            typedef volatile width & instance_ref_t;
            typedef volatile width * instance_ptr_t;
            inline instance_ref_t instance() {
                return *((instance_ptr_t) (base_addr + off));
            }
    };

    template <typename width, uint16_t base_addr, uint8_t off>
    inline void reg_st<width, base_addr, off>::setBit(width bitMask) {
        instance() |= bitMask;
    }

    template <typename width, uint16_t base_addr, uint8_t off>
    inline void reg_st<width, base_addr, off>::clearBit(width bitMask) {
        instance() &= ~(bitMask);
    }


    template <typename width, uint16_t base_addr, uint8_t off>
    inline width reg_st<width, base_addr, off>::getBit(width bitMask) {
        return (width)(instance() &= bitMask);
    }


    template <typename width, uint16_t base_addr, uint8_t off>
    inline void reg_st<width, base_addr, off>::toggleBit(width bitMask) {
        instance() ^= bitMask;
    }

}   // namespace MSP430FR5994

#endif /* MSP430FR5994_MSP430FR5994_H_ */
