#include "IPrimitive.hpp"

namespace hw5 {

IPrimitive::IPrimitive(int x_begin, int y_begin, int x_end, int y_end)
    : _x_begin(x_begin), _y_begin(y_begin), _x_end(x_end), _y_end(y_end) {}

void IPrimitive::setColor(int r, int g, int b) {
    _color.setComponents(r, g, b);
}

void IPrimitive::setColor(const Color& c) {
    _color = c;
}

void IPrimitive::setCoords(int xs, int ys, int xe, int ye) {
    _x_begin = xs;
    _y_begin = ys;
    _x_end   = xe;
    _y_end   = ye;
}

const std::string IPrimitive::getCoords() const {
    return std::to_string(_x_begin) + ' ' + std::to_string(_y_begin) + ' ' + std::to_string(_x_end) + ' ' +
           std::to_string(_y_end);
}

const std::string IPrimitive::getColor() const {
    return _color.getComponents();
}

const std::string IPrimitive::getFullInfo() const {
    return getName() + ' ' + getCoords() + " | " + getColor();
}

}  // namespace hw5
