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

// Функция для вычисления crc32-хэша для следующего блока файла.
uint32_t readBlockAndCompute(FileReader &reader, size_t blockSize) {
    std::string buffer(blockSize, 0);
    reader.stream.read(buffer.data(), blockSize);
    boost::crc_32_type crc;
    crc.process_bytes(buffer.data(), blockSize);
    return crc.checksum();
}

// Рекурсивная функция, которая группирует файлы по последовательности блоковых хэшей.
// Если дошли до конца файла (блоков), добавляем группу дубликатов.
void processGroup(std::vector<FileReader *> &readers, size_t blockIndex, size_t blockSize, size_t totalBlocks,
                  std::vector<std::vector<FileReader *>> &duplicates) {
    if (readers.size() < 2) {
        return;
    }

    if (blockIndex >= totalBlocks) {
        duplicates.push_back(readers);
        return;
    }

    std::unordered_map<uint32_t, std::vector<FileReader *>> groups;
    for (auto reader : readers) {
        if (reader->blockHashes.size() <= blockIndex) {
            uint32_t hash = readBlockAndCompute(*reader, blockSize);
            reader->blockHashes.push_back(hash);
        }

        uint32_t hash = reader->blockHashes[blockIndex];
        groups[hash].push_back(reader);
    }

    for (auto &entry : groups) {
        if (entry.second.size() >= 2) {
            processGroup(entry.second, blockIndex + 1, blockSize, totalBlocks, duplicates);
        }
    }
}

// Преобразование маски с wildcards ('*', '?') в регулярное выражение.
boost::regex wildcardToRegex(const std::string &mask, bool caseInsensitive = true) {
    std::string regexStr;
    regexStr.reserve(mask.size() * 2);
    regexStr.push_back('^');
    for (char ch : mask) {
        switch (ch) {
        case '*':
            regexStr.append(".*");
            break;
        case '?':
            regexStr.push_back('.');
            break;
        case '.':
        case '(':
        case ')':
        case '+':
        case '|':
        case '^':
        case '$':
        case '{':
        case '}':
        case '[':
        case ']':
            regexStr.push_back('\\');
            regexStr.push_back(ch);
            break;
        default:
            regexStr.push_back(ch);
            break;
        }
    }

    regexStr.push_back('$');
    if (caseInsensitive) {
        return boost::regex(regexStr, boost::regex_constants::icase);
    } else {
        return boost::regex(regexStr);
    }
}

// Функция для разбора параметров командной строки.
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

// Функция проверки, удовлетворяет ли файл условиям (минимальный размер и маски).
bool fileMatchesConditions(const fs::path &filePath, const Options &opts, uint64_t fsize) {
    if (fsize < opts.minSize) {
        return false;
    }

    if (!opts.masks.empty()) {
        bool match = false;
        for (const auto &rx : opts.masks) {
            if (boost::regex_match(filePath.filename().string(), rx)) {
                match = true;
                break;
            }
        }

        return match;
    }

    return true;
}

// Нерекурсивный обход директории.
std::vector<FileInfo> scanDirectoryNonRecursive(const fs::path &dirPath, const Options &opts) {
    std::vector<FileInfo> files;
    for (fs::directory_iterator it(dirPath), end; it != end; ++it) {
        if (!fs::is_regular_file(*it)) {
            continue;
        }

        try {
            uint64_t fsize = fs::file_size(it->path());
            if (!fileMatchesConditions(it->path(), opts, fsize))
                continue;
            files.push_back({it->path(), fsize});
        } catch (...) {
            // Игнорируем файлы с ошибками доступа.
        }
    }

    return files;
}

// Рекурсивный обход директории с учетом уровня глубины и исключаемых директорий.
std::vector<FileInfo> scanDirectoryRecursive(const fs::path &dirPath, const Options &opts) {
    std::vector<FileInfo> files;
    fs::recursive_directory_iterator end_it;
    for (fs::recursive_directory_iterator it(dirPath); it != end_it; ++it) {
        if (it.depth() > opts.depth) {
            it.pop();
            continue;
        }

        if (fs::is_regular_file(*it)) {
            fs::path canonicalPath = fs::canonical(it->path());
            bool excluded          = false;
            for (const auto &ex : opts.excludeDirs) {
                if (canonicalPath.string().find(ex.string()) == 0) {
                    excluded = true;
                    break;
                }
            }

            if (excluded) {
                continue;
            }

            try {
                uint64_t fsize = fs::file_size(it->path());
                if (!fileMatchesConditions(it->path(), opts, fsize))
                    continue;
                files.push_back({it->path(), fsize});
            } catch (...) {
                // Игнорируем файлы с ошибками доступа.
            }
        }
    }

    return files;
}

// Функция сканирования директорий согласно параметрам и сбор файлов, удовлетворяющих условиям.
std::vector<FileInfo> scanDirectories(const Options &opts) {
    std::vector<FileInfo> allFiles;
    for (const auto &dirStr : opts.scanDirs) {
        fs::path dirPath(dirStr);
        if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
            std::cerr << "Директория не найдена или не является директорией: " << dirStr << std::endl;
            continue;
        }

        std::vector<FileInfo> filesInDir =
            (opts.depth ? scanDirectoryRecursive(dirPath, opts) : scanDirectoryNonRecursive(dirPath, opts));

        allFiles.insert(allFiles.end(), filesInDir.begin(), filesInDir.end());
    }

    return allFiles;
}

// Группировка файлов по размеру.
std::unordered_map<uint64_t, std::vector<FileInfo>> groupFilesBySize(const std::vector<FileInfo> &files) {
    std::unordered_map<uint64_t, std::vector<FileInfo>> filesBySize;
    for (const auto &f : files) {
        filesBySize[f.size].push_back(f);
    }

    return filesBySize;
}

// Обработка группы файлов одинакового размера: блочное сравнение и вывод найденных дубликатов.
void processDuplicatesInGroup(const std::vector<FileInfo> &group, size_t blockSize) {
    if (group.size() < 2) {
        return;
    }

    size_t totalBlocks = (group.front().size + blockSize - 1) / blockSize;
    std::vector<std::unique_ptr<FileReader>> readers;
    std::vector<FileReader *> readerPtrs;
    for (const auto &fi : group) {
        try {
            readers.push_back(std::make_unique<FileReader>(fi.path, fi.size));
            readerPtrs.push_back(readers.back().get());
        } catch (const std::exception &e) {
            std::cerr << "Ошибка открытия файла " << fi.path << ": " << e.what() << std::endl;
        }
    }

    if (readerPtrs.size() < 2) {
        return;
    }

    std::vector<std::vector<FileReader *>> duplicateGroups;
    processGroup(readerPtrs, 0, blockSize, totalBlocks, duplicateGroups);
    for (const auto &dupGroup : duplicateGroups) {
        if (dupGroup.size() < 2) {
            continue;
        }

        for (auto reader : dupGroup) {
            std::cout << fs::canonical(reader->path).string() << std::endl;
        }

        std::cout << std::endl;
    }
}

// Основное решение: разбор параметров, сканирование, группировка и обработка дубликатов.
int solution(int argc, char *argv[]) {
    try {
        Options opts                   = parseOptions(argc, argv);
        std::vector<FileInfo> allFiles = scanDirectories(opts);
        auto filesBySize               = groupFilesBySize(allFiles);
        for (const auto &entry : filesBySize) {
            if (entry.second.size() < 2)
                continue;
            processDuplicatesInGroup(entry.second, opts.blockSize);
        }

    } catch (std::exception &ex) {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

}  // namespace hw8
