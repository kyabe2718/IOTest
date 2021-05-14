#include "time.hpp"

#include <ctime>
#include <cerrno>
#include <cstring>

namespace {
timespec time_diff(struct timespec t2, struct timespec t1) {
    timespec diff;
    if (t2.tv_nsec - t1.tv_nsec < 0) {
        diff.tv_sec = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return diff;
}
}  // namespace

#include <stdexcept>

namespace IOTest {
long Timer::runImpl(const std::function<void()> &func) {
    timespec st, ed;
    if (clock_gettime(CLOCK_MONOTONIC, &st) == -1)
        throw std::runtime_error(strerror(errno));

    func();

    if (clock_gettime(CLOCK_MONOTONIC, &ed) == -1)
        throw std::runtime_error(strerror(errno));

    auto diff = time_diff(ed, st);
    return diff.tv_nsec + diff.tv_sec * 1000000000;
}
}  // namespace IOTest
