#ifndef __MY_ALLOCATOR__
#define __MY_ALLOCATOR__

#include <new>
#include <cstddef>
#include <iostream>
#include <climits>

namespace MY {
    
template <class T>
inline T* __allocate(ptrdiff_t size, T*) {
    // set new handler 
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

// template specialization  template <> added in g++
template <>
class allocator<void> {
public:
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
    typedef void        value_type;

    template <class T> struct rebind {
        typedef allocator<T> other;
    };
};


template<class T, class Alloc=std::allocator<T>>
class simple_alloc {
public:
    static T *allocate(size_t n)
        { return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T)); };
    static T *allocate(void)
        { return (T*)Alloc::allocate(sizeof(T)); };
    static void deallocate(T *p)
        { Alloc::deallocate(p, sizeof(T)); };
    static void deallocate(T *p , size_t n)
        { if (0 != n) Alloc::deallocate(p, n*sizeof(T)); };
};

} // namespace MY


#endif // __MY_ALLOCATOR__