extern "C" {
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
}

#include <memory>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <fstream>

#include "cmdline.h"
#include "worker.hpp"

int main(int argc, char **argv) {
    cmdline::parser arg;
    arg.add<std::size_t>("blksz", 'b', "block size", true, 4096);
    arg.add<std::size_t>("size", 's', "size", true, 4096);
    arg.add<std::size_t>("range", 0, "access range", false, 0);
    arg.add<std::string>("output", 'o', "output file", false, "output.log");
    arg.add("random", 'r', "random access");

    arg.parse(argc, argv);

    using namespace IOTest;

    IOTestConfig conf;
    conf.block_size = arg.get<std::size_t>("blksz");
    conf.size = arg.get<std::size_t>("size");
    conf.range = arg.get<std::size_t>("range");
    if (conf.range == 0)
        conf.range = conf.size;
    conf.access_type = arg.exist("random") ? AccessType::Random : AccessType::Sequential;

    if (arg.rest().empty()) {
        std::cerr << arg.usage() << std::endl;
        return -1;
    }

    std::cout << "blksz: " << conf.block_size << " B" << std::endl;
    std::cout << "size : " << conf.size << " B" << std::endl;
    std::cout << "range: " << conf.range << " B" << std::endl;
    std::cout << "file : " << arg.rest()[0] << std::endl;
    std::cout << "type : " << (conf.access_type == AccessType::Random ? "Random" : "Sequential") << std::endl;

    conf.fd = open(arg.rest()[0].c_str(), O_DIRECT | O_RDONLY);
    if (conf.fd == -1) {
        std::cerr << "Failed to open " << arg.rest().front() << std::endl;
        std::cerr << strerror(errno) << std::endl;
        return -1;
    }

    PReadWorker worker(conf);

    worker.preread();
    auto ret = worker.run();

    long ns = 0;
    off64_t preoff = 0;
    std::ofstream output(arg.get<std::string>("output"));
    output << "offset,d_offset,ns\n";
    for (auto &r : ret) {
        ns += r.second.count();
        output << r.first << "," << (r.first - preoff) << "," << r.second.count() << "\n";
        preoff = r.first;
    }

    std::cout << "Count: " << ret.size() << std::endl;
    std::cout << "Time: " << ns / 1e9 << " [sec]" << std::endl;
    std::cout << "BW  : " << conf.size * 1000.0 / ns << " [MBps]"
              << std::endl;
    std::cout << "IOPS: " << (conf.size / conf.block_size) * 1000000000 / ns
              << std::endl;


    return 0;
}
