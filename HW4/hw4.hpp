#pragma once

#include <iostream>
#include <list>
#include <tuple>
#include <utility>
#include <vector>

namespace hw4 {

template <typename T>
struct is_needed_container: std::false_type {};

template <typename T>
struct is_needed_container<std::vector<T>>: std::true_type {};

template <typename T>
struct is_needed_container<std::list<T>>: std::true_type {};

template <typename T>
using is_vector_or_list = std::enable_if_t<is_needed_container<T>::value>;

template <typename T>
using is_integral_t = std::enable_if_t<std::is_integral_v<T>>;

template <typename, typename = void>
struct contain_c_str: std::false_type {};

template <typename T>
struct contain_c_str<T, std::enable_if_t<std::is_same_v<decltype(std::declval<T>().c_str()), const char*>>>
    : std::true_type {};

template <typename T>
using is_string_t = std::enable_if_t<contain_c_str<T>::value>;

template <typename T, typename... Tail>
using are_all_the_same_t = std::enable_if_t<std::conjunction_v<std::is_same<T, Tail>...>>;

namespace {

constexpr char SEPARATOR = '.';

constexpr bool is_little_endian() {
    uint16_t num = 0x0102;
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&num);
    return *ptr == 0x02;
}

template <typename T, typename = void>
struct has_output_operator: std::false_type {};

template <typename T>
struct has_output_operator<T, std::void_t<decltype(std::cout << std::declval<T>())>>: std::true_type {};

template <typename T>
using can_be_outputed = std::enable_if_t<has_output_operator<T>::value>;

template <typename T>
can_be_outputed<T> printArgs(const T& t) {
    std::cout << t << std::endl;
}

template <typename T, typename... Args>
can_be_outputed<T> printArgs(const T& t, const Args&... args) {
    std::cout << t << SEPARATOR;
    printArgs(args...);
}

template <typename T>
is_vector_or_list<T> printArgs(const T& t) {
    auto i = t.begin();
    std::cout << *(i++);
    for (; i != t.end(); i++) {
        std::cout << SEPARATOR << *i;
    }

    std::cout << std::endl;
}

}  // namespace

template <typename T>
is_integral_t<T> print_ip(const T& t) {
    size_t size = sizeof(t);
    auto ptr    = reinterpret_cast<const uint8_t*>(&t);
    if (is_little_endian()) {
        ptr = ptr + size - 1;
    }

    std::vector<int> args;

    for (size_t i = 0; i < size - 1; ++i) {
        args.push_back(static_cast<int>(*(ptr + (is_little_endian() ? -i : i))));
    }

    args.push_back(static_cast<int>(*(ptr + (is_little_endian() ? -size + 1 : size - 1))));

    printArgs(args);
}

template <typename T>
is_string_t<T> print_ip(const T& t) {
    printArgs(t);
}

template <typename T>
is_vector_or_list<T> print_ip(const T& t) {
    printArgs(t);
}

template <typename T, typename... Tail>
are_all_the_same_t<T, Tail...> print_ip(const std::tuple<T, Tail...>& t) {
    std::apply([](const auto&... tupleArgs) { printArgs(tupleArgs...); }, t);
}

void solution();

}  // namespace hw4