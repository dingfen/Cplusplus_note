#include "mytimer.hpp"

int main(int argc, char const *argv[])
{
    {
    mytimer timer;
    //mytimer timer2 = timer;
    int i = 0;
    int sum = 0;
    for (i = 0; i < 100000; i++)
        sum += i;
    }

    return 0;
}



// build 
// g++ -std=c++11 mytimer.hpp -o mytimer