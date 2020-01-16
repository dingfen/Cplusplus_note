#ifndef __MY_ALLOCATOR__
#define __MY_ALLOCATOR__

#include <new>
#include <cstddef>
#include <iostream>
#include <climits>

namespace MY {
    
template <class T>
inline T* __allocate(ptrdiff_t size, T*) {
    std::set_new_handler(0);
    // allocate memory only
    T* tmp = (T*)(::operator new)((size_t) (size * sizeof(T)));
    if (tmp == 0) {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    return tmp;
}

template <class T>
inline void __dellocate(T* t) {
    ::operator delete(t);
}

template <class T1, class T2>
inline void __construct(T1 *t, const T2 & value) {
    new(t) T1(value);   // placement new construct T1 object
}

template <class T>
inline void __destory(T* t) {
    t->~T();
}


template <class T>
class allocator {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

    pointer allocate(size_type n, const void *hint = 0) {
        return __allocate((difference_type)n, (T*)0);
    };

    void deallocate(pointer p, size_type) {
        __dellocate(p);
    };

    void construct(pointer p, const T& value) {
        __construct(p, value);
    };

    void destory(pointer ptr) {
        __destory(ptr);
    };
    
    pointer address(reference x) {
        return (pointer)&x;
    };

    const_pointer address(const_reference x) {
        return (const_pointer)&x;
    };

    size_type max_size() const {
        return size_type(UINT_MAX/sizeof(T));
    };

    template <class U>
    struct rebind {
        typedef allocator<U> other;
    };
};

template <>
class allocator<void> {
public:
    typedef void* pointer;
};

} // namespace MY


#endif // __MY_ALLOCATOR__