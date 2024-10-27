#include "Document.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "Tools/Primitives/Line.hpp"
#include "Tools/Primitives/Oval.hpp"
#include "Tools/Primitives/Rectangle.hpp"

namespace hw5 {

Document::Document(const std::string& path) : _path(path) {}

std::string Document::getPath() const {
    return _path;
}

void Document::addPrimitive(std::shared_ptr<IPrimitive> p) {
    _primitives.push_back(p);
}

void Document::removePrimetive(size_t index) {
    if (index >= _primitives.size()) {
        throw std::out_of_range("");
    }

    _primitives.erase(_primitives.begin() + index);
}

void Document::print(std::ostream& out) const {
    if (_primitives.empty()) {
        out << "Документ пуст!" << std::endl;
        return;
    }

    for (const auto& p : _primitives) {
        out << p->getFullInfo() << std::endl;
    }
}

void Document::parse() {
    if (_path.empty()) {
        std::cout << "Путь до файла пуст" << std::endl;
        return;
    }

    std::ifstream fin(_path);
    if (!fin.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        return;
    }

    _primitives.clear();
    std::string str;
    while (std::getline(fin, str)) {
        std::string name;
        int xs, ys, xe, ye, r, g, b;
        char c;
        std::stringstream ss;
        ss << str;
        ss >> name >> xs >> ys >> xe >> ye >> c >> r >> g >> b;
        if (name == "Line") {
            _primitives.push_back(std::make_shared<Line>(xs, ys, xe, ye));
        } else if (name == "Oval") {
            _primitives.push_back(std::make_shared<Oval>(xs, ys, xe, ye));
        } else /* if (name == "Rectangle") */ {
            _primitives.push_back(std::make_shared<Rectangle>(xs, ys, xe, ye));
        }

        (_primitives.end() - 1)->get()->setColor(r, g, b);
    }

    fin.close();
}

void Document::save() {}

}  // namespace hw5
