#include "Rational.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    Rational a(10,12);
    Rational b(30,42);
    Rational c = a*2.8;
    cout<<c;
    return 0;
}
