#ifndef ARCHIVER_CHAR_CODE_H
#define ARCHIVER_CHAR_CODE_H

#include <algorithm>
#include <cstdint>
#include <vector>

class CharCode {
public:
    uint16_t character;
    std::vector<bool> code;

public:
    uint16_t Length() const;

    bool operator<(const CharCode& other) const;

    void Reverse() {
        std::reverse(code.begin(), code.end());
    }

    void Add2PowK(uint16_t k);
};

void CalcCanonicalHuffmanCode(std::vector<CharCode>& codes);

#endif  // ARCHIVER_CHAR_CODE_H
