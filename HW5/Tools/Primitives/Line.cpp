#include "Line.hpp"

namespace hw5 {

Line::Line(Line&& line) : IPrimitive() {
    this->_x_begin = line._x_begin;
    this->_y_begin = line._y_begin;
    this->_x_end   = line._x_end;
    this->_y_end   = line._y_end;
}

Line::Line(int x_begin, int y_begin, int x_end, int y_end) : IPrimitive(x_begin, y_begin, x_end, y_end) {}

const std::string Line::getName() const {
    return "Line";
}

}  // namespace hw5
