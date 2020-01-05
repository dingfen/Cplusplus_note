#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template<class T>
class print {
public:
    void operator() (const T & elem) {
        cout << elem << endl;
    };
};

int main(int argc, char const *argv[])
{
    vector<int> a{1,2,3,4,5};
    double b[] = {2.0,3.5,3.2,5.6,1,7};
    // char *c;
    char d = 'a';
    // c = &d;

    for_each(a.begin(), a.end(), print<int>());
    for_each(b, b+5, print<double>());
    print<char>(c);
    return 0;
}
