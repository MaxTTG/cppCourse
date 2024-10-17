#include "hw3.hpp"

#include <iostream>
#include <map>

#include "MyAlloc.hpp"
#include "MyVec.hpp"

int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }

    return factorial(n - 1) * n;
}

void hw3::solution() {
    std::cout << "1) std::map<int, int>" << std::endl;
    std::map<int, int> map1;

    for (int i = 0; i < 10; ++i) {
        map1[i] = factorial(i);
    }

    for (const auto& pair : map1) {
        std::cout << "\tmap1[" << pair.first << "] = " << pair.second << std::endl;
    }

    std::cout << "2) std::map<int, int, std::less<int>, MyAlloc<std::pair<int, int>, 10>>" << std::endl;
    std::map<int, int, std::less<int>, MyAlloc<std::pair<int, int>, 10>> map2;

    for (int i = 0; i < 10; ++i) {
        map2[i] = factorial(i);
    }

    for (const auto& pair : map2) {
        std::cout << "\tmap2[" << pair.first << "] = " << pair.second << std::endl;
    }

    std::cout << "3) MyVec<int>" << std::endl;
    MyVec<int> vec1;
    for (int i = 0; i < 10; ++i) {
        vec1.push_back(i);
    }

    for (const auto& val : vec1) {
        std::cout << '\t' << val << std::endl;
    }

    std::cout << "4) MyVec<int, MyAlloc<int>>" << std::endl;
    MyVec<int, MyAlloc<int>> vec2;
    for (int i = 0; i < 10; ++i) {
        vec2.push_back(i);
    }

    for (const auto& val : vec2) {
        std::cout << '\t' << val << std::endl;
    }
}