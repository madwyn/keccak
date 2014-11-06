cmake_minimum_required(VERSION 2.8)
project(keccak)

# set source files
set(keccak_SRCS
    keccak.c
)

# set output
set(LIBRARY_OUTPUT_PATH ${keccak_BINARY_DIR}/lib)

# set lib
add_library(keccak STATIC ${keccak_SRCS})

target_include_directories(keccak PUBLIC "${KECCAK_ROOT_DIR}")

