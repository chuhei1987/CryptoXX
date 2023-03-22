#pragma once
#include "stream.hpp"
#define ROL32EQ(v, n) ((v) = (v) << (n) | (v) >> (32 - (n)))
#define QTR_RND(s, a, b, c, d) {                   \
	s[a] += s[b], s[d] ^= s[a], ROL32EQ(s[d], 16); \
	s[c] += s[d], s[b] ^= s[c], ROL32EQ(s[b], 12); \
	s[a] += s[b], s[d] ^= s[a], ROL32EQ(s[d],  8); \
	s[c] += s[d], s[b] ^= s[c], ROL32EQ(s[b],  7); \
}
class ChaCha20 {
	uint32_t input[16];
public:
	static constexpr size_t SECTION_SIZE = 64;
	ChaCha20(uint32_t const *key, uint32_t const *counter):
		input{
			0x61707865, 0x3320646e, 0x79622d32, 0x6b206574, // "expand 32-byte k"
			key    [0], key    [1], key    [2], key    [3],
			key    [4], key    [5], key    [6], key    [7],
			counter[0], counter[1], counter[2], counter[3],
		} {}
	void generate(uint8_t *dst) {
		uint32_t state[16] = {
			input[0x0], input[0x1], input[0x2], input[0x3],
			input[0x4], input[0x5], input[0x6], input[0x7],
			input[0x8], input[0x9], input[0xa], input[0xb],
			input[0xc], input[0xd], input[0xe], input[0xf],
		};
		for (int i = 0; i < 10; i++) {
			QTR_RND(state,  0,  4,  8, 12);
			QTR_RND(state,  1,  5,  9, 13);
			QTR_RND(state,  2,  6, 10, 14);
			QTR_RND(state,  3,  7, 11, 15);
			QTR_RND(state,  0,  5, 10, 15);
			QTR_RND(state,  1,  6, 11, 12);
			QTR_RND(state,  2,  7,  8, 13);
			QTR_RND(state,  3,  4,  9, 14);
		}
		for (int i = 0; i < 16; i++) {
			((uint32_t *)dst)[i] = state[i] + input[i];
		}
		for (int i = 12; i < 16 && ++input[i] == 0; i++) {} // increment counter
	}
};
