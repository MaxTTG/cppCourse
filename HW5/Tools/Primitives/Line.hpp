#pragma once

#include "IPrimitive.hpp"

namespace hw5 {

class Line: public IPrimitive {
public:
    Line(Line&& line);
    Line(int x_begin, int y_begin, int x_end, int y_end);

    const std::string getName() const override final;
};

}  // namespace hw5
