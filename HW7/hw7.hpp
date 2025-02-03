#pragma once

#include "BulkCommandHandler.hpp"

namespace hw7 {

int solution(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <block_size>" << std::endl;
        return 1;
    }

    size_t block_size = std::stoul(argv[1]);
    BulkCommandHandler handler(block_size, "/tmp");

    std::string line;
    while (std::getline(std::cin, line)) {
        handler.processLine(line);
    }

    handler.finalize();

    return 0;
}

}  // namespace hw7
