/*
 * 冒泡排序 算法导论 P 23 2-2
 */

#include "test.h"

template<typename T>
void bubble_sort(vector<T> &a) {
    int len = a.size();
    for(int i = 0; i < len-1; i++)
        for(int j = len-1; j > i; j--) {
            if(a[j] < a[j-1]) {
                T tmp = a[j];
                a[j] = a[j-1];
                a[j-1] = tmp;
            }
        }
}
