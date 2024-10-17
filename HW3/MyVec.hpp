#pragma once

#include <memory>

namespace hw3 {

template <class T, class Allocator = std::allocator<T>>
class MyVec {
public:
    MyVec() : _size(0), _capacity(0), _data(nullptr) {}
    MyVec(size_t size) : _size(size), _capacity(size), _data(_allocator.allocate(size)) {}

    ~MyVec() {
        clear();
        if (_data) {
            _allocator.deallocate(_data, _capacity);
        }
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return _capacity;
    }

    T& operator[](size_t index) {
        if (index >= _size) {
            throw std::out_of_range("Index out of bounds");
        }
        
        return _data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("Index out of bounds");
        }

        return _data[index];
    }

    void push_back(const T& value) {
        if (_size >= _capacity) {
            resize();
        }

        _allocator.construct(_data + _size, value);
        ++_size;
    }

    void clear() {
        for (size_t i = 0; i < _size; ++i) {
            _allocator.destroy(&_data[i]);
        }

        _size = 0;
    }

    T* begin() {
        return _data;
    }

    const T* begin() const {
        return _data;
    }

    T* end() {
        return _data + _size;
    }

    const T* end() const {
        return _data + _size;
    }

    void resize() {
        size_t new_capacity = _capacity == 0 ? 1 : _capacity * 2;
        T* new_data = _allocator.allocate(new_capacity);

        for (size_t i = 0; i < _size; ++i) {
            _allocator.construct(new_data + i, std::move(_data[i]));
            _allocator.destroy(_data + i);
        }

        if (_data) {
            _allocator.deallocate(_data, _capacity);
        }

        _data = new_data;
        _capacity = new_capacity;
    }

private:
    size_t _size;
    size_t _capacity;
    T* _data;
    Allocator _allocator;
};
    
}