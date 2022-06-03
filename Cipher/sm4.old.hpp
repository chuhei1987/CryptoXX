#pragma once
#include "block.hpp"
#define ROL32(x, n) ((x) << (n) | (x) >> (32 - (n)))
#define I2ARR(i, a) {           \
	(a)[0] = (i) >> 030;        \
	(a)[1] = (i) >> 020 & 0xff; \
	(a)[2] = (i) >> 010 & 0xff; \
	(a)[3] = (i) & 0xff;        \
}
class SM4 : public BlockCipher<16> {
	static constexpr uint32_t FK[4] = {
		0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc,
	};
	static constexpr uint32_t CK[32] = {
		0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
		0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
		0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
		0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
		0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
		0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
		0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
		0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279,
	};
	static constexpr uint8_t S_box[256] = {
		0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
		0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
		0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
		0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
		0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
		0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
		0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
		0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
		0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
		0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
		0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
		0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
		0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
		0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
		0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
		0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48,
	};
	static uint32_t tau(uint32_t const &a) {
		uint32_t b;
		uint8_t *A = (uint8_t *)&a, *B = (uint8_t *)&b;
		B[0] = S_box[A[0]];
		B[1] = S_box[A[1]];
		B[2] = S_box[A[2]];
		B[3] = S_box[A[3]];
		return b;
	}
	uint32_t rk[32];
public:
	SM4(uint8_t const *const &mk) {
		uint32_t t[36] = {
			(uint32_t)(mk[0x0] << 030 | mk[0x1] << 020 | mk[0x2] << 010 | mk[0x3]) ^ FK[0],
			(uint32_t)(mk[0x4] << 030 | mk[0x5] << 020 | mk[0x6] << 010 | mk[0x7]) ^ FK[1],
			(uint32_t)(mk[0x8] << 030 | mk[0x9] << 020 | mk[0xa] << 010 | mk[0xb]) ^ FK[2],
			(uint32_t)(mk[0xc] << 030 | mk[0xd] << 020 | mk[0xe] << 010 | mk[0xf]) ^ FK[3],
		};
		for (int i = 0; i < 32; i++) {
			uint32_t b = tau(t[i + 1] ^ t[i + 2] ^ t[i + 3] ^ CK[i]);
			rk[i] = t[i + 4] = t[i] ^ b ^ ROL32(b, 13) ^ ROL32(b, 23);
		}
	}
	void encrypt(uint8_t const *const &src, uint8_t *const &dst) const {
		uint32_t t[36] = {
			(uint32_t)(src[0x0] << 030 | src[0x1] << 020 | src[0x2] << 010 | src[0x3]),
			(uint32_t)(src[0x4] << 030 | src[0x5] << 020 | src[0x6] << 010 | src[0x7]),
			(uint32_t)(src[0x8] << 030 | src[0x9] << 020 | src[0xa] << 010 | src[0xb]),
			(uint32_t)(src[0xc] << 030 | src[0xd] << 020 | src[0xe] << 010 | src[0xf]),
		};
		for (int i = 0; i < 32; i++) {
			uint32_t b = tau(t[i + 1] ^ t[i + 2] ^ t[i + 3] ^ rk[i]);
			t[i + 4] = t[i] ^ b ^ ROL32(b, 2) ^ ROL32(b, 10) ^ ROL32(b, 18) ^ ROL32(b, 24);
		}
		I2ARR(t[35], ((uint8_t(*)[4])dst)[0]);
		I2ARR(t[34], ((uint8_t(*)[4])dst)[1]);
		I2ARR(t[33], ((uint8_t(*)[4])dst)[2]);
		I2ARR(t[32], ((uint8_t(*)[4])dst)[3]);
	}
	void decrypt(uint8_t const *const &src, uint8_t *const &dst) const {
		uint32_t t[36] = {
			(uint32_t)(src[0x0] << 030 | src[0x1] << 020 | src[0x2] << 010 | src[0x3]),
			(uint32_t)(src[0x4] << 030 | src[0x5] << 020 | src[0x6] << 010 | src[0x7]),
			(uint32_t)(src[0x8] << 030 | src[0x9] << 020 | src[0xa] << 010 | src[0xb]),
			(uint32_t)(src[0xc] << 030 | src[0xd] << 020 | src[0xe] << 010 | src[0xf]),
		};
		for (int i = 0; i < 32; i++) {
			uint32_t b = tau(t[i + 1] ^ t[i + 2] ^ t[i + 3] ^ rk[31 - i]);
			t[i + 4] = t[i] ^ b ^ ROL32(b, 2) ^ ROL32(b, 10) ^ ROL32(b, 18) ^ ROL32(b, 24);
		}
		I2ARR(t[35], ((uint8_t(*)[4])dst)[0]);
		I2ARR(t[34], ((uint8_t(*)[4])dst)[1]);
		I2ARR(t[33], ((uint8_t(*)[4])dst)[2]);
		I2ARR(t[32], ((uint8_t(*)[4])dst)[3]);
	}
};
