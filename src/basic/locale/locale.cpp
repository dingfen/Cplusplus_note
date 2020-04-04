// imbue example
#include <iostream>     // std::cout
#include <locale>       // std::locale

int main()
{
  std::locale mylocale("zh_CN.UTF8");   // get global locale
  std::cout.imbue(mylocale);  // imbue global locale
  std::cout << 3.14159 << '\n';
  return 0;
}