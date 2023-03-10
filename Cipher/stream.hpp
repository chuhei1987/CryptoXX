#pragma once
#include <stdint.h>
#define SCS sizeof(typename SC::block_t)
template <size_t bls>
class StreamCipher {
public:
	using block_t = uint8_t[bls];
	virtual ~StreamCipher() = default;
	virtual void generate(uint8_t *const &dst) = 0;
};
class StreamCipherFlow {
public:
	virtual ~StreamCipherFlow() = default;
	virtual uint8_t *update(uint8_t const *src, uint8_t const *const &end, uint8_t *dst) = 0;
};
template <class SC>
class Crypter: public StreamCipherFlow {
	SC sc;
	size_t use;
	typename SC::block_t mem;
public:
	using sc_t = SC;
	template <class... vals_t>
	Crypter(vals_t const &...vals):
		sc(vals...), use(SCS) {}
	uint8_t *update(uint8_t const *src, uint8_t const *const &end, uint8_t *dst) {
		while (SCS + src < end + use) {
			while (use < SCS) {
				*dst++ = *src++ ^ mem[use++];
			}
			sc.generate(mem);
			use -= SCS;
		}
		while (src < end) {
			*dst++ = *src++ ^ mem[use++];
		}
		return dst;
	}
};
