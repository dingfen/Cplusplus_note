#ifndef __MY_VECTOR__
#define __MY_VECTOR__

#include <memory>
#include <iostream>
#include "../alloc/myallocator.hpp"
#include "../alloc/myconstruct.hpp"

namespace MY {

template<class T, class Alloc=std::allocator<T>>
class vector {
public:
    typedef T               value_type;
    typedef value_type*     pointer;
    typedef value_type*     iterator;
    typedef const value_type*   const_iterator;
    typedef value_type&     reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;

protected:
    typedef simple_alloc<value_type, Alloc> data_allocator;
    
    iterator start;
    iterator finish;
    iterator end_of_storage;

    iterator allocate_and_fill(size_type n, const T &value) {
        iterator result = data_allocator::allocate(n);
        std::uninitialized_fill_n(result, n, value);
        return result;
    }

    void fill_initialize(size_type n, const T &value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }

    void deallocate() {
        if (start)
            data_allocator::deallocate(start, end_of_storage - start);
    }
public:
    iterator begin()    { return start; }
    iterator end()      { return finish; }
    const_iterator begin() const   { return start; }
    const_iterator end() const     { return finish; }
    size_type size() const   { return size_type(finish - start); }
    bool empty()        { return start == finish; }
    reference operator[](size_type n) { return *(start + n); }

    vector() : start(0), finish(0), end_of_storage(0) {}
    vector(size_type n, const T& value) {fill_initialize(n, value); }
    explicit vector(size_type n) {fill_initialize(n, T());}
    vector(const vector& x) {
        start = data_allocator::allocate(x.size());
        finish = start + x.size();
        end_of_storage = finish;
        std::uninitialized_copy(x.begin(), x.end(), start); 
    }

    ~vector() {
        mydestroy<iterator, T>(start, finish);
        deallocate();
    }

    reference front()   { return *begin(); }
    reference back()    { return *end(); }
    reference at(size_type n)  {
        if (n > this->size())
            throw std::bad_alloc();
        return (*this)[n];
    }
};

} // namespace MY

#endif // __MY_VECTOR__