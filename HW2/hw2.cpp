#include "hw2.hpp"

#include <algorithm>

auto hw2::split(const std::string& ip_str, char c) {
    std::vector<std::string> r;
    std::string::size_type start = 0;
    std::string::size_type stop = ip_str.find_first_of(c);

    while (stop != std::string::npos) {
        r.push_back(ip_str.substr(start, stop - start));

        start = stop + 1;
        stop = ip_str.find_first_of(c, start);
    }

    r.push_back(ip_str.substr(start));

    return r;
}

void hw2::solution() {
    std::vector< std::vector<std::string> > ip_pool;
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) break;
        auto v = split(line, '\t');
        ip_pool.push_back(split(v.at(0), '.'));
    }

    std::sort(ip_pool.rbegin(), ip_pool.rend(), [](std::vector<std::string> a, std::vector<std::string> b) {
        for (int i = 0; i < 4; ++i) {
            uint8_t a_num = std::stoi(a[i]);
            uint8_t b_num = std::stoi(b[i]);

            if (a_num != b_num) {
                return a_num < b_num;
            }
        }

        return true;
    });

    printIpPool(ip_pool);
    printIpPool(filter(ip_pool, {1}));
    printIpPool(filter(ip_pool, {46, 70}));
    printIpPool(filter_any(ip_pool, {46}));
}
