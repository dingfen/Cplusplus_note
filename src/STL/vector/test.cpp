#include "myvector.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    MY::vector<int, MY::alloc> v(4,3);
    MY::vector<int, MY::alloc> v2(v);

    for(auto i : v2)
        std::cout << i << " ";
    std::cout << std::endl;
    
    return 0;
}
