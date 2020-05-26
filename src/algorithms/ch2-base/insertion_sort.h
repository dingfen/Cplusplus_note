/* 插入排序的实现  算法导论 P 10 
 * 输入：未排序的序列
 * 输出：排好序的升序序列
 */ 
#include "test.h"

template<typename T>
void insertion_sort(vector<T> &a) {
    if(a.size() < 2) 
        return ;
    // 插入排序
    for(int j = 1;j < a.size(); j++) {
        T key = a[j];
        int i = j-1;
        while (i >= 0 && a[i] > key) {
            a[i+1] = a[i];
            i--; 
        }
        a[i+1] = key;
    }
}


