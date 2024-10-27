#include "ToolBar.hpp"

namespace hw5 {

ToolBar::ToolBar() {
    _barTool.push_back(std::make_unique<Line>(0, 0, 0, 0));
    _barTool.push_back(std::make_unique<Oval>(0, 0, 0, 0));
    _barTool.push_back(std::make_unique<Rectangle>(0, 0, 0, 0));
}

std::shared_ptr<IPrimitive> ToolBar::select(int index) const {
    auto name = _barTool.at(index)->getName();

    if (name == "Line") {
        return std::make_shared<Line>(0, 0, 0, 0);
    } else if (name == "Oval") {
        return std::make_shared<Oval>(0, 0, 0, 0);
    } else /* if (name == "Rectangle") */ {
        return std::make_shared<Rectangle>(0, 0, 0, 0);
    }
}

}  // namespace hw5
