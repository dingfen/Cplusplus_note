#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
    #if defined(__sgi)
        cout << "__sgi" <<endl;
    #endif

    #if defined(__GNUC__)
        cout << "__GNUC__" << endl;
        cout << __GNUC__ << ' ' << __GNUC_MINOR__ << endl;
    #endif
    
    return 0;
}
