/*
 * 算法导论 求逆序对数 2-4 P 24
 * 假设A[1..n]是一个有n个不同数的数组，若i<j且a[i]>a[j]，则对偶(i, j)称为A的一个逆序对(inversion)
 * 给出一个确定在n个元素的任何排列中逆序对数量的算法，最坏情况下需要o(nlgn)
 */

/*
 * 问题分析：
 * 算法的思路是从排序的过程中来计算逆序对，因为排序正是修正逆序的过程
 * 既然要控制在o(nlgn)以内，以现在接触的算法来说，就选用归并排序
 * 那么归并排序的过程中如何去确定逆序对的数量呢？
 * 首先，对于归并排序来讲，分解、解决、合并三步里的合并步骤是解决问题的关键，
 * 在这一步中，L与R两个子数组中的数都是相对有序的，并且L数组的下标都是小于R数组的下标的
 * 这样一来，要想出现逆序对就 只能是在L中出现某个值x比R数组中的值y大 
 * 而此时，排在x之后的所有值也必然要比y的值要大，这样我们就求得了在当前合并步骤中与y相关的逆序对的总量。
 * 由于在归并排序中，每次从L、R数组中取出的数都是未排元素中的最小值
 * 因此不会出现R数组中的某数y“丢失”逆序对的情况，因为此时L数组中所有的已经归并的元素绝不可能比y要大
 */

#include <iostream>
#include <vector>
using namespace std;

vector<double> a;

// 修改归并排序的代码
int merge(int p, int q, int r)
{
    int n1 = q - p + 1;
    int n2 = r - q;
    double L[n1 + 1];
    double R[n2 + 1];
    int inversion = 0;
    bool countable = false;
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
        if(!countable && L[i] > R[j]) {
            // L为升序 因此若L[i] > R[j] 后续的所有L数都大于R[j]
            inversion += n1 - i;
            countable = true;
        }
        if (L[i] <= R[j]) {
            a[k] = L[i];
            i++;
        }
        else {
            a[k] = R[j];
            j++;
            // R[j] 更新 因此计数器需要开始计数
            countable = false;
        }
    }
    return inversion;
}

int merge_sort(int p, int r)
{
    int inversions = 0;
    if(p < r) {
        int q = (p+r) / 2;
        inversions += merge_sort(p, q);
        inversions += merge_sort(q+1,r);
        inversions += merge(p,q,r);
        return inversions;
    }
    else return 0;
}

int main(int argc, char const *argv[])
{
    double n;
    while (cin>>n) {
        a.push_back(n);
    }
    // 需要写为a.size()-1，因为a[r]值是合法的
    int inversions = merge_sort(0, a.size()-1);
    for(auto i : a) 
        cout<<i<<" ";
    cout<<endl;
    cout<<"inversions : "<<inversions<<endl;
    return 0;
}
