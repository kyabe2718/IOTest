#pragma once

#include "config.hpp"

#include <chrono>
#include <utility>
#include <vector>
#include <sys/types.h>

namespace IOTest {

struct PReadWorker {
    using Res = std::vector<std::pair<off64_t, std::chrono::nanoseconds>>;

    explicit PReadWorker(const IOTestConfig &conf)
            : conf(conf), start_offset(0) {}

    void preread();
    Res run();

private:
    IOTestConfig conf;
    off64_t start_offset;
    Res result;
};
}  // namespace IOTest
