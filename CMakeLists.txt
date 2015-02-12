cmake_minimum_required(VERSION 2.8)
project(keccak)

# set source files
set(keccak_SRCS
    keccak.c
)

# set output
set(LIBRARY_OUTPUT_PATH ${keccak_BINARY_DIR}/lib)

# set lib
add_library(keccak
    ${keccak_SRCS}
)

# Install
install(TARGETS ${INS_TARGETS}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
