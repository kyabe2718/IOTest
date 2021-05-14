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

set(REMOTE_HOST "kugenuma19")
set(REMOTE_DIR "work/io_test")

add_custom_target(sync
        COMMAND rsync -av --exclude="*build*" --exclude=".idea" ${CMAKE_CURRENT_LIST_DIR}/ ${REMOTE_HOST}:${REMOTE_DIR}
        DEPENDS io_test
        )
