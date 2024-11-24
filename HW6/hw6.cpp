#include "hw6.hpp"

#include <cassert>
#include <iostream>
#include <tuple>

#include "Matrix.hpp"

namespace hw6 {

void solution() {
    exampleFunc();

    Matrix<int, 0> matrix;

    std::cout << std::endl << "Task 1" << std::endl;
    for (size_t i{0}; i < 10; ++i) {
        matrix[i][i] = i;
        std::cout << i << " size = " << matrix.size() << std::endl;
    }

    std::cout << std::endl << "Task 2" << std::endl;
    for (size_t i{0}; i < 10; ++i) {
        matrix[i][9 - i] = 9 - i;
    }
    std::cout << "size = " << matrix.size() << std::endl;

    std::cout << std::endl << "Task 3" << std::endl;
    for (size_t i{0}; i < 9; ++i) {
        for (size_t j{0}; j < 9; ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "Task 4" << std::endl;
    std::cout << "size = " << matrix.size() << std::endl;

    std::cout << std::endl << "Task 5" << std::endl;
    for (auto c : matrix) {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << "row: " << x << "; col: " << y << "; val: " << v << std::endl;
    }
}

void exampleFunc() {
    // бесконечная матрица int заполнена значениями -1
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0);  // все ячейки свободны
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);
    // выведется одна строка
    // 100100314
    for (auto c : matrix) {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }
}

}  // namespace hw6
