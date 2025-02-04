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
Options parseOptions(int argc, char *argv[]) {
    Options opts;
    po::options_description desc("Доступные опции");
    // clang-format off
    desc.add_options()
            ("help,h",                                                           "Показать справку")
            ("scan-dir",    po::value<std::vector<std::string>>()->multitoken(), "Директории для сканирования")
            ("exclude-dir", po::value<std::vector<std::string>>()->multitoken(), "Директории, исключаемые из сканирования")
            ("depth",       po::value<int>()->default_value(0),                  "Уровень сканирования (0 - только указанная директория, >0 - с вложенными)")
            ("min-size",    po::value<uint64_t>()->default_value(1),             "Минимальный размер файла (в байтах)")
            ("mask",        po::value<std::vector<std::string>>()->multitoken(), "Маски имен файлов (поддерживаются * и ?)")
            ("block-size",  po::value<size_t>()->required(),                     "Размер блока S (в байтах)");
    // clang-format on

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }

    po::notify(vm);

    if (vm.count("scan-dir"))
        opts.scanDirs = vm["scan-dir"].as<std::vector<std::string>>();
    else {
        std::cerr << "Необходимо указать хотя бы одну директорию для сканирования (--scan-dir)" << std::endl;
        exit(1);
    }

    if (vm.count("exclude-dir")) {
        for (const auto &d : vm["exclude-dir"].as<std::vector<std::string>>()) {
            opts.excludeDirs.insert(fs::canonical(fs::path(d)));
        }
    }

    opts.depth     = vm["depth"].as<int>();
    opts.minSize   = vm["min-size"].as<uint64_t>();
    opts.blockSize = vm["block-size"].as<size_t>();

    if (vm.count("mask")) {
        for (const auto &m : vm["mask"].as<std::vector<std::string>>()) {
            opts.masks.push_back(wildcardToRegex(m));
        }
    }

    return opts;
}

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
