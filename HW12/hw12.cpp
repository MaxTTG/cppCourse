#include "hw12.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>

namespace hw12 {

size_t commonPrefixLength(const std::string &a, const std::string &b) {
    size_t len    = 0;
    size_t maxLen = std::min(a.size(), b.size());
    while (len < maxLen && a[len] == b[len]) {
        ++len;
    }

    return len;
}

std::string computeMinUniquePrefix(const std::string &s, const std::string *prev, const std::string *next) {
    size_t prefixPrev = prev ? commonPrefixLength(s, *prev) : 0;
    size_t prefixNext = next ? commonPrefixLength(s, *next) : 0;
    size_t needed     = std::max(prefixPrev, prefixNext) + 1;
    if (needed > s.size()) {
        needed = s.size();
    }

    return s.substr(0, needed);
}

std::vector<std::string> reduceFunctor(const std::vector<std::string> &block) {
    std::vector<std::string> result;
    size_t n = block.size();
    result.reserve(n);
    for (size_t i = 0; i < n;) {
        size_t j = i + 1;
        while (j < n && block[j] == block[i]) {
            ++j;
        }

        const std::string *prev = (i > 0) ? &block[i - 1] : nullptr;
        const std::string *next = (j < n) ? &block[j] : nullptr;
        std::string prefix      = computeMinUniquePrefix(block[i], prev, next);
        for (size_t k = i; k < j; ++k) {
            result.push_back(prefix);
        }

        i = j;
    }

    return result;
}

std::vector<FileSection> splitFileIntoSections(const std::string &src, int mnum) {
    std::vector<FileSection> sections;

    std::ifstream file(src, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Не удалось открыть файл: " << src << std::endl;
        exit(1);
    }

    std::streampos fileSize = file.tellg();
    file.seekg(0);

    std::streampos approxSectionSize = fileSize / mnum;
    std::streampos currentStart      = 0;

    for (int i = 0; i < mnum; ++i) {
        FileSection section;
        section.start = currentStart;

        if (i == mnum - 1) {
            section.end = fileSize;
            sections.push_back(section);
            break;
        }

        std::streampos tentativeEnd = currentStart + approxSectionSize;
        file.seekg(tentativeEnd);
        // Чтобы не разорвать строку, считываем остаток текущей строки
        std::string dummy;
        std::getline(file, dummy);
        section.end = file.tellg();
        if (section.end == std::streampos(-1)) {
            section.end = fileSize;
        }

        sections.push_back(section);
        currentStart = section.end;
    }

    return sections;
}

std::vector<std::vector<std::string>> mapStage(const std::string &src, const std::vector<FileSection> &sections) {
    int mnum = sections.size();
    std::vector<std::vector<std::string>> mapResults(mnum);
    std::vector<std::thread> mapThreads;

    for (int i = 0; i < mnum; ++i) {
        mapThreads.emplace_back([&, i]() {
            std::ifstream in(src);
            if (!in) {
                std::cerr << "Не удалось открыть файл в map-потоке " << i << "\n";
                return;
            }

            in.seekg(sections[i].start);
            std::string line;
            while (in.tellg() < sections[i].end && std::getline(in, line)) {
                mapResults[i].push_back(line);
            }

            std::sort(mapResults[i].begin(), mapResults[i].end());
        });
    }

    for (auto &t : mapThreads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return mapResults;
}

std::vector<std::vector<std::string>> shuffleStage(const std::vector<std::vector<std::string>> &mapResults, int rnum) {
    using HeapElem = std::tuple<std::string, size_t, size_t>;  // (значение, номер списка, индекс внутри списка)
    auto cmp       = [](const HeapElem &a, const HeapElem &b) { return std::get<0>(a) > std::get<0>(b); };
    std::priority_queue<HeapElem, std::vector<HeapElem>, decltype(cmp)> minHeap(cmp);

    size_t mnum = mapResults.size();
    for (size_t i = 0; i < mnum; ++i) {
        if (!mapResults[i].empty()) {
            minHeap.emplace(mapResults[i][0], i, 0);
        }
    }

    std::vector<std::string> globalSorted;
    while (!minHeap.empty()) {
        auto [str, listIndex, elemIndex] = minHeap.top();
        minHeap.pop();
        globalSorted.push_back(str);
        if (elemIndex + 1 < mapResults[listIndex].size()) {
            minHeap.emplace(mapResults[listIndex][elemIndex + 1], listIndex, elemIndex + 1);
        }
    }

    std::vector<std::vector<std::string>> reduceInputs(rnum);
    size_t total     = globalSorted.size();
    size_t blockSize = total / rnum;
    size_t start     = 0;
    for (int i = 0; i < rnum; ++i) {
        size_t end      = (i + 1 == rnum) ? total : start + blockSize;
        reduceInputs[i] = std::vector<std::string>(globalSorted.begin() + start, globalSorted.begin() + end);
        start           = end;
    }

    return reduceInputs;
}

void reduceStage(const std::vector<std::vector<std::string>> &reduceInputs) {
    int rnum = reduceInputs.size();
    std::vector<std::thread> reduceThreads;

    for (int i = 0; i < rnum; ++i) {
        reduceThreads.emplace_back([&, i]() {
            std::vector<std::string> blockExtended = reduceInputs[i];
            if (i > 0 && !reduceInputs[i - 1].empty()) {
                blockExtended.insert(blockExtended.begin(), reduceInputs[i - 1].back());
            }

            if (i < rnum - 1 && !reduceInputs[i + 1].empty()) {
                blockExtended.push_back(reduceInputs[i + 1].front());
            }

            std::vector<std::string> outputExtended = reduceFunctor(blockExtended);
            std::vector<std::string> output;
            size_t startIdx = (i > 0) ? 1 : 0;
            size_t endIdx   = outputExtended.size() - ((i < rnum - 1) ? 1 : 0);
            for (size_t j = startIdx; j < endIdx; ++j) {
                output.push_back(outputExtended[j]);
            }

            std::string filename = "result_" + std::to_string(i) + ".txt";
            std::ofstream out(filename);
            if (!out) {
                std::cerr << "Не удалось открыть файл " << filename << " для записи\n";
                return;
            }

            for (size_t k = 0; k < output.size(); ++k) {
                out << reduceInputs[i][k] << " : " << output[k] << "\n";
            }

            std::cout << "Результат записан в файл: " << filename << std::endl;
        });
    }

    for (auto &t : reduceThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

int solution(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Использование: mapreduce <src> <mnum> <rnum>" << std::endl;
        return 1;
    }

    std::string srcFile = argv[1];
    int mnum            = std::stoi(argv[2]);
    int rnum            = std::stoi(argv[3]);

    if (mnum <= 0 || rnum <= 0) {
        std::cerr << "Числа mnum и rnum должны быть положительными" << std::endl;
        return 1;
    }

    std::vector<FileSection> sections                  = splitFileIntoSections(srcFile, mnum);
    std::vector<std::vector<std::string>> mapResults   = mapStage(srcFile, sections);
    std::vector<std::vector<std::string>> reduceInputs = shuffleStage(mapResults, rnum);
    reduceStage(reduceInputs);

    std::cout << "Обработка завершена успешно.\n";
    return 0;
}

}  // namespace hw12
