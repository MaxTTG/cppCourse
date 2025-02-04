#pragma once

#include <boost/filesystem.hpp>
#include <vector>

namespace hw8 {

// Структура для чтения файла по блокам с кешированием прочитанных блоков
struct FileReader {
    boost::filesystem::path path;
    uint64_t size;
    std::ifstream stream;

    // Кэш прочитанных блоков: blockHashes[i] содержит хэш i-го блока
    std::vector<uint32_t> blockHashes;

    FileReader(const boost::filesystem::path &p, uint64_t s) : path(p), size(s) {
        stream.open(p.string(), std::ios::binary);
        if (!stream) {
            throw std::runtime_error("Не удалось открыть файл: " + p.string());
        }
    }
};

}  // namespace hw8
