/*
 * 线性时间排序 算法导论 P99
 * 适用于大量的整数 且范围确定的
 */
#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;

void counting_sort(vector<int> A, vector<int> &B, vector<int> &C, int k)
{
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

int main(int argc, char const *argv[])
{
    vector<int> A{2, 4, 1, 0, 4, 5, 3, 3, 6, 7, 8, 6, 2, 1, 0, 2};

    int k = *std::max_element(A.begin(), A.end());
    vector<int> B(A.size() + 1, 0);
    vector<int> C(k + 1, 0);
    counting_sort(A, B, C, k);
    for (int i = 1; i <= A.size(); i++)
        cout << B[i] << " ";
    cout << endl;
    // int a, b;
    // cin >> a >> b;
    // cout << C[b] - C[a] << endl;
}
