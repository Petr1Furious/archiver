cmake_minimum_required(VERSION 3.20)
project(archiver)

set(CMAKE_CXX_STANDARD 17)

add_executable(archiver
        main.cpp
        ostream_wrapper.cpp
        ostream_wrapper.h
        archiver.cpp
        archiver.h
        binary_trie.cpp
        binary_trie.h
        char_code.cpp
        char_code.h
        istream_wrapper.cpp
        istream_wrapper.h)

add_subdirectory(Google_tests)
