#include "fun.h"

#include <vector>
#include <iostream>

int add(int a, int b)
{
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    return a+b;
}

int sum(std::vector<int> a)
{
    int sum = 0;
    for(int i = 0; i < a.size(); i++)
        sum += a[i];
    return sum;
}


int sum(int *a, int num)
{
    std::vector<int> c;
    for (int i = 0; i < num; i++)
        c.push_back(a[i]);
    return sum(c);
}