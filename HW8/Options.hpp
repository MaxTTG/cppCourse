#pragma once

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <set>
#include <string>
#include <vector>

namespace hw8 {

// Структура для хранения параметров командной строки.
struct Options {
    std::vector<std::string> scanDirs;
    std::set<boost::filesystem::path> excludeDirs;
    int depth;
    uint64_t minSize;
    std::vector<boost::regex> masks;
    size_t blockSize;
};

}  // namespace hw8
