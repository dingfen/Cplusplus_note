/*
 * 归并排序算法 P 17
 */

#include "test.h"

template<typename T>
void merge(vector<T> &a, int p, int q, int r) {
    int n1 = q - p + 1;
    int n2 = r - q;
    T L[n1 + 1];
    T R[n2 + 1];
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
        if (L[i] < R[j]) {
            a[k] = L[i];
            i++;
        }
        else {
            a[k] = R[j];
            j++;
        }
    }
}

template<typename T>
void merge_sort(vector<T> &a, int p, int r)
{
    if(p < r) {
        int q = (p+r) / 2;
        merge_sort(a, p, q);
        merge_sort(a, q+1,r);
        merge(a, p, q, r);
    } 
}
