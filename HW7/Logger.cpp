#include "Logger.hpp"

namespace hw7 {

Logger::Logger(const std::string& logPath) : _logPath(logPath) {}

void Logger::log(const std::vector<std::string>& commands, time_t time) {
    if (commands.empty()) {
        return;
    }

    /// Вывод в консоль
    std::cout << "bulk: ";
    for (size_t i = 0; i < commands.size(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }

        std::cout << commands[i];
    }

    std::cout << std::endl;

    /// Запись в файл
    std::string filename = _logPath + "/bulk" + std::to_string(time) + ".log";
    std::ofstream file(filename);
    if (file) {
        for (const auto& cmd : commands) {
            file << cmd << '\n';
        }
    } else {
        std::cerr << "Cannot create " << filename << std::endl;
    }
}

}  // namespace hw7
