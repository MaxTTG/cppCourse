#pragma once

#include <stdint.h>

#include <string>

namespace hw5 {

class Color {
public:
    Color() = default;
    Color(const Color& c);
    Color(uint8_t r, uint8_t g, uint8_t b);

    Color& operator=(const Color& c);

    void setComponents(uint8_t r, uint8_t g, uint8_t b);

    const std::string getComponents() const;

private:
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;
};

}  // namespace hw5
