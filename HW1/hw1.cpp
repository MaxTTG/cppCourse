#include "hw1.hpp"

#include <iostream>

#include "version.hpp"

void hw1::solution() {
    std::cout << "build " << getMajorVersion() << '.' << getMinorVersion() << '.' << getPatchVersion() << std::endl
        << "Hello World!" << std::endl;
}

int hw1::getMajorVersion() {
    return PROJECT_VERSION_MAJOR;
}

int hw1::getMinorVersion() {
    return PROJECT_VERSION_MINOR;
}

int hw1::getPatchVersion() {
    return PROJECT_VERSION_PATCH;
}