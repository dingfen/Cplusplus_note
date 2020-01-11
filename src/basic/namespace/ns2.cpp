#include <iostream>
#include "ns1.hpp"

namespace ns {
    std::string s;
    int p = 2;
} // namespace ns

using namespace ns;

int main(int argc, char const *argv[])
{
    ns::pl = 2;
    ns::p = 5;
    return 0;
}
