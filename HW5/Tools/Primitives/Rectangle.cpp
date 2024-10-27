#include "Rectangle.hpp"

namespace hw5 {

Rectangle::Rectangle(Rectangle&& rect) : IPrimitive() {
    this->_x_begin = rect._x_begin;
    this->_y_begin = rect._y_begin;
    this->_x_end   = rect._x_end;
    this->_y_end   = rect._y_end;
}

Rectangle::Rectangle(int x_begin, int y_begin, int x_end, int y_end) : IPrimitive(x_begin, y_begin, x_end, y_end) {}

const std::string Rectangle::getName() const {
    return "Rectangle";
}

}  // namespace hw5
