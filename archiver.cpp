#include <iostream>
#include "ostream_wrapper.h"
#include "istream_wrapper.h"
#include "binary_trie.h"
#include "archiver.h"

static const uint16_t FILENAME_END = 256;
static const uint16_t ONE_MORE_FILE = 257;
static const uint16_t ARCHIVE_END = 258;
static const uint8_t BITS_COUNT = 9;

void CalcCharCount(size_t char_count[ARCHIVE_END + 1], const std::string_view& filename, std::ifstream& file) {
    for (char c: filename) {
        char_count[static_cast<uint8_t>(c)]++;
    }

    char cur_char;
    while (file.get(cur_char)) {
        char_count[static_cast<uint8_t>(cur_char)]++;
    }

    char_count[FILENAME_END]++;
    char_count[ONE_MORE_FILE]++;
    char_count[ARCHIVE_END]++;

    file.clear();
    file.seekg(0);
}

void PrintToFile(const std::string_view& filename, std::ifstream& file, OStreamWrapper& ostream,
                 CharCode* code_by_char) {
    for (char c: filename) {
        ostream.write(code_by_char[static_cast<uint8_t>(c)].code);
    }

    ostream.write(code_by_char[FILENAME_END].code);

    char cur_char;
    while (file.get(cur_char)) {
        ostream.write(code_by_char[static_cast<uint8_t>(cur_char)].code);
    }
}

void Archive(const std::string_view& archive_name, const std::vector<std::string_view>& filenames) {
    std::ofstream archive_file(archive_name.data());

    if (!archive_file.is_open()) {
        throw std::ios_base::failure("Failed to create the archive file");
    }

    OStreamWrapper ostream(archive_file);

    for (size_t file_num = 0; file_num < filenames.size(); file_num++) {
        std::ifstream file(filenames[file_num].data(), std::ios::binary);

        if (!file.is_open()) {
            std::remove(archive_name.data());
            throw std::ios_base::failure("Failed to locate the file \"" +
                                         static_cast<std::string>(filenames[file_num]) + "\"");
        }

        size_t char_count[ARCHIVE_END + 1]{};
        CalcCharCount(char_count, filenames[file_num], file);

        BinaryTrie trie;
        trie.BuildByCharCounts(char_count, ARCHIVE_END + 1);

        std::vector<CharCode> codes;
        trie.GetCodesLen(codes);

        CalcCanonicalHuffmanCode(codes);

        ostream.write(codes.size(), BITS_COUNT);

        CharCode code_by_char[ARCHIVE_END + 1];

        if (!codes.empty()) {
            auto* char_len_count = new uint64_t[codes[codes.size() - 1].Length()]();

            for (const CharCode& code: codes) {
                ostream.write(code.character, BITS_COUNT);
                char_len_count[code.Length() - 1]++;
                code_by_char[code.character] = code;
            }

            for (size_t i = 0; i < codes[codes.size() - 1].Length(); i++) {
                ostream.write(char_len_count[i], BITS_COUNT);
            }
        }

        PrintToFile(filenames[file_num], file, ostream, code_by_char);
        if (file_num + 1 != filenames.size()) {
            ostream.write(code_by_char[ONE_MORE_FILE].code);
        } else {
            ostream.write(code_by_char[ARCHIVE_END].code);
        }
    }
}

void Unarchive(const std::string_view& archive_name) {
    std::ifstream archive_file(archive_name.data(), std::ios::binary);

    if (!archive_file.is_open()) {
        throw std::ios_base::failure("Failed to locate the archive file");
    }

    IStreamWrapper istream(archive_file);
    bool one_more_file = true;
    while (one_more_file) {
        one_more_file = false;
        uint16_t symbols_count = istream.read(BITS_COUNT);

        std::vector<CharCode> codes(symbols_count);
        for (CharCode& code: codes) {
            code.character = istream.read(BITS_COUNT);
        }

        size_t total_count = 0;
        uint8_t cur_len = 0;
        while (total_count != codes.size()) {
            ++cur_len;
            uint16_t cur_count = istream.read(BITS_COUNT);
            for (size_t i = total_count; i < total_count + cur_count; i++) {
                codes[i].code.resize(cur_len);
            }
            total_count += cur_count;
        }

        CalcCanonicalHuffmanCode(codes);

        BinaryTrie trie;
        trie.BuildByHuffmanCodes(codes);

        std::string filename;
        bool reading_filename = true;
        std::ofstream ostream;

        std::vector<bool> reversed_code;
        uint16_t cur_char;
        while (true) {
            const BinaryTrie::TrieNode* cur_node = trie.GetRoot();

            reversed_code.clear();
            while (cur_node->nodes[0] != nullptr) {
                bool cur_bit = istream.get1b();
                BinaryTrie::MoveNodeByBit(cur_node, cur_bit);
                reversed_code.push_back(cur_bit);
            }

            cur_char = trie.GetChar(reversed_code);

            if (cur_char == ONE_MORE_FILE || cur_char == ARCHIVE_END) {
                break;
            }

            if (cur_char == FILENAME_END) {
                reading_filename = false;
                ostream.open(filename.c_str(), std::ios::binary);

                if (!ostream.is_open()) {
                    throw std::ios_base::failure("Failed to create a file");
                }
            } else {
                if (reading_filename) {
                    filename += static_cast<char>(cur_char);
                } else {
                    ostream.put(static_cast<char>(cur_char));
                }
            }
        }

        if (cur_char == ONE_MORE_FILE) {
            one_more_file = true;
        }
    }
}
