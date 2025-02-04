#include "FileReader.hpp"

namespace hw8 {

hw8::FileReader::FileReader(const boost::filesystem::path &p, uint64_t s) : path(p), size(s) {
    stream.open(p.string(), std::ios::binary);
    if (!stream) {
        throw std::runtime_error("Не удалось открыть файл: " + p.string());
    }
}

}  // namespace hw8
