#include <cassert>
#include <iostream>
#include <map>
#include <tuple>

template <typename T, T DefaultValue>
class Matrix {
private:
    struct ProxyValue {
        Matrix& parent;
        int row, col;

        operator T() const { return parent.get(row, col); }

        ProxyValue& operator=(T value) {
            parent.set(row, col, value);
            return *this;
        }
    };

    struct ProxyRow {
        Matrix& parent;
        int row;

        ProxyValue operator[](int col) { return ProxyValue{parent, row, col}; }
    };

    std::map<int, std::map<int, T>> data;
    int occupied = 0;

public:
    Matrix() = default;

    T get(int row, int col) const {
        auto it = data.find(row);
        if (it != data.end()) {
            auto colIt = it->second.find(col);
            if (colIt != it->second.end()) {
                return colIt->second;
            }
        }
        return DefaultValue;
    }

    void set(int row, int col, T value) {
        if (value == DefaultValue) {
            if (get(row, col) != DefaultValue) {
                --occupied;
            }

            data[row].erase(col);
            if (data[row].empty()) {
                data.erase(row);
            }
        } else {
            if (get(row, col) == DefaultValue) {
                ++occupied;
            }

            data[row][col] = value;
        }
    }

    int size() const { return occupied; }

    ProxyRow operator[](int row) { return ProxyRow{*this, row}; }

    class Iterator {
        typename std::map<int, std::map<int, T>>::const_iterator rowIt;
        typename std::map<int, T>::const_iterator colIt;
        const std::map<int, std::map<int, T>>* matrix;

    public:
        Iterator(const std::map<int, std::map<int, T>>* matrix, bool end = false) : matrix(matrix) {
            if (end || matrix->empty()) {
                rowIt = matrix->end();
                colIt = {};
            } else {
                rowIt = matrix->begin();
                colIt = rowIt->second.begin();
                advance();
            }
        }

        void advance() {
            while (rowIt != matrix->end() && colIt == rowIt->second.end()) {
                ++rowIt;
                if (rowIt != matrix->end()) {
                    colIt = rowIt->second.begin();
                }
            }
        }

        bool operator!=(const Iterator& other) const {
            return rowIt != other.rowIt || (rowIt != matrix->end() && colIt != other.colIt);
        }

        void operator++() {
            ++colIt;
            advance();
        }

        std::tuple<int, int, T> operator*() const { return {rowIt->first, colIt->first, colIt->second}; }
    };

    Iterator begin() const { return Iterator(&data); }

    Iterator end() const { return Iterator(&data, true); }
};
