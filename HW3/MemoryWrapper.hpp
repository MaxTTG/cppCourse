#pragma once

#include <cstddef>

namespace hw3 {

class MemoryWrapper {
public:
    MemoryWrapper() = default;

    MemoryWrapper(size_t size) : _pos(0), _size(size) {
        _data = ::operator new(size);
    };

    ~MemoryWrapper() {
        ::operator delete(_data);
    };

    void* begin() noexcept {
        return _data;
    }

    void* end() noexcept {
        return reinterpret_cast<char*>(_data) + _size;
    }

    void push() noexcept {
        ++_pos;
    }

    void pop() noexcept {
        --_pos;
    }

    size_t pos() noexcept {
        return _pos;
    }

    size_t size() noexcept {
        return _size;
    }

    bool empty() const noexcept {
        return _pos == 0;
    }

    bool operator==(MemoryWrapper const& mw) noexcept {
        return _data == mw._data;
    }

private:
    void* _data = nullptr;
    size_t _pos = 0;
    size_t _size = 0;
};

}