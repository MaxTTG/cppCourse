#include "Oval.hpp"

namespace hw5 {

Oval::Oval(Oval&& oval) : IPrimitive() {
    this->_x_begin = oval._x_begin;
    this->_y_begin = oval._y_begin;
    this->_x_end   = oval._x_end;
    this->_y_end   = oval._y_end;
}

Oval::Oval(int x_begin, int y_begin, int x_end, int y_end) : IPrimitive(x_begin, y_begin, x_end, y_end) {}

const std::string Oval::getName() const {
    return "Oval";
}

}  // namespace hw5
