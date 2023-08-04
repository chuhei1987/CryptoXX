#pragma once
#include "stream.hpp"
#define BLK BlockCipher::BLOCK_SIZE
template <class BlockCipher>
class CTRGen {
    BlockCipher const bc;
    uint8_t ctr[BLK];
public:
    static constexpr size_t SECTION_SIZE = BLK;
    template <class... vals_t>
    CTRGen(uint8_t const *civ, vals_t &&...vals):
        bc(std::forward<vals_t>(vals)...) {
        memcpy(ctr, civ, BLK);
    }
    void generate(uint8_t *buf) {
        bc.encrypt(ctr, buf);
        for (size_t i = BLK - 1; i < BLK && ++ctr[i] == 0; i--) {}
    }
};
template <class BlockCipher>
using CTRCrypter = StreamCipherCrypter<CTRGen<BlockCipher>>;
#undef BLK
