#include <vector>
#include <iostream>

#include "myallocator.hpp"

int main(int argc, char const *argv[])
{
    int a[] = {1,2,3,4,5};

    std::vector<int, MY::allocator<int>> v(a, a+5);
    for(auto i : v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}
