/*
 * 在数组中找到第二大的元素
 * 算法导论 9.1-1 复杂度：n+lgn-2
 * 先用 建立堆提取出最大的元素 
 * 再从与最大元素比较过的元素中寻找第二大的
 */ 

#include <iostream>
#include <vector>
#include <map>
#include <random>

using namespace std;

double treecomp(vector<double> a, map<double, vector<double>> & record)
{
    vector<double> nexta;
    int beg = 0;

    if(a.size() == 1) {
        return a[0];
    }
    if(a.size() % 2 == 1) {
        beg = 1;
        nexta.push_back(a[0]);
    }
    for(int i=beg;i<a.size();i+=2) {
        if(a[i] > a[i+1]) {
            nexta.push_back(a[i]);
            record[a[i]].push_back(a[i+1]);
        }
        else {
            nexta.push_back(a[i+1]);
            record[a[i+1]].push_back(a[i]);
        }
    }
    return treecomp(nexta, record);
}

double vecmax(vector<double> a)
{
    double max = INT32_MIN;
    for(auto i : a) {
        if(i > max)
            max = i;
    }
    return max;
}


double secondmax(vector<double> a)
{
    // 记录下 某一个数字都比哪些数字大就行
    // 树形的比较
    map<double, vector<double>> record;
    double max = treecomp(a, record);
    cout<<"max "<<max<<endl;
    vector<double> rs = record[max];
    return vecmax(rs);
}

int main(int argc, char const *argv[])
{
    vector<double> a;
    default_random_engine e(5);
    uniform_real_distribution<double> u(0.0, 10.0);
    for(int i=0;i<10;i++)
        a.push_back(u(e));
    double ma = secondmax(a);
    for(auto i : a)
        cout<<i<<" ";
    cout<<endl;
    cout<<"second max "<<ma<<endl;
    return 0;
}
