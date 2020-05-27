/*
 * 优先队列 priority_queue 算法导论 P 90
 */

#include <iostream>
#include <vector>

#define PARENT(i) (i/2)
#define LEFT(i) (2*i+1)
#define RIGHT(i) (2*i+2)

using namespace std;


void max_heapify(vector<double> &a, int heapsize, int i)
{
    int largest = i;
    while(i < heapsize) {
        int l = LEFT(i);
        int r = RIGHT(i);
        // 防止 l r 越界
        if(l<heapsize && a[l] > a[i]) {
            largest = l;
        }
        if(r<heapsize && a[r] > a[largest]) {
            largest = r;
        }
        if(largest != i) {
            // 交换 a[i] a[largest]
            double tmp = a[i];
            a[i] = a[largest];
            a[largest] = tmp;
            // 更新左右节点
            i = largest;
        }
        else {
            break;
        }
    }
}


double heap_maximum(vector<double> a)
{
    return a[0];
}

double heap_extract_max(vector<double> &a, int & heapsize)
{
    if(a.size() < 1) {
        cout << "heap is empty !"<<endl;
        exit(1);
    }
    double max = a[0];
    heapsize --;
    a[0] = a[heapsize];
    max_heapify(a, heapsize, 0);
    return max;
}

bool heap_increase_key(vector<double> &a, int i, double key)
{
    if(key < a[i]) {
        cout<<"new key is smaller than current key !"<<endl;
        return false;
    }
    else {
        a[i] = key;
        while(i > 0 && a[PARENT(i)] < a[i]) {
            double tmp = a[i];
            a[i] = a[PARENT(i)];
            a[PARENT(i)] = tmp;
            i = PARENT(i);
        }
        return true;
    }
}

void max_heap_insert(vector<double> &a, double key)
{
    a.push_back(INT32_MIN);
    heap_increase_key(a, a.size()-1, key);
}