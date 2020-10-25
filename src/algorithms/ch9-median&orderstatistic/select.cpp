/*
 * 最坏时间为线性时间的选择算法
 * 1 把输入的数组 5个元素5个元素地分为若干组 最后一组为余数
 * 2 寻找每个组的中位数 即对5个元素进行插入排序 选第三个
 * 3 对 2 中找到的中位数 递归调用算法寻找中位数x
 * 4 利用partition 将x作为支点划分数组 k是比x小的元素个数+1
 * 5 所以x是第k个小的 递归调用算法 找到第i个小的
 */
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

void insertion_sort(vector<double> &a)
{
    if(a.size() < 2) return ;
    // 插入排序
    for(int j=1;j<a.size();j++) {
        double key = a[j];
        int i = j-1;
        while (i >= 0 && a[i] > key) {
            a[i+1] = a[i];
            i--; 
        }
        a[i+1] = key;
    }
}

int partition(vector<double> &a, int p, int r, double x)
{
    int i = p;
    r--;
    while(a[i]!=x) i++;
    double tmp = a[i];
    a[i] = a[p];
    a[p] = tmp;
    while(p < r) {
        while(p<r && a[r] >= x) r--;
        a[p] = a[r];
        while(p<r && a[p] <= x) p++;
        a[r] = a[p];
    }
    a[p]=x;
    return p+1;
}

double select(vector<double> &a, int p, int r, int i)
{
    vector<double> median;
    vector<double> tmp;
    // 5个元素一组 进行插入排序 并找到中位数
    for(int i=p;i<=r;i++) {
        tmp.push_back(a[i]);
        if(tmp.size() == 5) {
            insertion_sort(tmp);
            median.push_back(tmp[2]);
            tmp.clear();
        }
    }
    // 处理余数部分
    if(!tmp.empty()) {
        insertion_sort(tmp);
        median.push_back(tmp[tmp.size()/2]);
        tmp.clear();
    }
    for(auto i : a)
        cout<<i<<" ";
    cout<<endl;
    cout<<"Median :";
    for(auto i : median)
        cout<<i<<" ";
    cout<<endl;
    // 中位数的中位数x
    double x;
    if(median.size() == 1)
        x = median[0];
    else 
        x = select(median, 0, median.size()-1, median.size()/2);
    // 根据x进行划分 调用划分函数 得到 x 是第k小的
    int k = partition(a, p, r, x);
    cout<<"k x i "<<k<<" "<<x<<" "<<i<<endl;
    for(auto i : a)
        cout<<i<<" ";
    cout<<"**********"<<endl;
    if(i == k)
        return x;
    else if(i < k)
        return select(a, p, k-1, i);
    else return select(a, k+1, r, i-k);
}

int main(int argc, char const *argv[])
{
    vector<double> a{5, 2.2, 3.4, 9.5, 6.4, 8.4, 7.6, 
        1.2, 3, 6.8, 9, 1, 11, 3.5, 3.9, 5.5, 1.5, 0.5};
    vector<double> b;
    int n;
    default_random_engine e;
    uniform_real_distribution<double> u(0.0, 12.0);
    for(int i=0;i<20;i++) 
        b.push_back(u(e));

    cout<<"Input the nth smallest "<<endl;
    cin>>n;
    while(n<1 || n>a.size()) {
        cout<<"Oops, n is an illegal num."<<endl;
        cin>>n;
    }
    double ans = select(a, 0, a.size()-1, n);
    cout<<ans<<endl;
    return 0;
}
