#include "Color.hpp"

namespace hw5 {

Color::Color(uint8_t r, uint8_t g, uint8_t b) : _red(r), _green(g), _blue(b) {}

Color::Color(const Color& c) : _red(c._red), _green(c._green), _blue(c._blue) {}

void Color::setComponents(uint8_t r, uint8_t g, uint8_t b) {
    _red   = r;
    _green = g;
    _blue  = b;
}

Color& Color::operator=(const Color& c) {
    this->_red   = c._red;
    this->_green = c._green;
    this->_blue  = c._blue;
    return *this;
}

const std::string Color::getComponents() const {
    return std::to_string(_red) + ' ' + std::to_string(_green) + ' ' + std::to_string(_blue);
}

}  // namespace hw5
