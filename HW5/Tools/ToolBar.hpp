#pragma once

#include "IBar.hpp"
#include "Primitives/Line.hpp"
#include "Primitives/Oval.hpp"
#include "Primitives/Rectangle.hpp"

namespace hw5 {

class ToolBar: public IBar {
public:
    ToolBar();

    std::shared_ptr<IPrimitive> select(int index) const override;
};

}  // namespace hw5
