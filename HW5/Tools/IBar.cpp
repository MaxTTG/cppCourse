#include "IBar.hpp"

#include <iostream>

namespace hw5 {

IBar::IBar() {}

void IBar::printPrimitivesList() {
    for (size_t i = 0; i < _barTool.size(); ++i) {
        std::cout << i + 1 << ") " << _barTool[i]->getName() << std::endl;
    }
}

size_t IBar::size() {
    return _barTool.size();
}

}  // namespace hw5
