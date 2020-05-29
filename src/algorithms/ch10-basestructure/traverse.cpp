#include <queue>
#include <iostream>
#include <vector>
#include "binarytree.h"

using std::cout;
using std::endl;
using std::queue;
using std::vector;

template<typename T>
vector<vector<T>> levelOrder(TreeNode<T>* root) {
    vector<vector<T>> ret;
    if (!root)
        return ret;

    queue<TreeNode<T> *> q;
    q.push(root);
    while (!q.empty()) {
        int size = q.size();
        ret.push_back(vector<T>());
        for(int i = 0; i < size; i++) {
            auto pt = q.front();
            q.pop();
            ret.back().push_back(pt->val);
            if (pt->left) q.push(pt->left);
            if (pt->right) q.push(pt->right);
        }
    }
    return ret;
}

int main(int argc, char const *argv[])
{
    return 0;
}
