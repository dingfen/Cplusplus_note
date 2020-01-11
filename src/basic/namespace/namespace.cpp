#include <iostream>
#include <new>
#include <cstddef>

int x = 2;

int main(int argc, char const *argv[])
{
    int *p = (int *)::operator new(sizeof(int));
    int *q = new int;   // that's the same
    int x = 5;
    std::cout << "global x " << ::x << std::endl;
    std::cout << "local x " << x << std::endl;

    delete p;
    delete q;
    return 0;
}
