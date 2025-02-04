#pragma once

#include <boost/filesystem.hpp>
#include <cstdint>

namespace hw8 {

// Структура для хранения информации о файле
struct FileInfo {
    boost::filesystem::path path;
    uint64_t size;
};

}  // namespace hw8
