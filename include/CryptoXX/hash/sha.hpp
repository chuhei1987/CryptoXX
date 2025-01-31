#pragma once
#include "../utils.hpp"
#define FF0(x, y, z) ((x) & ((y) ^ (z)) ^ (z))
#define FF1(x, y, z) ((x) ^ (y) ^ (z))
#define FF2(x, y, z) ((x) & (y) | (z) & ((x) | (y)))
#define FF3(x, y, z) ((x) ^ (y) ^ (z))
#define KK0 0x5A827999U
#define KK1 0x6ED9EBA1U
#define KK2 0x8F1BBCDCU
#define KK3 0xCA62C1D6U
#define GG1(N, a, b, c, d, e, w, i) do {                 \
    e = ROTL(a,  5) + FF##N(b, c, d) + e + KK##N + w[i]; \
    b = ROTL(b, 30);                                     \
} while (0)
#define GGX(N, a, b, c, d, e, w, i)                      \
    FOR(j, i, j + 5, j < i + 20, {                       \
        GG1(N, a, b, c, d, e, w, j    );                 \
        GG1(N, e, a, b, c, d, w, j + 1);                 \
        GG1(N, d, e, a, b, c, w, j + 2);                 \
        GG1(N, c, d, e, a, b, w, j + 3);                 \
        GG1(N, b, c, d, e, a, w, j + 4);                 \
    })
class SHA {
    void compress(uint32_t *w) {
        uint32_t a = h[0];
        uint32_t b = h[1];
        uint32_t c = h[2];
        uint32_t d = h[3];
        uint32_t e = h[4];
        uint32_t t;
        FOR(i, 16, i + 1, i < 80, {
            t = w[i - 16] ^ w[i - 14] ^ w[i - 8] ^ w[i - 3];
            w[i] = ROTL(t, 1);
        });
        GGX(0, a, b, c, d, e, w,  0);
        GGX(1, a, b, c, d, e, w, 20);
        GGX(2, a, b, c, d, e, w, 40);
        GGX(3, a, b, c, d, e, w, 60);
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
    }
    uint32_t lo = 0;
    uint32_t hi = 0;
    uint32_t h[5] = {
        0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0,
    };
public:
    static constexpr size_t BLOCK_SIZE = 64;
    static constexpr size_t DIGEST_SIZE = 20;
    static constexpr bool NOT_ALWAYS_PADDING = false;
    void input(uint8_t const *blk) {
        uint32_t w[80];
        READB_BE(w, blk, 64);
        (lo += 64 * 8) < 64 * 8 && ++hi;
        compress(w);
    }
    void final(uint8_t const *src, size_t len, uint8_t *dst) {
        uint32_t w[80];
        memset(w, 0, 64);
        READB_BE(w, src, len);
        (lo += len * 8) < len * 8 && ++hi;
        BYTE_BE(w, len) = 0x80;
        if (len >= 56) {
            compress(w);
            memset(w, 0, 56);
        }
        w[14] = hi;
        w[15] = lo;
        compress(w);
        WRITEB_BE(dst, h, 20);
    }
};
#undef FF0
#undef FF1
#undef FF2
#undef FF3
#undef KK0
#undef KK1
#undef KK2
#undef KK3
#undef GG1
#undef GGX
