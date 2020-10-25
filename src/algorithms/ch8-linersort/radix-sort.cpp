/*
 * 基数排序 算法导论 P101
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void counting_sort(vector<int> A, vector<int> &B, int k)
{
    int C[k + 1];
    for (int i = 0; i <= k; i++)
        C[i] = 0;
    // 统计 A[j] == j 的个数
    for (int j = 0; j < A.size(); j++)
        C[A[j]]++;
    // 统计 A[j] <= j 的个数
    for (int i = 1; i <= k; i++)
        C[i] = C[i] + C[i - 1];
    for (int j = A.size() - 1; j >= 0; j--)
    {
        B[C[A[j]]] = A[j];
        C[A[j]]--;
    }
}

void radix_sort(vector<int> &a, int d)
{
    for (int i = 0; i < d; i++)
    {
        vector<int> b;
        b.clear();
        for (auto k : a)
        {
            int tmp = i;
            while (tmp > 0)
            {
                k /= 10;
                tmp--;
            }
            b.push_back(k % 10);
        }
        vector<int> c(a.size() + 1, 0);
        counting_sort(b, c, 10);
        for (auto j : c)
            cout << j << " ";
        cout << endl;
    }
}

int main(int argc, char const *argv[])
{
    vector<int> a{533, 513, 578, 464, 369, 741, 555, 748, 454, 333};
    radix_sort(a, 3);
    for (auto i : a)
        cout << i << " ";
    cout << endl;
    return 0;
}
