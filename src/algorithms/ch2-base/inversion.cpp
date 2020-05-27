#include <iostream>
#include <vector>
using namespace std;

vector<double> a;

// 修改归并排序的代码
int merge(int p, int q, int r)
{
    int n1 = q - p + 1;
    int n2 = r - q;
    double L[n1 + 1];
    double R[n2 + 1];
    int inversion = 0;
    bool countable = false;
    for (int i = 0; i < n1; i++)
        // 数组起始位置为 p
        L[i] = a[i + p];
    for (int i = 0; i < n2; i++)
        // 数组起始位置为q+1
        R[i] = a[i + q + 1];
    L[n1] = INT32_MAX;
    R[n2] = INT32_MAX;
    int i = 0;
    int j = 0;
    // [p, r] p和r都是合法可以取到的
    for (int k = p; k <= r; k++) {
        if(!countable && L[i] > R[j]) {
            // L为升序 因此若L[i] > R[j] 后续的所有L数都大于R[j]
            inversion += n1 - i;
            countable = true;
        }
        if (L[i] <= R[j]) {
            a[k] = L[i];
            i++;
        }
        else {
            a[k] = R[j];
            j++;
            // R[j] 更新 因此计数器需要开始计数
            countable = false;
        }
    }
    return inversion;
}

int merge_sort(int p, int r)
{
    int inversions = 0;
    if(p < r) {
        int q = (p+r) / 2;
        inversions += merge_sort(p, q);
        inversions += merge_sort(q+1,r);
        inversions += merge(p,q,r);
        return inversions;
    }
    else return 0;
}

int main(int argc, char const *argv[])
{
    double n;
    while (cin>>n) {
        a.push_back(n);
    }
    // 需要写为a.size()-1，因为a[r]值是合法的
    int inversions = merge_sort(0, a.size()-1);
    for(auto i : a) 
        cout<<i<<" ";
    cout<<endl;
    cout<<"inversions : "<<inversions<<endl;
    return 0;
}
