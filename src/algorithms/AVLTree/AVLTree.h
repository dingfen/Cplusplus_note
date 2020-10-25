#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <stack>

using std::cout;
using std::endl;
using std::ostream;
using std::setw;
using std::stack;

int LEVEL = 3;

template<class T=int>
class AVLNode {
public:
    T key;              // 节点 值
    AVLNode *parent;    // 父节点
    AVLNode *lchild;    // 左节点
    AVLNode *rchild;    // 右节点
    int depth;          // 左右子树高的差 左-右
public:
    AVLNode():lchild(nullptr), rchild(nullptr) {};
    AVLNode(T k):key(k), lchild(nullptr), rchild(nullptr), 
        parent(nullptr) {};

    int getDepth() {
        int l,r;
        if(this->lchild)
            l = this->lchild->getDepth();
        else l = 0;
        if(this->rchild)
            r = this->rchild->getDepth();
        else r = 0;

        depth = l - r;
        return std::max(l, r) + 1;
    }

    friend ostream & operator<<(ostream & out, const AVLNode<T> *e) {
        if (!e)
            return out;
        LEVEL++;
        out << e->key << " depth: " << e->depth << endl;
        if (e->lchild) {
            out << setw(LEVEL) << "L: " << e->lchild;
            LEVEL--;
        }
        if (e->rchild) {
            out << setw(LEVEL) << "R: " << e->rchild << endl;
        }
        return out;
    }

    bool operator<(const AVLNode<T> & a) {
        return this->key < a.key;
    }
};

template<class T=int>
class AVLTree {
private:
    AVLNode<T> *root;
    AVLNode<T>* leftRotation(AVLNode<T> *x);    // 左旋
    AVLNode<T>* rightRotation(AVLNode<T> *x);   // 右旋
    AVLNode<T>* findRotationType(AVLNode<T> *p, int *type); // 找到不平衡点
    void rebalanced(AVLNode<T> *x);             // 再平衡方案
public:
    AVLTree() = default;
    AVLTree(AVLNode<T> *proot): root(proot) {};
    AVLTree(AVLNode<T> * arrayNode, int num);
    ~AVLTree() = default;

    bool insert(const T &);
    bool insert(AVLNode<T> &);

    AVLNode<T> * Root() { return root; };
};

template<class T>
AVLTree<T>::AVLTree(AVLNode<T> * arrayNode, int num) {
    std::sort(arrayNode, arrayNode+num);
    root = arrayNode;
    auto temp = arrayNode;
    for(int i = 1; i < num; i++) {
        temp->rchild = &arrayNode[i];
        arrayNode[i].parent = temp;
        temp = &arrayNode[i];
    }
    // 旋转调整 从叶子出发 上回溯到根
    rebalanced(temp);
}


template<class T>
AVLNode<T>* AVLTree<T>::leftRotation(AVLNode<T> *x) {
    AVLNode<T> *y = x->lchild;
    x->lchild = y->rchild;
    if (y->rchild)
        y->rchild->parent = x;

    y->rchild = x;
    y->parent = x->parent;
    x->parent = y;
    if (x != root) {
        bool isLeftChild = x->parent->lchild == x;
        if (isLeftChild)
            x->parent->lchild = y;
        else
            x->parent->rchild = y;
    } else {
        root = y;
    }
    return y;
}


template<class T>
AVLNode<T>* AVLTree<T>::rightRotation(AVLNode<T> *x) {
    AVLNode<T> *y = x->rchild;
    x->rchild = y->lchild;
    if (y->lchild)
        y->lchild->parent = x;

    y->lchild = x;
    y->parent = x->parent;
    if (x != root) {
        bool isLeftChild = x->parent->lchild == x;
        if (isLeftChild)
            x->parent->lchild = y;
        else
            x->parent->rchild = y;
    } else {
        root = y;
    }
    x->parent = y;
    return y;
}


template<class T>
void AVLTree<T>::rebalanced(AVLNode<T> *x) {
    int type;
    do {
        root->getDepth();
        auto p = findRotationType(x, &type);
        switch (type) {
        case 1:
            x = leftRotation(p);
            break;
        case 2:
            x = rightRotation(p);
            break;
        case 3:
            rightRotation(p->lchild);
            x = leftRotation(p);
            break;
        case 4:
            leftRotation(p->rchild);
            x = rightRotation(p);
            break;
        default:
            break;
        }
    }while(type != -1);
}


template<class T>
AVLNode<T>* AVLTree<T>::findRotationType(AVLNode<T> *p, int *type) {
    while(p->parent) {
        auto q = p->parent;
        if(p == q->lchild) {
            if(p->depth >= 1 && q->depth > 1) { // case 1  左左
                *type = 1;
                return q;
            }
            if(p->depth <= -1 && q->depth > 1) {    // case 3 左右
                *type = 3;
                return q;
            }
        } else {
            if(p->depth <= -1 && q->depth < -1) {   // case 2 右右
                *type = 2;
                return q;
            }
            if(p->depth >= 1 && q->depth < -1) {    // case 4 右左
                *type = 4;
                return q;
            }
        }
        p = q;
    }
    *type = -1;
    return nullptr;
}


template<class T>
bool AVLTree<T>::insert(const T & key) {

}


template<class T>
bool AVLTree<T>::insert(AVLNode<T> & node) {
    AVLNode<T> *pt = root;
    if(!pt) {
        root = &node;
        return true;
    }
    // AVL树 按值插入
    while(1) {
        if(pt->key > node.key) {
            if(pt->lchild)
                pt = pt->lchild;
            else {
                pt->lchild = &node;
                node.parent = pt;
                break;
            }
        } else {
            if(pt->rchild)
                pt = pt->rchild;
            else {
                pt->rchild = &node;
                node.parent = pt;
                break;
            }
        }
    }
    // 旋转调整
    rebalanced(&node);
}

#endif // _AVL_TREE_H_