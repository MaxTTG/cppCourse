#pragma once

#include <memory>
#include <vector>

#include "Primitives/IPrimitive.hpp"

namespace hw5 {

class IBar {
public:
    IBar();
    void printPrimitivesList();
    size_t size();

    virtual std::shared_ptr<IPrimitive> select(int index) const = 0;

protected:
    std::vector<std::shared_ptr<IPrimitive>> _barTool;
};

}  // namespace hw5
