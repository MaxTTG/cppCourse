#pragma once

#include <string>

#include "Color.hpp"

namespace hw5 {

class IPrimitive {
public:
    IPrimitive() = default;
    IPrimitive(int x_begin, int y_begin, int x_end, int y_end);

    void setColor(int r, int g, int b);
    void setColor(const Color& c);
    void setCoords(int xs, int ys, int xe, int ye);

    virtual const std::string getName() const = 0;
    const std::string getCoords() const;
    const std::string getColor() const;
    const std::string getFullInfo() const;

protected:
    int _x_begin;
    int _y_begin;
    int _x_end;
    int _y_end;
    Color _color;
};

}  // namespace hw5
