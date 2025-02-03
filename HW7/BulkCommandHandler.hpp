#pragma once

#include "Logger.hpp"

namespace hw7 {

class BulkCommandHandler {
public:
    BulkCommandHandler(size_t staticBlockSize, const std::string& logPath = ".");
    void processLine(const std::string& line);
    void finalize();

private:
    void startDynamicBlock();
    void endDynamicBlock();
    void endStaticBlock();

private:
    size_t _staticBlockSize;
    Logger _logger;

    std::time_t _staticBlockStartTime  = 0;
    std::time_t _dynamicBlockStartTime = 0;

    bool _isDynamic = false;
    std::vector<std::string> _staticBlock;
    std::vector<std::string> _dynamicBlock;

    size_t _innerDynamic = 0;
};

}  // namespace hw7
