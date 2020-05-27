/*
 * 堆的性质 heap 算法导论 P 74
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

void build_max_heap(vector<double> &a)
{
    for(int i=a.size()/2-1;i>=0;i--) {
        max_heapify(a, a.size(),i);
    }
}

void heapsort(vector<double> &a)
{
    build_max_heap(a);
    for(auto i : a) 
        cout<<i <<" ";
    cout<<endl;
    int heapsize = a.size();
    for(int i=a.size()-1;i>0;i--) {
        // 提出最大的元素
        double tmp = a[0];
        a[0] = a[i];
        a[i] = tmp;
        heapsize --;
        max_heapify(a, heapsize, 0);
        for(auto i : a) 
            cout<<i <<" ";
        cout<<endl;
    }
}

int main(int argc, char const *argv[])
{
    vector<double> h{1,0,3,5,4,12,7,8,9,10,11};
    heapsort(h);
    for(auto i : h)
        cout<<i<<" ";
    cout<<endl;
    return 0;
}
