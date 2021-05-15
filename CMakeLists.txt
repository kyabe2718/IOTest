cmake_minimum_required(VERSION 3.16)
project(io_test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_executable(io_test
        time.cpp
        worker.cpp
        main.cpp
        )

target_compile_options(io_test PUBLIC
        -D_GNU_SOURCE
        -D_FILE_OFFSET_BITS=64
        )

target_link_libraries(io_test pthread)

