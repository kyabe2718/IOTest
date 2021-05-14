#pragma once

#include <chrono>
#include <functional>

namespace IOTest {
struct Timer {
    template<class Duration>
    Duration run(const std::function<void()> &func) {
        auto time = std::chrono::nanoseconds(runImpl(func));
        return std::chrono::duration_cast<Duration>(time);
    }

private:
    long runImpl(const std::function<void()> &func);
};
}  // namespace IOTest

