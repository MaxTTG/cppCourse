#pragma once

#include <string>
#include <vector>

namespace hw12 {

struct FileSection {
    std::streampos start;
    std::streampos end;
};

/// Функция вычисления длины минимального общего префикса
size_t commonPrefixLength(const std::string &a, const std::string &b);

/// Функция вычисления минимально возможного префикса, который однозначно идентифицирует строку
std::string computeMinUniquePrefix(const std::string &s, const std::string *prev, const std::string *next);

/// Функтор свертки. Вычисление минимального уникального префикса по соседним элементам
std::vector<std::string> reduceFunctor(const std::vector<std::string> &block);

/// split:      определение секций файла
std::vector<FileSection> splitFileIntoSections(const std::string &src, int mnum);

/// map:        параллельное считывание и обработка секций
std::vector<std::vector<std::string>> mapStage(const std::string &src, const std::vector<FileSection> &sections);

/// shuffle:    объединение отсортированных списков
std::vector<std::vector<std::string>> shuffleStage(const std::vector<std::vector<std::string>> &mapResults, int rnum);

/// reduce:     параллельная обработка блоков и запись результатов
void reduceStage(const std::vector<std::vector<std::string>> &reduceInputs);

/// Основное решение
int solution(int argc, char *argv[]);

}  // namespace hw12
