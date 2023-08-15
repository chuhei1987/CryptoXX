#pragma once
#include "block.hpp"
#define ROTL28(x, n) ((x) << (n) & 0x0FFFFFFF | (x) >> (28 - (n)))
// choose the smallest type that can hold the number of bits
template <int bits>
using DESUint = typename std::conditional<bits <= 32, uint32_t, uint64_t>::type;
template <int wi, int wo>
struct DESPermutation {
    std::array<std::array<DESUint<wo>, 256>, wi / 8> LUT = {};
    // generate lookup table for P-boxes
    constexpr DESPermutation(std::initializer_list<bits_t> A) {
        for (int o = 0; o < wo; o++) {
            int i = A.begin()[o];
            for (DESUint<wi> x = 0; x < 256; x++) {
                LUT[i / 8][x] |= (DESUint<wo>)(x >> i % 8 & 1) << (wo - 1 - o);
            }
        }
    }
    // permutation function optimized using lookup table
    DESUint<wo> operator()(DESUint<wi> vi) const {
        DESUint<wo> vo = 0;
        FOR(i, 0, i + 8, i < wi, {
            vo |= LUT[i / 8][vi >> i & 0xff];
        });
        return vo;
    }
};
class DES {
    static constexpr DESPermutation<64, 56> PC1 = {
        0x07, 0x0f, 0x17, 0x1f, 0x27, 0x2f, 0x37, 0x3f,
        0x06, 0x0e, 0x16, 0x1e, 0x26, 0x2e, 0x36, 0x3e,
        0x05, 0x0d, 0x15, 0x1d, 0x25, 0x2d, 0x35, 0x3d,
        0x04, 0x0c, 0x14, 0x1c, 0x01, 0x09, 0x11, 0x19,
        0x21, 0x29, 0x31, 0x39, 0x02, 0x0a, 0x12, 0x1a,
        0x22, 0x2a, 0x32, 0x3a, 0x03, 0x0b, 0x13, 0x1b,
        0x23, 0x2b, 0x33, 0x3b, 0x24, 0x2c, 0x34, 0x3c,
    };
    static constexpr DESPermutation<56, 48> PC2 = {
        0x2a, 0x27, 0x2d, 0x20, 0x37, 0x33, 0x35, 0x1c,
        0x29, 0x32, 0x23, 0x2e, 0x21, 0x25, 0x2c, 0x34,
        0x1e, 0x30, 0x28, 0x31, 0x1d, 0x24, 0x2b, 0x36,
        0x0f, 0x04, 0x19, 0x13, 0x09, 0x01, 0x1a, 0x10,
        0x05, 0x0b, 0x17, 0x08, 0x0c, 0x07, 0x11, 0x00,
        0x16, 0x03, 0x0a, 0x0e, 0x06, 0x14, 0x1b, 0x18,
    };
    static constexpr DESPermutation<64, 64> IP = {
        0x06, 0x0e, 0x16, 0x1e, 0x26, 0x2e, 0x36, 0x3e,
        0x04, 0x0c, 0x14, 0x1c, 0x24, 0x2c, 0x34, 0x3c,
        0x02, 0x0a, 0x12, 0x1a, 0x22, 0x2a, 0x32, 0x3a,
        0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38,
        0x07, 0x0f, 0x17, 0x1f, 0x27, 0x2f, 0x37, 0x3f,
        0x05, 0x0d, 0x15, 0x1d, 0x25, 0x2d, 0x35, 0x3d,
        0x03, 0x0b, 0x13, 0x1b, 0x23, 0x2b, 0x33, 0x3b,
        0x01, 0x09, 0x11, 0x19, 0x21, 0x29, 0x31, 0x39,
    };
    static constexpr DESPermutation<64, 64> FP = {
        0x18, 0x38, 0x10, 0x30, 0x08, 0x28, 0x00, 0x20,
        0x19, 0x39, 0x11, 0x31, 0x09, 0x29, 0x01, 0x21,
        0x1a, 0x3a, 0x12, 0x32, 0x0a, 0x2a, 0x02, 0x22,
        0x1b, 0x3b, 0x13, 0x33, 0x0b, 0x2b, 0x03, 0x23,
        0x1c, 0x3c, 0x14, 0x34, 0x0c, 0x2c, 0x04, 0x24,
        0x1d, 0x3d, 0x15, 0x35, 0x0d, 0x2d, 0x05, 0x25,
        0x1e, 0x3e, 0x16, 0x36, 0x0e, 0x2e, 0x06, 0x26,
        0x1f, 0x3f, 0x17, 0x37, 0x0f, 0x2f, 0x07, 0x27,
    };
    static constexpr DESPermutation<32, 48> E = {
        0x00, 0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1c, 0x1b,
        0x1a, 0x19, 0x18, 0x17, 0x18, 0x17, 0x16, 0x15,
        0x14, 0x13, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0f,
        0x10, 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0c, 0x0b,
        0x0a, 0x09, 0x08, 0x07, 0x08, 0x07, 0x06, 0x05,
        0x04, 0x03, 0x04, 0x03, 0x02, 0x01, 0x00, 0x1f,
    };
    static constexpr DESPermutation<32, 32> P = {
        0x10, 0x19, 0x0c, 0x0b, 0x03, 0x14, 0x04, 0x0f,
        0x1f, 0x11, 0x09, 0x06, 0x1b, 0x0e, 0x01, 0x16,
        0x1e, 0x18, 0x08, 0x12, 0x00, 0x05, 0x1d, 0x17,
        0x0d, 0x13, 0x02, 0x1a, 0x0a, 0x15, 0x1c, 0x07,
    };
    static constexpr uint8_t S_BOX[8][64] = {
        0xe, 0x0, 0x4, 0xf, 0xd, 0x7, 0x1, 0x4, 0x2, 0xe, 0xf, 0x2, 0xb, 0xd, 0x8, 0x1,
        0x3, 0xa, 0xa, 0x6, 0x6, 0xc, 0xc, 0xb, 0x5, 0x9, 0x9, 0x5, 0x0, 0x3, 0x7, 0x8,
        0x4, 0xf, 0x1, 0xc, 0xe, 0x8, 0x8, 0x2, 0xd, 0x4, 0x6, 0x9, 0x2, 0x1, 0xb, 0x7,
        0xf, 0x5, 0xc, 0xb, 0x9, 0x3, 0x7, 0xe, 0x3, 0xa, 0xa, 0x0, 0x5, 0x6, 0x0, 0xd,
        0xf, 0x3, 0x1, 0xd, 0x8, 0x4, 0xe, 0x7, 0x6, 0xf, 0xb, 0x2, 0x3, 0x8, 0x4, 0xe,
        0x9, 0xc, 0x7, 0x0, 0x2, 0x1, 0xd, 0xa, 0xc, 0x6, 0x0, 0x9, 0x5, 0xb, 0xa, 0x5,
        0x0, 0xd, 0xe, 0x8, 0x7, 0xa, 0xb, 0x1, 0xa, 0x3, 0x4, 0xf, 0xd, 0x4, 0x1, 0x2,
        0x5, 0xb, 0x8, 0x6, 0xc, 0x7, 0x6, 0xc, 0x9, 0x0, 0x3, 0x5, 0x2, 0xe, 0xf, 0x9,
        0xa, 0xd, 0x0, 0x7, 0x9, 0x0, 0xe, 0x9, 0x6, 0x3, 0x3, 0x4, 0xf, 0x6, 0x5, 0xa,
        0x1, 0x2, 0xd, 0x8, 0xc, 0x5, 0x7, 0xe, 0xb, 0xc, 0x4, 0xb, 0x2, 0xf, 0x8, 0x1,
        0xd, 0x1, 0x6, 0xa, 0x4, 0xd, 0x9, 0x0, 0x8, 0x6, 0xf, 0x9, 0x3, 0x8, 0x0, 0x7,
        0xb, 0x4, 0x1, 0xf, 0x2, 0xe, 0xc, 0x3, 0x5, 0xb, 0xa, 0x5, 0xe, 0x2, 0x7, 0xc,
        0x7, 0xd, 0xd, 0x8, 0xe, 0xb, 0x3, 0x5, 0x0, 0x6, 0x6, 0xf, 0x9, 0x0, 0xa, 0x3,
        0x1, 0x4, 0x2, 0x7, 0x8, 0x2, 0x5, 0xc, 0xb, 0x1, 0xc, 0xa, 0x4, 0xe, 0xf, 0x9,
        0xa, 0x3, 0x6, 0xf, 0x9, 0x0, 0x0, 0x6, 0xc, 0xa, 0xb, 0x1, 0x7, 0xd, 0xd, 0x8,
        0xf, 0x9, 0x1, 0x4, 0x3, 0x5, 0xe, 0xb, 0x5, 0xc, 0x2, 0x7, 0x8, 0x2, 0x4, 0xe,
        0x2, 0xe, 0xc, 0xb, 0x4, 0x2, 0x1, 0xc, 0x7, 0x4, 0xa, 0x7, 0xb, 0xd, 0x6, 0x1,
        0x8, 0x5, 0x5, 0x0, 0x3, 0xf, 0xf, 0xa, 0xd, 0x3, 0x0, 0x9, 0xe, 0x8, 0x9, 0x6,
        0x4, 0xb, 0x2, 0x8, 0x1, 0xc, 0xb, 0x7, 0xa, 0x1, 0xd, 0xe, 0x7, 0x2, 0x8, 0xd,
        0xf, 0x6, 0x9, 0xf, 0xc, 0x0, 0x5, 0x9, 0x6, 0xa, 0x3, 0x4, 0x0, 0x5, 0xe, 0x3,
        0xc, 0xa, 0x1, 0xf, 0xa, 0x4, 0xf, 0x2, 0x9, 0x7, 0x2, 0xc, 0x6, 0x9, 0x8, 0x5,
        0x0, 0x6, 0xd, 0x1, 0x3, 0xd, 0x4, 0xe, 0xe, 0x0, 0x7, 0xb, 0x5, 0x3, 0xb, 0x8,
        0x9, 0x4, 0xe, 0x3, 0xf, 0x2, 0x5, 0xc, 0x2, 0x9, 0x8, 0x5, 0xc, 0xf, 0x3, 0xa,
        0x7, 0xb, 0x0, 0xe, 0x4, 0x1, 0xa, 0x7, 0x1, 0x6, 0xd, 0x0, 0xb, 0x8, 0x6, 0xd,
        0x4, 0xd, 0xb, 0x0, 0x2, 0xb, 0xe, 0x7, 0xf, 0x4, 0x0, 0x9, 0x8, 0x1, 0xd, 0xa,
        0x3, 0xe, 0xc, 0x3, 0x9, 0x5, 0x7, 0xc, 0x5, 0x2, 0xa, 0xf, 0x6, 0x8, 0x1, 0x6,
        0x1, 0x6, 0x4, 0xb, 0xb, 0xd, 0xd, 0x8, 0xc, 0x1, 0x3, 0x4, 0x7, 0xa, 0xe, 0x7,
        0xa, 0x9, 0xf, 0x5, 0x6, 0x0, 0x8, 0xf, 0x0, 0xe, 0x5, 0x2, 0x9, 0x3, 0x2, 0xc,
        0xd, 0x1, 0x2, 0xf, 0x8, 0xd, 0x4, 0x8, 0x6, 0xa, 0xf, 0x3, 0xb, 0x7, 0x1, 0x4,
        0xa, 0xc, 0x9, 0x5, 0x3, 0x6, 0xe, 0xb, 0x5, 0x0, 0x0, 0xe, 0xc, 0x9, 0x7, 0x2,
        0x7, 0x2, 0xb, 0x1, 0x4, 0xe, 0x1, 0x7, 0x9, 0x4, 0xc, 0xa, 0xe, 0x8, 0x2, 0xd,
        0x0, 0xf, 0x6, 0xc, 0xa, 0x9, 0xd, 0x0, 0xf, 0x3, 0x3, 0x5, 0x5, 0x6, 0x8, 0xb,
    };
    static constexpr bits_t SHIFT[16] = {
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1,
    };
    static DESUint<32> F(DESUint<32> r, DESUint<48> k) {
        DESUint<48> x = E(r) ^ k;
        // Merge the substitution and permutation operations
        return
            P.LUT[0][S_BOX[6][x >>  6 & 0x3f] << 4 | S_BOX[7][x       & 0x3f]] |
            P.LUT[1][S_BOX[4][x >> 18 & 0x3f] << 4 | S_BOX[5][x >> 12 & 0x3f]] |
            P.LUT[2][S_BOX[2][x >> 30 & 0x3f] << 4 | S_BOX[3][x >> 24 & 0x3f]] |
            P.LUT[3][S_BOX[0][x >> 42       ] << 4 | S_BOX[1][x >> 36 & 0x3f]];
    }
    DESUint<48> rk[16];
public:
    static constexpr size_t BLOCK_SIZE = 8;
    static constexpr size_t KEY_SIZE = 8;
    DES(uint8_t const *mk) {
        DESUint<56> t = PC1(GET_BE<DESUint<64>>(mk));
        DESUint<28> l = t >> 28, r = t & 0x0FFFFFFF;
        for (int i = 0; i < 16; i++) {
            l = ROTL28(l, SHIFT[i]);
            r = ROTL28(r, SHIFT[i]);
            rk[i] = PC2((DESUint<56>)l << 28 | r);
        }
    }
    void encrypt(uint8_t const *src, uint8_t *dst) const {
        DESUint<64> t = IP(GET_BE<DESUint<64>>(src));
        DESUint<32> l = t >> 32, r = t & 0xFFFFFFFF;
        for (int i = 0; i < 16;) {
            l ^= F(r, rk[i++]);
            r ^= F(l, rk[i++]);
        }
        PUT_BE(dst, FP((DESUint<64>)r << 32 | l));
    }
    void decrypt(uint8_t const *src, uint8_t *dst) const {
        DESUint<64> t = IP(GET_BE<DESUint<64>>(src));
        DESUint<32> l = t >> 32, r = t & 0xFFFFFFFF;
        for (int i = 16; i > 0;) {
            l ^= F(r, rk[--i]);
            r ^= F(l, rk[--i]);
        }
        PUT_BE(dst, FP((DESUint<64>)r << 32 | l));
    }
};
