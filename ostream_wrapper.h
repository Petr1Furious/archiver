#ifndef ARCHIVER_OSTREAM_WRAPPER_H
#define ARCHIVER_OSTREAM_WRAPPER_H

#include <ostream>
#include <vector>

class OStreamWrapper {
public:
    explicit OStreamWrapper(std::ostream& str);

    std::ostream& flush();

    std::ostream& put1b(bool c);

    std::ostream& write(uint16_t x, uint8_t bits_count);

    std::ostream& write(const std::vector<bool>& code);

    ~OStreamWrapper();

protected:
    std::ostream& str_;

    uint8_t buf_;
    uint8_t bit_num_;
};


#endif  // ARCHIVER_OSTREAM_WRAPPER_H
