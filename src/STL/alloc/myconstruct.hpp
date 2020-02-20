#ifndef __MY_CONSTRUCT__
#define __MY_CONSTRUCT__

#include <new>
#include <memory>

namespace MY {

template <class T1, class T2>
inline void myconstruct(T1 *ptr, const T2 & value) {
    new(ptr) T1(value);
}

template <class T>
inline void mydestroy(T *ptr) {
    ptr->~T();
}

template <class ForwardIterator, class T>
inline void mydestroy(ForwardIterator first, ForwardIterator last) {
    for(; first < last; ++first)
        mydestroy(&*first);
}

}   // namespace MY

#endif  // __MY_CONSTRUCT__