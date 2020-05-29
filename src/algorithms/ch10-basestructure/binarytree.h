#ifndef _BINARY_TREE_
#define _BINARY_TREE_

template<class T = int>
class TreeNode {
public:
    T val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(T x) : val(x), left(NULL), right(NULL) {}
};

#endif // _BINARY_TREE_