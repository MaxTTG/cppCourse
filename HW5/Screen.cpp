#include "Screen.hpp"

#include <iostream>

namespace hw5 {

Screen::Screen() {}

void Screen::printDocument(Document* doc, std::ostream& out) {
    if (!doc) {
        std::cout << "Не выбран документ!" << std::endl;
        return;
    }

    doc->print(out);
}

}  // namespace hw5
