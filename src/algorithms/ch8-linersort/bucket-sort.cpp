/*
 * 桶排序 算法导论 P 102
 * 适用于 均匀分布的 在[0,1]内的随机数
 */
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
using namespace std;

vector<double> bucketsort(vector<double> &a)
{
    int n = a.size();
    vector<double> lists[10];
    vector<double> ans;
    for (int i = 0; i < n; i++)
    {
        lists[(int)floor(10 * a[i])].push_back(a[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        sort(lists[i].begin(), lists[i].end());
    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < lists[i].size(); j++)
            ans.push_back(lists[i][j]);
    }
    return ans;
}

int main(int argc, char const *argv[])
{
    vector<double> a;
    vector<double> ans;
    default_random_engine e;
    uniform_real_distribution<double> u(0.0, 1.0);
    for (int i = 0; i < 25; i++)
    {
        a.push_back(u(e));
    }
    ans = bucketsort(a);
    for (auto i : ans)
        cout << i << " ";
    cout << endl;
    return 0;
}
