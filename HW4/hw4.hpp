#pragma once

#include <iostream>
#include <list>
#include <tuple>
#include <utility>
#include <vector>

namespace hw4 {

/**
 * @brief Блок для нахождения std::vector и std::list
 *
 * @tparam T
 */
template <typename T>
struct is_needed_container: std::false_type {};

template <typename T>
struct is_needed_container<std::vector<T>>: std::true_type {};

template <typename T>
struct is_needed_container<std::list<T>>: std::true_type {};

template <typename T>
using is_vector_or_list = std::enable_if_t<is_needed_container<T>::value>;

/**
 * @brief Шаблон для выявления целочисленных
 *
 * @tparam T
 */
template <typename T>
using is_integral_t = std::enable_if_t<std::is_integral_v<T>>;

/**
 * @brief Блок для выявления типов с определенной функцией c_str()
 *
 */
template <typename, typename = void>
struct contain_c_str: std::false_type {};

template <typename T>
struct contain_c_str<T, std::enable_if_t<std::is_same_v<decltype(std::declval<T>().c_str()), const char*>>>
    : std::true_type {};

template <typename T>
using is_string_t = std::enable_if_t<contain_c_str<T>::value>;

/**
 * @brief Проверка на то, что все типы T, Tail... одинаковы
 *
 */
template <typename T, typename... Tail>
using are_all_the_same_t = std::enable_if_t<std::conjunction_v<std::is_same<T, Tail>...>>;

namespace {

/**
 * @brief Разделитель для задачи; т.к. нужно вывести IP, разделитель символ "точка"
 *
 */
constexpr char SEPARATOR = '.';

/**
 * @brief Для битовой магии
 *
 * @return true если в системе little endian
 * @return false если в системе big endian
 */
constexpr bool is_little_endian() {
    uint16_t num = 0x0102;
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&num);
    return *ptr == 0x02;
}

/**
 * @brief Блок для выявления типов с определенным оператором вывода
 *
 * @tparam T
 * @tparam typename
 */
template <typename T, typename = void>
struct has_output_operator: std::false_type {};

template <typename T>
struct has_output_operator<T, std::void_t<decltype(std::cout << std::declval<T>())>>: std::true_type {};

template <typename T>
using can_be_outputed = std::enable_if_t<has_output_operator<T>::value>;

/**
 * @brief Вывод в консоль одиночного значения
 *
 * @tparam T - тип должен иметь определенный оператор вывода
 * @param t - выводимое значение
 * @return can_be_outputed<T> SFINAE void
 */
template <typename T>
can_be_outputed<T> printArgs(const T& t) {
    std::cout << t << std::endl;
}

/**
 * @brief Вывод в консоль параметр пака
 *
 * @tparam T - тип должен иметь определенный оператор вывода
 * @tparam Args - параметр пак
 * @param t - выводимое значение
 * @param args - аргументы рекурсивного вызова функции
 * @return can_be_outputed<T> SFINAE void
 */
template <typename T, typename... Args>
can_be_outputed<T> printArgs(const T& t, const Args&... args) {
    std::cout << t << SEPARATOR;
    printArgs(args...);
}

/**
 * @brief
 *
 * @tparam T
 * @param t
 * @return is_vector_or_list<T>
 */
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

/**
 * @brief Вывод когда передается число
 *
 * @tparam T - тип переменной
 * @param t - число
 * @return is_integral_t<T> SFINAE void
 */
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

/**
 * @brief Вывод когда передается тип с методом c_str() [в данном контексте передается строка]
 *
 * @tparam T - тип переменной
 * @param t - строка
 * @return is_string_t<T> SFINAE void
 */
template <typename T>
is_string_t<T> print_ip(const T& t) {
    printArgs(t);
}

/**
 * @brief Вывод когда передается std::vector или std::list
 *
 * @tparam T - тип переменной
 * @param t - вектор или лист
 * @return is_vector_or_list<T> SFINAE void
 */
template <typename T>
is_vector_or_list<T> print_ip(const T& t) {
    printArgs(t);
}

/**
 * @brief Вывод когда передается std::tuple, в котором лежат одинаковые типы
 *
 * @tparam T - для мервого значения
 * @tparam Tail - остальной параметр пак
 * @param t - сам тупл
 * @return are_all_the_same_t<T, Tail...> SFINAE void
 */

template <typename T, typename... Tail>
are_all_the_same_t<T, Tail...> print_ip(const std::tuple<T, Tail...>& t) {
    std::apply([](const auto&... tupleArgs) { printArgs(tupleArgs...); }, t);
}

/**
 * @brief Непосредственно решение
 */
void solution();

}  // namespace hw4