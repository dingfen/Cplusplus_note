#include <iostream>

constexpr int fun(int a, int b)
{
    return 10;
}

int func(int a, int b)
{
    return 10;
}

int main(int argc, char const *argv[])
{
    int b[fun(3,4)];
    int c[func(4,5)];
    return 0;
}
