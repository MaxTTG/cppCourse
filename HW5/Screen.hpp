#pragma once

#include <memory>

#include "Document.hpp"

namespace hw5 {

class Screen {
public:
    Screen();

    void printDocument(Document* doc, std::ostream& out = std::cout);
};

}  // namespace hw5
