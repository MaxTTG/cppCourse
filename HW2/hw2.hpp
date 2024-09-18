#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <algorithm>

namespace hw2 {

void solution();

auto split(const std::string& ip_str, char c);

template <template <typename, typename> class Container, 
          typename Allocator = std::allocator<std::string>>
void printIpPool(const Container<std::vector<std::string>, Allocator> & pool) {
    for (const auto& ip : pool) {
        for (auto ip_part_iter = ip.begin(); ip_part_iter != ip.end(); ++ip_part_iter) {
            if (ip_part_iter != ip.cbegin()) {
                std::cout << '.';
            }

            std::cout << *ip_part_iter;
        }

        std::cout << '\n';
    }
}

template <template <typename, typename> class Container, 
          typename Allocator = std::allocator<std::string>>
auto filter(const Container<std::vector<std::string>, Allocator> & pool, const std::initializer_list<int> octants) {
    int octantIndex = 0;

    auto first = pool.begin();
    auto last = pool.end();

    for (const auto octant : octants) {
        first = std::lower_bound(first, last, octant, [octantIndex](auto a, auto b) {
            return std::stoi(a[octantIndex]) > b;
        });

        last = std::upper_bound(first, last, octant, [octantIndex](auto a, auto b) {
            return a > std::stoi(b[octantIndex]);
        });

        ++octantIndex;
    }

    return Container<std::vector<std::string>, Allocator>(first, last);
}

template <template <typename, typename> class Container, 
          typename Allocator = std::allocator<std::string>>
auto filter_any(const Container<std::vector<std::string>, Allocator> & pool, const int needOctant) {
    Container<std::vector<std::string>, Allocator> ans;

    for (const auto& ip : pool) {
        for (const auto& octant : ip) {
            if (std::stoi(octant) == needOctant) {
                ans.push_back(ip);
                break;
            }
        }
    }

    return ans;
}

}