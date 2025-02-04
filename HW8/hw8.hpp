#pragma once

#include <algorithm>
#include <boost/crc.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "FileInfo.hpp"
#include "FileReader.hpp"
#include "Options.hpp"

namespace hw8 {

namespace fs = boost::filesystem;
namespace po = boost::program_options;

/// Функция для вычисления crc32-хэша для следующего блока файла.
uint32_t readBlockAndCompute(FileReader &reader, size_t blockSize);

/// Рекурсивная функция, которая группирует файлы по последовательности блоковых хэшей.
/// Если дошли до конца файла (блоков), добавляем группу дубликатов.
void processGroup(std::vector<FileReader *> &readers, size_t blockIndex, size_t blockSize, size_t totalBlocks,
                  std::vector<std::vector<FileReader *>> &duplicates);

/// Преобразование маски с wildcards ('*', '?') в регулярное выражение.
boost::regex wildcardToRegex(const std::string &mask, bool caseInsensitive = true);

/// Функция для разбора параметров командной строки.
Options parseOptions(int argc, char *argv[]);

/// Функция проверки, удовлетворяет ли файл условиям (минимальный размер и маски).
bool fileMatchesConditions(const fs::path &filePath, const Options &opts, uint64_t fsize);

/// Нерекурсивный обход директории.
std::vector<FileInfo> scanDirectoryNonRecursive(const fs::path &dirPath, const Options &opts);

/// Рекурсивный обход директории с учетом уровня глубины и исключаемых директорий.
std::vector<FileInfo> scanDirectoryRecursive(const fs::path &dirPath, const Options &opts);

/// Функция сканирования директорий согласно параметрам и сбор файлов, удовлетворяющих условиям.
std::vector<FileInfo> scanDirectories(const Options &opts);

/// Группировка файлов по размеру.
std::unordered_map<uint64_t, std::vector<FileInfo>> groupFilesBySize(const std::vector<FileInfo> &files);

// Обработка группы файлов одинакового размера: блочное сравнение и вывод найденных дубликатов.
void processDuplicatesInGroup(const std::vector<FileInfo> &group, size_t blockSize);

/// Основное решение
int solution(int argc, char *argv[]);

}  // namespace hw8
