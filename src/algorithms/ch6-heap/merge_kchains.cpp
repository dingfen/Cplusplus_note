/*
 * 设计一个时间复杂度为 O(nlgk)的算法
 * 能将k个有序的链表合并成一个有序的链表
 * 使用最小堆完成合并
 */

#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::priority_queue;
using std::cout;
using std::endl;

// 假设 链表用vector实现
vector<double> mergekchains(vector<double> links[], int k)
{
    typedef priority_queue<double, vector<double>, std::greater<double>> MinHeap;
    MinHeap h;
    vector<double> ret;
    vector<int> idx(k, -1);

    // init
    for(int i = 0; i < k; i++) {
        if (links[i].size() > 0) {
            h.push(links[i][0]);
            idx[i] = 0;
        }
    }
    while(!h.empty()) {        
        auto t = h.top();
        ret.push_back(t);
        h.pop();
        for(int i = 0; i < k; i++) {
            if (idx[i] != -1) {
                if(t == links[i][idx[i]]) {
                    idx[i]++;
                    if (links[i].size() > idx[i])
                        h.push(links[i][idx[i]]);
                    break;
                }
            }
        }
    }

    return ret;
}

int main(int argc, char const *argv[])
{
    int k = 3;
    vector<double> links[k];
    links[0] = {2,3,4,5,7,10,12};
    links[1] = {3.5,6.4,9.0};
    links[2] = {1,4,5.5,7};
    vector<double> ans = mergekchains(links, k);
    for(auto i : ans)
        cout<<i<<" ";
    cout<<endl;
    return 0;
}
