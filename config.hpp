#pragma once

#include <cstddef>

namespace IOTest {

enum AccessType { Random, Sequential };

struct IOTestConfig {
    size_t block_size;  // block size
    size_t size;        // total access size
    size_t range;       // access range (ignored when accessed sequentially)
    enum AccessType access_type;
    int fd;
};


}