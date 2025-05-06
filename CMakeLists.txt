cmake_minimum_required(VERSION 3.10)
project(huffman-coding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/90a41521142c978131f38c6da07b4eb96a9f1ff6.zip
)
FetchContent_MakeAvailable(googletest)

# Main executable
add_executable(huff
    huff.cpp
    huffman/huffman.cpp
)

# Include huffman headers
target_include_directories(huff PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Test executable
add_executable(test_huffman
    huffman/test_huffman.cpp
    huffman/huffman.cpp
)
target_link_libraries(test_huffman PRIVATE gtest gtest_main)
target_include_directories(test_huffman PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
