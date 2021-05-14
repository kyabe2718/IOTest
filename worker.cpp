#include "worker.hpp"
#include "time.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <memory>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace IOTest {

void PReadWorker::preread() {

    struct stat st;
    if (fstat(conf.fd, &st) != 0)
        throw std::runtime_error(strerror(errno));

    std::size_t bs = 4 * 1024 * 1024; // 4MiB
    std::size_t preread_size = 8ul * 1024 * 1024 * 1024; // 8GiB

    start_offset = 10ul * 1024 * 1024 * 1024; // 10GiB

    std::unique_ptr<char, void (*)(void *)>
            buf((char *) valloc(bs), [](void *p) { free(p); });

    for (off64_t off = 0; off < preread_size; off += bs) {
        auto ret = pread(conf.fd, buf.get(), bs, off);
        if (ret != bs)
            throw std::runtime_error(strerror(errno));

        std::cout << "\rpreread: " << (100 * off / preread_size) << "%" << std::ends;
    }
    std::cout << "\rpreread: 100%" << std::endl;
}

PReadWorker::Res PReadWorker::run() {
    size_t count = conf.size / conf.block_size;
    size_t blk_num = conf.range / conf.block_size;
    std::unique_ptr<char, void (*)(void *)>
            buf((char *) valloc(conf.block_size), [](void *p) { free(p); });

    off64_t off = start_offset;
    for (size_t i = 0; i < count; ++i) {
        ssize_t ret;
        auto ns = Timer{}.run<std::chrono::nanoseconds>(
                [&] { ret = pread(conf.fd, buf.get(), conf.block_size, off); });

        if (ret != conf.block_size)
            throw std::runtime_error(strerror(errno));

        result.emplace_back(off, ns);

        switch (conf.access_type) {
            case Sequential:
                off += conf.block_size;
                break;
            case Random:
                off = start_offset + (off64_t) ((lrand48() % blk_num) * conf.block_size);
                break;
            default:
                throw std::runtime_error("Unknown Access Type");
        }

        std::cout << "\rprocess: " << (100 * i / count) << "%" << std::ends;
    }
    std::cout << "\rprocess: 100%" << std::endl;

    return result;
}
}  // namespace IOTest
