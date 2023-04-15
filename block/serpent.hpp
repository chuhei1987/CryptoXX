#pragma once
#include "block.hpp"
#define XXROTL(x) {                           \
    x[0] = ROTL(x[0]                   , 13); \
    x[2] = ROTL(x[2]                   ,  3); \
    x[3] = ROTL(x[3] ^ x[2] ^ x[0] << 3,  7); \
    x[1] = ROTL(x[1] ^ x[0] ^ x[2]     ,  1); \
    x[0] = ROTL(x[0] ^ x[1] ^ x[3]     ,  5); \
    x[2] = ROTL(x[2] ^ x[3] ^ x[1] << 7, 22); \
}
#define XXROTR(x) {                           \
    x[2] = ROTR(x[2], 22) ^ x[3] ^ x[1] << 7; \
    x[0] = ROTR(x[0],  5) ^ x[1] ^ x[3]     ; \
    x[1] = ROTR(x[1],  1) ^ x[0] ^ x[2]     ; \
    x[3] = ROTR(x[3],  7) ^ x[2] ^ x[0] << 3; \
    x[2] = ROTR(x[2],  3)                   ; \
    x[0] = ROTR(x[0], 13)                   ; \
}
#define XK(r, x) {          \
    x[0] ^= rk[4 * r +  8]; \
    x[1] ^= rk[4 * r +  9]; \
    x[2] ^= rk[4 * r + 10]; \
    x[3] ^= rk[4 * r + 11]; \
}
#define LK(r, x) {          \
    x[0]  = rk[4 * r +  8]; \
    x[1]  = rk[4 * r +  9]; \
    x[2]  = rk[4 * r + 10]; \
    x[3]  = rk[4 * r + 11]; \
}
#define SK(r, x) {          \
    rk[4 * r +  8]  = x[0]; \
    rk[4 * r +  9]  = x[1]; \
    rk[4 * r + 10]  = x[2]; \
    rk[4 * r + 11]  = x[3]; \
}
#define SB0(R) {  \
	R[3] ^= R[0]; \
	R[4] =  R[1]; \
	R[1] &= R[3]; \
	R[4] ^= R[2]; \
	R[1] ^= R[0]; \
	R[0] |= R[3]; \
	R[0] ^= R[4]; \
	R[4] ^= R[3]; \
	R[3] ^= R[2]; \
	R[2] |= R[1]; \
	R[2] ^= R[4]; \
	R[4] = ~R[4]; \
	R[4] |= R[1]; \
	R[1] ^= R[3]; \
	R[1] ^= R[4]; \
	R[3] |= R[0]; \
	R[1] ^= R[3]; \
	R[4] ^= R[3]; \
	R[3] =  R[0]; \
	R[0] =  R[1]; \
	R[1] =  R[4]; \
}
#define IB0(R) {  \
	R[2] = ~R[2]; \
	R[4] =  R[1]; \
	R[1] |= R[0]; \
	R[4] = ~R[4]; \
	R[1] ^= R[2]; \
	R[2] |= R[4]; \
	R[1] ^= R[3]; \
	R[0] ^= R[4]; \
	R[2] ^= R[0]; \
	R[0] &= R[3]; \
	R[4] ^= R[0]; \
	R[0] |= R[1]; \
	R[0] ^= R[2]; \
	R[3] ^= R[4]; \
	R[2] ^= R[1]; \
	R[3] ^= R[0]; \
	R[3] ^= R[1]; \
	R[2] &= R[3]; \
	R[4] ^= R[2]; \
	R[2] =  R[1]; \
	R[1] =  R[4]; \
}
#define SB1(R) {  \
	R[0] = ~R[0]; \
	R[2] = ~R[2]; \
	R[4] =  R[0]; \
	R[0] &= R[1]; \
	R[2] ^= R[0]; \
	R[0] |= R[3]; \
	R[3] ^= R[2]; \
	R[1] ^= R[0]; \
	R[0] ^= R[4]; \
	R[4] |= R[1]; \
	R[1] ^= R[3]; \
	R[2] |= R[0]; \
	R[2] &= R[4]; \
	R[0] ^= R[1]; \
	R[1] &= R[2]; \
	R[1] ^= R[0]; \
	R[0] &= R[2]; \
	R[4] ^= R[0]; \
	R[0] =  R[2]; \
	R[2] =  R[3]; \
	R[3] =  R[1]; \
	R[1] =  R[4]; \
}
#define IB1(R) {  \
	R[4] =  R[1]; \
	R[1] ^= R[3]; \
	R[3] &= R[1]; \
	R[4] ^= R[2]; \
	R[3] ^= R[0]; \
	R[0] |= R[1]; \
	R[2] ^= R[3]; \
	R[0] ^= R[4]; \
	R[0] |= R[2]; \
	R[1] ^= R[3]; \
	R[0] ^= R[1]; \
	R[1] |= R[3]; \
	R[1] ^= R[0]; \
	R[4] = ~R[4]; \
	R[4] ^= R[1]; \
	R[1] |= R[0]; \
	R[1] ^= R[0]; \
	R[1] |= R[4]; \
	R[3] ^= R[1]; \
	R[1] =  R[0]; \
	R[0] =  R[4]; \
	R[4] =  R[2]; \
	R[2] =  R[3]; \
	R[3] =  R[4]; \
}
#define SB2(R) {  \
	R[4] =  R[0]; \
	R[0] &= R[2]; \
	R[0] ^= R[3]; \
	R[2] ^= R[1]; \
	R[2] ^= R[0]; \
	R[3] |= R[4]; \
	R[3] ^= R[1]; \
	R[4] ^= R[2]; \
	R[1] =  R[3]; \
	R[3] |= R[4]; \
	R[3] ^= R[0]; \
	R[0] &= R[1]; \
	R[4] ^= R[0]; \
	R[1] ^= R[3]; \
	R[1] ^= R[4]; \
	R[0] =  R[2]; \
	R[2] =  R[1]; \
	R[1] =  R[3]; \
	R[3] = ~R[4]; \
}
#define IB2(R) {  \
	R[2] ^= R[3]; \
	R[3] ^= R[0]; \
	R[4] =  R[3]; \
	R[3] &= R[2]; \
	R[3] ^= R[1]; \
	R[1] |= R[2]; \
	R[1] ^= R[4]; \
	R[4] &= R[3]; \
	R[2] ^= R[3]; \
	R[4] &= R[0]; \
	R[4] ^= R[2]; \
	R[2] &= R[1]; \
	R[2] |= R[0]; \
	R[3] = ~R[3]; \
	R[2] ^= R[3]; \
	R[0] ^= R[3]; \
	R[0] &= R[1]; \
	R[3] ^= R[4]; \
	R[3] ^= R[0]; \
	R[0] =  R[1]; \
	R[1] =  R[4]; \
}
#define SB3(R) {  \
	R[4] =  R[0]; \
	R[0] |= R[3]; \
	R[3] ^= R[1]; \
	R[1] &= R[4]; \
	R[4] ^= R[2]; \
	R[2] ^= R[3]; \
	R[3] &= R[0]; \
	R[4] |= R[1]; \
	R[3] ^= R[4]; \
	R[0] ^= R[1]; \
	R[4] &= R[0]; \
	R[1] ^= R[3]; \
	R[4] ^= R[2]; \
	R[1] |= R[0]; \
	R[1] ^= R[2]; \
	R[0] ^= R[3]; \
	R[2] =  R[1]; \
	R[1] |= R[3]; \
	R[0] ^= R[1]; \
	R[1] =  R[2]; \
	R[2] =  R[3]; \
	R[3] =  R[4]; \
}
#define IB3(R) {  \
	R[4] =  R[2]; \
	R[2] ^= R[1]; \
	R[0] ^= R[2]; \
	R[4] &= R[2]; \
	R[4] ^= R[0]; \
	R[0] &= R[1]; \
	R[1] ^= R[3]; \
	R[3] |= R[4]; \
	R[2] ^= R[3]; \
	R[0] ^= R[3]; \
	R[1] ^= R[4]; \
	R[3] &= R[2]; \
	R[3] ^= R[1]; \
	R[1] ^= R[0]; \
	R[1] |= R[2]; \
	R[0] ^= R[3]; \
	R[1] ^= R[4]; \
	R[0] ^= R[1]; \
	R[4] =  R[0]; \
	R[0] =  R[2]; \
	R[2] =  R[3]; \
	R[3] =  R[4]; \
}
#define SB4(R) {  \
	R[1] ^= R[3]; \
	R[3] = ~R[3]; \
	R[2] ^= R[3]; \
	R[3] ^= R[0]; \
	R[4] =  R[1]; \
	R[1] &= R[3]; \
	R[1] ^= R[2]; \
	R[4] ^= R[3]; \
	R[0] ^= R[4]; \
	R[2] &= R[4]; \
	R[2] ^= R[0]; \
	R[0] &= R[1]; \
	R[3] ^= R[0]; \
	R[4] |= R[1]; \
	R[4] ^= R[0]; \
	R[0] |= R[3]; \
	R[0] ^= R[2]; \
	R[2] &= R[3]; \
	R[0] = ~R[0]; \
	R[4] ^= R[2]; \
	R[2] =  R[0]; \
	R[0] =  R[1]; \
	R[1] =  R[4]; \
}
#define IB4(R) {  \
	R[4] =  R[2]; \
	R[2] &= R[3]; \
	R[2] ^= R[1]; \
	R[1] |= R[3]; \
	R[1] &= R[0]; \
	R[4] ^= R[2]; \
	R[4] ^= R[1]; \
	R[1] &= R[2]; \
	R[0] = ~R[0]; \
	R[3] ^= R[4]; \
	R[1] ^= R[3]; \
	R[3] &= R[0]; \
	R[3] ^= R[2]; \
	R[0] ^= R[1]; \
	R[2] &= R[0]; \
	R[3] ^= R[0]; \
	R[2] ^= R[4]; \
	R[2] |= R[3]; \
	R[3] ^= R[0]; \
	R[2] ^= R[1]; \
	R[1] =  R[3]; \
	R[3] =  R[4]; \
}
#define SB5(R) {  \
	R[0] ^= R[1]; \
	R[1] ^= R[3]; \
	R[3] = ~R[3]; \
	R[4] =  R[1]; \
	R[1] &= R[0]; \
	R[2] ^= R[3]; \
	R[1] ^= R[2]; \
	R[2] |= R[4]; \
	R[4] ^= R[3]; \
	R[3] &= R[1]; \
	R[3] ^= R[0]; \
	R[4] ^= R[1]; \
	R[4] ^= R[2]; \
	R[2] ^= R[0]; \
	R[0] &= R[3]; \
	R[2] = ~R[2]; \
	R[0] ^= R[4]; \
	R[4] |= R[3]; \
	R[4] ^= R[2]; \
	R[2] =  R[0]; \
	R[0] =  R[1]; \
	R[1] =  R[3]; \
	R[3] =  R[4]; \
}
#define IB5(R) {  \
	R[1] = ~R[1]; \
	R[4] =  R[3]; \
	R[2] ^= R[1]; \
	R[3] |= R[0]; \
	R[3] ^= R[2]; \
	R[2] |= R[1]; \
	R[2] &= R[0]; \
	R[4] ^= R[3]; \
	R[2] ^= R[4]; \
	R[4] |= R[0]; \
	R[4] ^= R[1]; \
	R[1] &= R[2]; \
	R[1] ^= R[3]; \
	R[4] ^= R[2]; \
	R[3] &= R[4]; \
	R[4] ^= R[1]; \
	R[3] ^= R[4]; \
	R[4] = ~R[4]; \
	R[3] ^= R[0]; \
	R[0] =  R[1]; \
	R[1] =  R[4]; \
	R[4] =  R[3]; \
	R[3] =  R[2]; \
	R[2] =  R[4]; \
}
#define SB6(R) {  \
	R[2] = ~R[2]; \
	R[4] =  R[3]; \
	R[3] &= R[0]; \
	R[0] ^= R[4]; \
	R[3] ^= R[2]; \
	R[2] |= R[4]; \
	R[1] ^= R[3]; \
	R[2] ^= R[0]; \
	R[0] |= R[1]; \
	R[2] ^= R[1]; \
	R[4] ^= R[0]; \
	R[0] |= R[3]; \
	R[0] ^= R[2]; \
	R[4] ^= R[3]; \
	R[4] ^= R[0]; \
	R[3] = ~R[3]; \
	R[2] &= R[4]; \
	R[3] ^= R[2]; \
	R[2] =  R[4]; \
}
#define IB6(R) {  \
	R[0] ^= R[2]; \
	R[4] =  R[2]; \
	R[2] &= R[0]; \
	R[4] ^= R[3]; \
	R[2] = ~R[2]; \
	R[3] ^= R[1]; \
	R[2] ^= R[3]; \
	R[4] |= R[0]; \
	R[0] ^= R[2]; \
	R[3] ^= R[4]; \
	R[4] ^= R[1]; \
	R[1] &= R[3]; \
	R[1] ^= R[0]; \
	R[0] ^= R[3]; \
	R[0] |= R[2]; \
	R[3] ^= R[1]; \
	R[4] ^= R[0]; \
	R[0] =  R[1]; \
	R[1] =  R[2]; \
	R[2] =  R[4]; \
}
#define SB7(R) {  \
	R[4] =  R[1]; \
	R[1] |= R[2]; \
	R[1] ^= R[3]; \
	R[4] ^= R[2]; \
	R[2] ^= R[1]; \
	R[3] |= R[4]; \
	R[3] &= R[0]; \
	R[4] ^= R[2]; \
	R[3] ^= R[1]; \
	R[1] |= R[4]; \
	R[1] ^= R[0]; \
	R[0] |= R[4]; \
	R[0] ^= R[2]; \
	R[1] ^= R[4]; \
	R[2] ^= R[1]; \
	R[1] &= R[0]; \
	R[1] ^= R[4]; \
	R[2] = ~R[2]; \
	R[2] |= R[0]; \
	R[4] ^= R[2]; \
	R[2] =  R[1]; \
	R[1] =  R[3]; \
	R[3] =  R[0]; \
	R[0] =  R[4]; \
}
#define IB7(R) {  \
	R[4] =  R[2]; \
	R[2] ^= R[0]; \
	R[0] &= R[3]; \
	R[4] |= R[3]; \
	R[2] = ~R[2]; \
	R[3] ^= R[1]; \
	R[1] |= R[0]; \
	R[0] ^= R[2]; \
	R[2] &= R[4]; \
	R[3] &= R[4]; \
	R[1] ^= R[2]; \
	R[2] ^= R[0]; \
	R[0] |= R[2]; \
	R[4] ^= R[1]; \
	R[0] ^= R[3]; \
	R[3] ^= R[4]; \
	R[4] |= R[0]; \
	R[3] ^= R[2]; \
	R[4] ^= R[2]; \
	R[2] =  R[1]; \
	R[1] =  R[0]; \
	R[0] =  R[3]; \
	R[3] =  R[4]; \
}
class SerpentBase {};
template <int L>
	requires (L == 4 || L == 6 || L == 8)
class SerpentTmpl: public SerpentBase {
    uint32_t rk[140] = {};
public:
    static constexpr size_t BLOCK_SIZE = 16;
    SerpentTmpl(uint8_t const *kin) {
        uint32_t x[5]; // 5th element is used as a temporary
        READ_LE(rk, kin, L);
        if constexpr (L < 8) {
            rk[L] = 1; // padding with 1 bit and 0 bits to make 256 bits
        }
        for (int i = 0; i < 132; ++i) {
            rk[i + 8] = ROTL(rk[i] ^ rk[i + 3] ^ rk[i + 5] ^ rk[i + 7] ^ 0x9E3779B9 ^ i, 11);
        }
        LK( 0, x); SB3(x); SK( 0, x);
        LK( 1, x); SB2(x); SK( 1, x);
        LK( 2, x); SB1(x); SK( 2, x);
        LK( 3, x); SB0(x); SK( 3, x);
        LK( 4, x); SB7(x); SK( 4, x);
        LK( 5, x); SB6(x); SK( 5, x);
        LK( 6, x); SB5(x); SK( 6, x);
        LK( 7, x); SB4(x); SK( 7, x);
        LK( 8, x); SB3(x); SK( 8, x);
        LK( 9, x); SB2(x); SK( 9, x);
        LK(10, x); SB1(x); SK(10, x);
        LK(11, x); SB0(x); SK(11, x);
        LK(12, x); SB7(x); SK(12, x);
        LK(13, x); SB6(x); SK(13, x);
        LK(14, x); SB5(x); SK(14, x);
        LK(15, x); SB4(x); SK(15, x);
        LK(16, x); SB3(x); SK(16, x);
        LK(17, x); SB2(x); SK(17, x);
        LK(18, x); SB1(x); SK(18, x);
        LK(19, x); SB0(x); SK(19, x);
        LK(20, x); SB7(x); SK(20, x);
        LK(21, x); SB6(x); SK(21, x);
        LK(22, x); SB5(x); SK(22, x);
        LK(23, x); SB4(x); SK(23, x);
        LK(24, x); SB3(x); SK(24, x);
        LK(25, x); SB2(x); SK(25, x);
        LK(26, x); SB1(x); SK(26, x);
        LK(27, x); SB0(x); SK(27, x);
        LK(28, x); SB7(x); SK(28, x);
        LK(29, x); SB6(x); SK(29, x);
        LK(30, x); SB5(x); SK(30, x);
        LK(31, x); SB4(x); SK(31, x);
        LK(32, x); SB3(x); SK(32, x);
    }
    void encrypt(uint8_t const *src, uint8_t *dst) const {
        uint32_t x[5]; // 5th element is used as a temporary
        READ_LE(x, src, 4);
        XK( 0, x); SB0(x); XXROTL(x);
        XK( 1, x); SB1(x); XXROTL(x);
        XK( 2, x); SB2(x); XXROTL(x);
        XK( 3, x); SB3(x); XXROTL(x);
        XK( 4, x); SB4(x); XXROTL(x);
        XK( 5, x); SB5(x); XXROTL(x);
        XK( 6, x); SB6(x); XXROTL(x);
        XK( 7, x); SB7(x); XXROTL(x);
        XK( 8, x); SB0(x); XXROTL(x);
        XK( 9, x); SB1(x); XXROTL(x);
        XK(10, x); SB2(x); XXROTL(x);
        XK(11, x); SB3(x); XXROTL(x);
        XK(12, x); SB4(x); XXROTL(x);
        XK(13, x); SB5(x); XXROTL(x);
        XK(14, x); SB6(x); XXROTL(x);
        XK(15, x); SB7(x); XXROTL(x);
        XK(16, x); SB0(x); XXROTL(x);
        XK(17, x); SB1(x); XXROTL(x);
        XK(18, x); SB2(x); XXROTL(x);
        XK(19, x); SB3(x); XXROTL(x);
        XK(20, x); SB4(x); XXROTL(x);
        XK(21, x); SB5(x); XXROTL(x);
        XK(22, x); SB6(x); XXROTL(x);
        XK(23, x); SB7(x); XXROTL(x);
        XK(24, x); SB0(x); XXROTL(x);
        XK(25, x); SB1(x); XXROTL(x);
        XK(26, x); SB2(x); XXROTL(x);
        XK(27, x); SB3(x); XXROTL(x);
        XK(28, x); SB4(x); XXROTL(x);
        XK(29, x); SB5(x); XXROTL(x);
        XK(30, x); SB6(x); XXROTL(x);
        XK(31, x); SB7(x); XK(32, x);
        WRITE_LE(dst, x, 4);
    }
    void decrypt(uint8_t const *src, uint8_t *dst) const {
        uint32_t x[5]; // 5th element is used as a temporary
        READ_LE(x, src, 4);
        XK(32, x); IB7(x); XK(31, x);
        XXROTR(x); IB6(x); XK(30, x);
        XXROTR(x); IB5(x); XK(29, x);
        XXROTR(x); IB4(x); XK(28, x);
        XXROTR(x); IB3(x); XK(27, x);
        XXROTR(x); IB2(x); XK(26, x);
        XXROTR(x); IB1(x); XK(25, x);
        XXROTR(x); IB0(x); XK(24, x);
        XXROTR(x); IB7(x); XK(23, x);
        XXROTR(x); IB6(x); XK(22, x);
        XXROTR(x); IB5(x); XK(21, x);
        XXROTR(x); IB4(x); XK(20, x);
        XXROTR(x); IB3(x); XK(19, x);
        XXROTR(x); IB2(x); XK(18, x);
        XXROTR(x); IB1(x); XK(17, x);
        XXROTR(x); IB0(x); XK(16, x);
        XXROTR(x); IB7(x); XK(15, x);
        XXROTR(x); IB6(x); XK(14, x);
        XXROTR(x); IB5(x); XK(13, x);
        XXROTR(x); IB4(x); XK(12, x);
        XXROTR(x); IB3(x); XK(11, x);
        XXROTR(x); IB2(x); XK(10, x);
        XXROTR(x); IB1(x); XK( 9, x);
        XXROTR(x); IB0(x); XK( 8, x);
        XXROTR(x); IB7(x); XK( 7, x);
        XXROTR(x); IB6(x); XK( 6, x);
        XXROTR(x); IB5(x); XK( 5, x);
        XXROTR(x); IB4(x); XK( 4, x);
        XXROTR(x); IB3(x); XK( 3, x);
        XXROTR(x); IB2(x); XK( 2, x);
        XXROTR(x); IB1(x); XK( 1, x);
        XXROTR(x); IB0(x); XK( 0, x);
        WRITE_LE(dst, x, 4);
    }
};
using Serpent128 = SerpentTmpl<4>;
using Serpent192 = SerpentTmpl<6>;
using Serpent256 = SerpentTmpl<8>;
#undef LK
#undef SK
#undef XK
#undef XXROTR
#undef XXROTL
#undef SB0
#undef SB1
#undef SB2
#undef SB3
#undef SB4
#undef SB5
#undef SB6
#undef SB7
#undef IB0
#undef IB1
#undef IB2
#undef IB3
#undef IB4
#undef IB5
#undef IB6
#undef IB7
