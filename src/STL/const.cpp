#include <iostream>
#include <vector>
using namespace std;

class C
{
private:
    string text;
public:
    C(string a) {text = a;};
    const char & operator[](size_t pos) const {
        return text[pos];
    };
    char & operator[](size_t pos) {
        return const_cast<char&>(static_cast<const C&>(*this)[pos]);
    };
    ~C() {};
};



int main(int argc, char const *argv[])
{
    vector<int> vec{2,3,4,5};
    vector<int>::iterator const it = vec.begin();
    const vector<int>::iterator it1 = vec.begin();
    *it1 = 3;
    *it = 5;
    // it1 = vec.end();
    vector<int>::const_iterator it2 = vec.cbegin();
    // *it2 = 5;
    ++it2;
    C a("abc");
    a[0] = 'b';
    const C b("bcd");
    // b[0] = 'a';
    cout<<b[0]<<endl;
    cout<<a[0]<<endl;
    return 0;
}