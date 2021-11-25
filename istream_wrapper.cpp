#include "istream_wrapper.h"

IStreamWrapper::IStreamWrapper(std::istream& str) : str_(str), buf_(0), bit_num_(8) {
}

bool IStreamWrapper::get1b() {
    if (bit_num_ == 8) {
        bit_num_ = 0;
        buf_ = str_.get();
    }
    return (buf_ >> (7 - bit_num_++)) & 1;
}

uint16_t IStreamWrapper::read(uint8_t bits_count) {
    uint16_t res = 0;
    for (uint8_t i = 0; i < bits_count; i++) {
        bool cur = get1b();
        res += cur * (1 << i);
    }
    return res;
}
