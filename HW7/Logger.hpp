#pragma once

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace hw7 {

class Logger {
public:
    Logger(const std::string& logPath = ".");
    void log(const std::vector<std::string>& commands, std::time_t time);

private:
    std::string _logPath;
};

}  // namespace hw7
