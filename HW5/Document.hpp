#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Tools/Primitives/IPrimitive.hpp"

namespace hw5 {

class Document {
public:
    Document(const std::string& path);

    std::string getPath() const;

    void addPrimitive(std::shared_ptr<IPrimitive> p);
    void removePrimetive(size_t index);

    void print(std::ostream& out = std::cout) const;

    void parse();
    void save();

private:
    std::string _path;
    std::vector<std::shared_ptr<IPrimitive>> _primitives;
};

}  // namespace hw5
