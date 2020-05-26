#include "bubble_sort.h"
#include "insertion_sort.h"
#include "merge_sort.h"
#include <random>
#include <algorithm>

template<typename T>
void test_bubble(vector<T> a) {
    vector<T> tmp = a;
    std::sort(tmp.begin(), tmp.end());
    bubble_sort(a);
    int count = 0;
    for(int i = 0; i < a.size(); i++) {
        if (a[i] == tmp[i])
            count ++;
    }
    cout << "sort correct " << count << " / " << a.size() << endl;
}

template<typename T>
void test_insert(vector<T> a) {
    vector<T> tmp = a;
    std::sort(tmp.begin(), tmp.end());
    insertion_sort(a);
    int count = 0;
    for(int i = 0; i < a.size(); i++) {
        if (a[i] == tmp[i])
            count ++;
    }
    cout << "sort correct " << count << " / " << a.size() << endl;
}

template<typename T>
void test_merge(vector<T> a) {
    vector<T> tmp = a;
    std::sort(tmp.begin(), tmp.end());
    merge_sort(a, 0, a.size()-1);
    int count = 0;
    for(int i = 0; i < a.size(); i++) {
        if (a[i] == tmp[i])
            count ++;
    }
    cout << "sort correct " << count << " / " << a.size() << endl;
}

int main(int argc, char const *argv[])
{
    std::random_device rd;
    vector<int> a;
    if (argc < 2) {
        cout << "Enter the length of the array." << endl;
        return 1;
    }
    int len = std::stoi(argv[1]);
    for(int i = 0; i < len; i++) {
        a.push_back(rd() % 100);
    }

    test_bubble(a);
    test_insert(a);
    test_merge(a);
    return 0;
}
