project(Google_tests)

add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run
        test.cpp
        ../ostream_wrapper.cpp
        ../ostream_wrapper.h
        ../archiver.cpp
        ../archiver.h
        ../binary_trie.cpp
        ../binary_trie.h
        ../char_code.cpp
        ../char_code.h
        ../istream_wrapper.cpp
        ../istream_wrapper.h)

target_link_libraries(Google_Tests_run gtest gtest_main)
target_link_libraries(archiver gtest gtest_main)
