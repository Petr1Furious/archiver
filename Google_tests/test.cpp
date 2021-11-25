#include "../archiver.h"
#include "gtest/gtest.h"

#include <random>

uint8_t RandNum(uint8_t l, uint8_t r, std::mt19937& rnd) {
    return l + rnd() % (r - l + 1);
}

TEST(ArchiveUnarchive, ArchiveUnarchiveSmall) {
    std::vector<std::string_view> filenames = {"aa"};

    const std::string_view archive_name = "test_archive";

    std::ofstream test_file(filenames[0].data(), std::ios::binary);
    test_file.close();

    Archive(archive_name, filenames);

    std::remove(filenames[0].data());

    Unarchive(archive_name);
    std::remove(archive_name.data());

    std::ifstream test_file_r(filenames[0].data(), std::ios::binary);

    EXPECT_TRUE(test_file_r.is_open());
    EXPECT_TRUE(test_file_r.peek() == std::ifstream::traits_type::eof());

    std::remove(filenames[0].data());
}

TEST(ArchiveUnarchive, ArchiveUnarchiveBig) {
    const size_t file_size = 100000;
    const size_t files_count = 10;

    std::mt19937 rnd(631);
    std::vector<std::string> filenames_str(files_count);
    std::vector<std::string_view> filenames(files_count);
    const std::string archive_name = "test_archive";

    std::vector<std::vector<uint8_t>> contents(files_count, std::vector<uint8_t>(file_size));
    for (size_t i = 0 ; i < files_count ; i++) {
        filenames_str[i] = "file" + std::to_string(i);
        filenames[i] = filenames_str[i];

        std::ofstream test_file("file" + std::to_string(i), std::ios::binary);
        for (size_t j = 0 ; j < file_size ; j++) {
            contents[i][j] = RandNum(0, 255, rnd);
            test_file.put(contents[i][j]);
        }
        test_file.close();
    }

    Archive(archive_name, filenames);

    for (size_t i = 0 ; i < files_count ; i++) {
        std::remove(filenames[i].data());
    }

    Unarchive(archive_name.c_str());
    std::remove(archive_name.c_str());

    for (size_t i = 0 ; i < files_count ; i++) {
        std::ifstream test_file_r(filenames[i].data(), std::ios::binary);
        EXPECT_TRUE(test_file_r.is_open());

        char* res = new char[file_size];
        test_file_r.read(res, file_size);

        for (size_t j = 0 ; j < file_size ; j++) {
            EXPECT_TRUE(contents[i][j] == static_cast<uint8_t>(res[j]));
        }

        delete[] res;

        std::remove(filenames[i].data());
    }
}

TEST(ArchiveUnarchive, ArchiveUnarchiveLongCodes) {
    std::vector<std::string_view> filenames = {"z"};
    const std::string_view archive_name = "test_archive";
    const int8_t symbols_count = 22;

    std::ofstream test_file(filenames[0].data(), std::ios::binary);
    size_t file_size = 0;
    for (char c = 'a' ; c <= 'a' + symbols_count ; c++) {
        for (uint32_t i = 0 ; i < (1u << (c - 'a')) ; i++) {
            test_file.put(c);
            file_size++;
        }
    }
    test_file.close();

    Archive(archive_name, filenames);

    std::remove(filenames[0].data());

    Unarchive(archive_name);
    std::remove(archive_name.data());

    std::ifstream test_file_r(filenames[0].data(), std::ios::binary);
    EXPECT_TRUE(test_file_r.is_open());

    char* content = new char[file_size];
    test_file_r.read(content, file_size);

    size_t cur_pos = 0;
    for (char c = 'a' ; c < 'a' + symbols_count ; c++) {
        for (uint32_t i = 0 ; i < (1u << (c - 'a')) ; i++) {
            EXPECT_TRUE(content[cur_pos++] == c);
        }
    }

    delete[] content;

    std::remove(filenames[0].data());
}