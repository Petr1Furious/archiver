#include "char_code.h"

#include <algorithm>

uint16_t CharCode::Length() const {
    return code.size();
}

bool CharCode::operator<(const CharCode& other) const {
    return Length() < other.Length() || (Length() == other.Length() && character < other.character);
}

void CharCode::Add2PowK(uint16_t k) {
    size_t first_zero = 0;

    for (size_t j = 0; j < Length() - k; j++) {
        if (!code[Length() - k - 1 - j]) {
            first_zero = j;
            break;
        }
    }

    for (size_t j = 0; j < first_zero; j++) {
        code[Length() - k - 1 - j] = false;
    }
    code[Length() - k - 1 - first_zero] = true;
}

void CalcCanonicalHuffmanCode(std::vector<CharCode>& codes) {
    std::sort(codes.begin(), codes.end());

    for (size_t i = 1; i < codes.size(); i++) {
        std::copy(codes[i - 1].code.begin(), codes[i - 1].code.end(), codes[i].code.begin());

        codes[i].Add2PowK(codes[i].Length() - codes[i - 1].Length());
    }
}
