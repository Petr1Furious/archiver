#include "ostream_wrapper.h"

OStreamWrapper::OStreamWrapper(std::ostream& str) : str_(str), buf_(0), bit_num_(0) {
}

std::ostream& OStreamWrapper::flush() {
    str_.put(static_cast<char>(buf_));
    bit_num_ = 0;
    buf_ = 0;
    return str_;
}

std::ostream& OStreamWrapper::put1b(bool c) {
    buf_ += c << (7 - bit_num_);

    ++bit_num_;
    if (bit_num_ == 8) {
        flush();
    }
    return str_;
}

OStreamWrapper::~OStreamWrapper() {
    flush();
}

std::ostream& OStreamWrapper::write(uint16_t x, uint8_t bits_count) {
    for (uint8_t i = 0; i < bits_count; i++) {
        put1b(x >> i & 1);
    }
    return str_;
}

std::ostream& OStreamWrapper::write(const std::vector<bool>& code) {
    for (bool i: code) {
        put1b(i);
    }
    return str_;
}
