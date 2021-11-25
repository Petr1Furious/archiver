#ifndef ARCHIVER_ISTREAM_WRAPPER_H
#define ARCHIVER_ISTREAM_WRAPPER_H

#include <istream>

class IStreamWrapper {
public:
    explicit IStreamWrapper(std::istream& str);

    bool get1b();

    uint16_t read(uint8_t bits_count);

protected:
    std::istream& str_;

    uint8_t buf_;
    uint8_t bit_num_;
};

#endif  // ARCHIVER_ISTREAM_WRAPPER_H
