#pragma once

#include <cstddef>
#include <memory>
#include <iostream>
#include <list>

#include "MemoryWrapper.hpp"

namespace hw3 {

template <class T, size_t startSize = 10>
struct MyAlloc {
    using value_type = T;

    std::list<std::shared_ptr<MemoryWrapper>> pool;
    size_t poolSize = startSize;
    size_t pos;

    MyAlloc () noexcept {}

    template <class U> MyAlloc(const MyAlloc <U>& a) noexcept {
        pool = a.pool;
        poolSize = a.poolSize;
        pos = a.pos;
    }

    MyAlloc select_on_container_copy_construction() const {
        return MyAlloc();
    }

    T* allocate (size_t n) {
        if (n > poolSize) {
            poolSize *= 2;
            pool.push_back(std::make_shared<MemoryWrapper>(poolSize * sizeof(T)));
            pos = 0;
        } 

        if (pool.empty() || pos + n > poolSize) {
            pool.push_back(std::make_shared<MemoryWrapper>(poolSize * sizeof(T)));
            pos = 0;
        }

        auto& currentBlock = pool.back();
        T* result = reinterpret_cast<T*>(reinterpret_cast<char*>(currentBlock->begin()) + pos * sizeof(T));
        pos += n;
        return result;
    }

    void deallocate (T*, std::size_t) {
        // чистится sharet_ptr + MemoryWrapper'ом
    }
    
    template <class U, class... Args>
    void construct(U* p, Args&&... args) {
        ::new((void*)p) U(std::forward<Args>(args)...);
    }

    template <class U>
    void destroy(U* p) {
        p->~U();
    }

    template<class U>
    struct rebind {
        typedef MyAlloc<U> other;
    };

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
};

template <class T, class U>
constexpr bool operator== (const MyAlloc<T>& a1, const MyAlloc<U>& a2) noexcept
{
    return a1.pool == a2.pool;
}

template <class T, class U>
constexpr bool operator!= (const MyAlloc<T>& a1, const MyAlloc<U>& a2) noexcept
{
    return a1.pool != a2.pool;
}

}