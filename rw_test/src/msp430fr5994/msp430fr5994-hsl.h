/*
 * msp430fr5994-hsl.h
 *
 *  Created on: Jan 30, 2022
 *      Author: sailedeer
 */

#ifndef MSP430FR5994_HSL_H_
#define MSP430FR5994_HSL_H_

#include <cstdint>

namespace MSP430FR5994 {
    template <typename width, uint8_t off>
    struct RegisterBit {
        public:
            RegisterBit(uint16_t baseAddress, width bitMask) : _baseAddress{baseAddress}, _bitMask{bitMask} {}
            inline void set();
            inline void clear();
            inline void toggle();
            inline bool get();

        private:
            typedef volatile width & instance_ref_t;
            typedef volatile width * instance_ptr_t;
            inline instance_ref_t instance() {
                return *((instance_ptr_t) (_baseAddress + off));
            }

            uint16_t _baseAddress;
            width _bitMask;
    };

    template <typename width, uint8_t off>
    inline void RegisterBit<width, off>::set() {
        instance() |= _bitMask;
    }

    template <typename width, uint8_t off>
    inline void RegisterBit<width, off>::clear() {
        instance() &= ~(_bitMask);
    }

    template <typename width, uint8_t off>
    inline void RegisterBit<width, off>::toggle() {
        instance() ^= _bitMask;
    }

    template <typename width, uint8_t off>
    inline bool RegisterBit<width, off>::get() {
        return ((instance() & _bitMask) != 0);
    }

    template <typename width, uint8_t off>
    struct Register {
        public:
            Register(uint16_t baseAddress) : _baseAddress{baseAddress} {}
            inline void setBit(width bitMask);
            inline void clearBit(width bitMask);
            inline bool getBit(width bitMask);
            inline void toggleBit(width bitMask);
            inline void set(width next) { instance() = next; }
            inline width get() { return instance(); }

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
                return *((instance_ptr_t) (_baseAddress + off));
            }
            uint16_t _baseAddress;
    };

    template <typename width, uint8_t off>
    inline void Register<width, off>::setBit(width bitMask) {
        instance() |= bitMask;
    }

    template <typename width, uint8_t off>
    inline void Register<width, off>::clearBit(width bitMask) {
        instance() &= ~(bitMask);
    }


    template <typename width, uint8_t off>
    inline bool Register<width, off>::getBit(width bitMask) {
        return ((instance() &= bitMask) != 0);
    }


    template <typename width, uint8_t off>
    inline void Register<width, off>::toggleBit(width bitMask) {
        instance() ^= bitMask;
    }
}

#endif /* MSP430FR5994_HSL_H_ */
