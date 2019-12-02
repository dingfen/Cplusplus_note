#ifndef MY_TIMER_HPP_
#define MY_TIMER_HPP_

#include "noncopyable.hpp"
#include <ctime>
#include <iostream>

class mytimer : public noncopyable
{
private:
    std::clock_t clock;
public:
    mytimer();
    ~mytimer();
};

mytimer::mytimer()
{
    clock = std::clock();
}

mytimer::~mytimer()
{
    std::cout << (std::clock() - clock) << std::endl;
}

#endif