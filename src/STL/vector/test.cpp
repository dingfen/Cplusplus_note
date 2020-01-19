#include "myvector.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    MY::vector<int, MY::alloc> v(4,3);
    for(auto i : v)
        std::cout << i << " ";
    std::cout << std::endl;
    return 0;
}
