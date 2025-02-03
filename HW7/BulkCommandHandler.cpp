#include "BulkCommandHandler.hpp"

namespace hw7 {

hw7::BulkCommandHandler::BulkCommandHandler(size_t staticBlockSize, const std::string& logPath)
    : _staticBlockSize(staticBlockSize), _logger(logPath) {}

void hw7::BulkCommandHandler::processLine(const std::string& line) {
    if (_isDynamic) {
        if (line == "{") {
            startDynamicBlock();
        } else if (line == "}") {
            endDynamicBlock();
        } else {
            if (_dynamicBlock.empty()) {
                _dynamicBlockStartTime = std::time(nullptr);
            }
            _dynamicBlock.push_back(line);
        }
    } else {
        if (line == "{") {
            if (!_staticBlock.empty()) {
                endStaticBlock();
            }
            startDynamicBlock();
        } else {
            if (_staticBlock.empty()) {
                _staticBlockStartTime = std::time(nullptr);
            }
            _staticBlock.push_back(line);
            if (_staticBlock.size() >= _staticBlockSize) {
                endStaticBlock();
            }
        }
    }
}

void hw7::BulkCommandHandler::finalize() {
    if (_isDynamic) {
        // Игнорируем незавершенный динамический блок
        _dynamicBlock.clear();
        _isDynamic = false;
    } else {
        if (!_staticBlock.empty()) {
            endStaticBlock();
        }
    }
}

void hw7::BulkCommandHandler::startDynamicBlock() {
    _isDynamic = true;
    ++_innerDynamic;
    if (_innerDynamic == 1) {
        _dynamicBlock.clear();
        _dynamicBlockStartTime = 0;
    }
}

void hw7::BulkCommandHandler::endDynamicBlock() {
    --_innerDynamic;
    if (_innerDynamic == 0) {
        _isDynamic = false;
        _logger.log(_dynamicBlock, _dynamicBlockStartTime);
        _dynamicBlock.clear();
    }
}

void hw7::BulkCommandHandler::endStaticBlock() {
    _logger.log(_staticBlock, _staticBlockStartTime);
    _staticBlock.clear();
    _staticBlockStartTime = 0;
}

}  // namespace hw7
