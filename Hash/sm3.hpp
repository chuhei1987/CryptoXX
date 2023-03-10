#pragma once
#include <array>
#include "hash.hpp"
#define ROL32(x, n) ((x) << (n) | (x) >> (32 - (n)))
#define PP0(x) ((x) ^ ROL32(x,  9) ^ ROL32(x, 17))
#define PP1(x) ((x) ^ ROL32(x, 15) ^ ROL32(x, 23))
#define FF0(x, y, z) ((x) ^ (y) ^ (z))
#define FF1(x, y, z) ((x) & (y) | (x) & (z) | (y) & (z))
#define GG0(x, y, z) ((x) ^ (y) ^ (z))
#define GG1(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define HHN(A, B, C, D, E, F, G, H, W, K, N, X, Y)                           \
	for (int j = X; j < Y; j++) {                                            \
		uint32_t A12 = ROL32(A, 12);                                         \
		uint32_t AEK = A12 + E + K[j];                                       \
		uint32_t SS1 = ROL32(AEK, 7);                                        \
		uint32_t TT1 = FF##N(A, B, C) + D + (SS1 ^ A12) + (W[j] ^ W[j + 4]); \
		uint32_t TT2 = GG##N(E, F, G) + H +  SS1        +  W[j]            ; \
		D = C;                                                               \
		C = ROL32(B,  9);                                                    \
		B = A;                                                               \
		A = TT1;                                                             \
		H = G;                                                               \
		G = ROL32(F, 19);                                                    \
		F = E;                                                               \
		E = PP0(TT2);                                                        \
	}
class SM3: public Hash<64, 32> {
	static constexpr auto K = [](uint32_t const &TT00, uint32_t const &TT10) {
		std::array<uint32_t, 64> K = {};
		for (int j =  0; j < 16; j++) {
			K[j] = TT00 << j % 32 | TT00 >> (32 - j) % 32;
		}
		for (int j = 16; j < 64; j++) {
			K[j] = TT10 << j % 32 | TT10 >> (96 - j) % 32;
		}
		return K;
	}(0x79cc4519, 0x7a879d8a);
	static void compress(uint32_t *const &rec, uint8_t const *const &blk) {
		uint32_t A = rec[0];
		uint32_t B = rec[1];
		uint32_t C = rec[2];
		uint32_t D = rec[3];
		uint32_t E = rec[4];
		uint32_t F = rec[5];
		uint32_t G = rec[6];
		uint32_t H = rec[7];
		uint32_t W[68], TMP;
		for (int j =  0; j < 16; j++) {
			W[j] = blk[j << 2] << 030 | blk[j << 2 | 1] << 020 | blk[j << 2 | 2] << 010 | blk[j << 2 | 3];
		}
		for (int j = 16; j < 68; j++) {
			TMP = W[j - 16] ^ W[j - 9] ^ ROL32(W[j -  3], 15);
			W[j] = PP1(TMP) ^ W[j - 6] ^ ROL32(W[j - 13],  7);
		}
		HHN(A, B, C, D, E, F, G, H, W, K, 0,  0, 16);
		HHN(A, B, C, D, E, F, G, H, W, K, 1, 16, 64);
		rec[0] ^= A;
		rec[1] ^= B;
		rec[2] ^= C;
		rec[3] ^= D;
		rec[4] ^= E;
		rec[5] ^= F;
		rec[6] ^= G;
		rec[7] ^= H;
	}
	uint64_t countr = 0;
	uint32_t rec[8] = {
		0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600,
		0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E,
	};
public:
	void block(uint8_t const *const &src) {
		compress(rec, src);
		countr += 512;
	}
	void final(uint8_t const *const &src, size_t const &len, uint8_t *const &dst) const {
		uint8_t tmp[64];
		memcpy(tmp, src, len);
		memset(tmp + len, 0, 64 - len);
		tmp[len] = 0x80;
		uint32_t rectmp[8];
		memcpy(rectmp, rec, 32);
		uint64_t ctrtmp = countr + 8 * len;
		uint8_t *u8ctmp = (uint8_t *)&ctrtmp;
		if (len >= 56) {
			compress(rectmp, tmp);
			memset(tmp, 0, 56);
		}
		tmp[63] = u8ctmp[0];
		tmp[62] = u8ctmp[1];
		tmp[61] = u8ctmp[2];
		tmp[60] = u8ctmp[3];
		tmp[59] = u8ctmp[4];
		tmp[58] = u8ctmp[5];
		tmp[57] = u8ctmp[6];
		tmp[56] = u8ctmp[7];
		compress(rectmp, tmp);
		for (int j = 0; j < 8; j++) {
			dst[j << 2    ] = rectmp[j] >> 24;
			dst[j << 2 | 1] = rectmp[j] >> 16;
			dst[j << 2 | 2] = rectmp[j] >>  8;
			dst[j << 2 | 3] = rectmp[j]      ;
		}
	}
};
#undef PP0
#undef PP1
#undef FF0
#undef FF1
#undef GG0
#undef GG1
#undef HHN
