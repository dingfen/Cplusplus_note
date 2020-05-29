/*
 * 快速排序 算法导论 P 86
 */

#include <iostream>
#include <vector>

using namespace std;

int partition(vector<double> &a, int p, int r)
{
    double x = a[r];
    int i = p-1;
    for(int j = p; j < r;j++) {
        if(a[j] <= x) {
            i++;
            double tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }
    double tmp = a[i+1];
    a[i+1] = a[r];
    a[r] = tmp;
    return i+1;
}

int random_partition(vector<double> &a, int p, int r)
{
    int i = rand() % (r-p+1) + p;
    double tmp = a[i];
    a[i] = a[r];
    a[r] = tmp;
    return partition(a, p, r);
}

void random_quicksort(vector<double> &a, int p, int r)
{
    if(p < r) {
        int q = random_partition(a, p, r);
        random_quicksort(a, p, q-1);
        random_quicksort(a, q+1, r);
    }
}

void quicksort(vector<double> &a, int p, int r)
{
    if(p < r) {
        int q = partition(a, p, r);
        quicksort(a, p, q-1);
        quicksort(a, q+1, r);
    }
}

int main(int argc, char const *argv[])
{
    vector<double> a{5,3,6,1,4,7,8,5,9,10};
    quicksort(a, 0, a.size()-1);
    for(auto i : a)
        cout<<i<<" ";
    cout<<endl;
    return 0;
}
