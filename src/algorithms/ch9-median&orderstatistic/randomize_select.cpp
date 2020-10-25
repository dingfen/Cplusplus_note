/*
 * 在期望时间为线性时间内做选择
 * O(n) 随机选择一个支点进行划分
 * 处理划分的一边即可
 */ 

#include <iostream>
#include <vector>
#include <random>

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

double random_select(vector<double> &a, int p, int r, int i)
{
    if(p == r)
        return a[p];
    int q = random_partition(a, p, r);
    int k = q-p+1;
    if(i == k) {
        return a[q];
    }
    else if(i < k)
        return random_select(a, p, q-1, i);
    else return random_select(a, q+1, r, i-k);
}

int main(int argc, char const *argv[])
{
    default_random_engine e;
    uniform_real_distribution<double> u(1.0, 10.0);
    vector<double> a;
    for(int i=0;i<10;i++) {
        a.push_back(u(e));
    }
    int n;
    cout<<"Input the nth smallest in number: "<<endl;
    cin>>n;
    for(auto i : a)
        cout<<i<<" ";
    cout<<endl;
    cout<<random_select(a, 0, a.size()-1, n)<<endl;
    return 0;
}
