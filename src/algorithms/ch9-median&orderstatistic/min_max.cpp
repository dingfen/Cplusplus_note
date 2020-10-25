/*
 * 找最小最大值 算法导论 P 109
 * 在 3/2 *n的复杂度内
 */

#include <iostream>
#include <vector>
#include <random>
using namespace std;

typedef std::pair<double,double> group;

group min_max(vector<double> &a)
{
    group ans(0.0,0.0);
    int beg = 0;
    if(a.size() == 0)   return ans;
    if(a.size() % 2 == 1) {
        ans = {a[0],a[0]};
        beg = 1;
    }
    else {
        ans = {a[0],a[1]};
        beg = 2;
    }
    for(int i=beg;i<a.size();i+=2) {
        double max, min;
        if(a[i] < a[i+1]) {
            max = a[i+1];
            min = a[i];
        }
        else {
            max = a[i];
            min = a[i+1];
        }
        if(ans.second < max)
            ans.second = max;
        if(ans.first > min)
            ans.first = min;
    }
    return ans;
}

int main(int argc, char const *argv[])
{
    default_random_engine e;
    uniform_real_distribution<double> u(0.0, 10.0);
    vector<double> a;
    for(int i=0;i<15;i++) {
        a.push_back(u(e));
    }
    group ans = min_max(a);
    for(auto i : a) {
        cout<<i<<" ";
    }
    cout<<endl;
    cout<<ans.first<<", "<<ans.second<<endl;
    return 0;
}
