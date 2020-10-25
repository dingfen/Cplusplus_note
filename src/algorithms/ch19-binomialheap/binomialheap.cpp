/*
 * 二项堆 算法导论 P 279
 * make-heap    O(1)
 * insert       O(lgn)
 * minimum      O(lgn)
 * extract-min  O(lgn)
 * union        O(lgn)
 * decrease-key O(lgn)
 * delete       O(lgn)
 */

#include <iostream>

using namespace std;

// 二项堆节点 类
template <class T>
class binomialnode
{
public:
    /* data */
    T key;
    int degree;
    binomialnode *p;
    binomialnode *child;
    binomialnode *sibling;

    binomialnode():key(0),degree(0),p(nullptr),child(nullptr),sibling(nullptr) {}; 
    binomialnode(T key):key(key),degree(0),p(nullptr),child(nullptr),sibling(nullptr) {};
    binomialnode(T key, binomialnode *p):key(key),degree(0),p(p),child(nullptr),sibling(nullptr) {};
};

// 二项堆 类
template<class T>
class binomialheap
{
private:
    /* data */
    binomialnode<T> *root;

    // 使 z 成为 y 的父节点
    void link(binomialheap *y, binomialheap *z)
    {
        y->p = z;
        y->sibling = z->child;
        z->child = y;
        z->degree++;
    }

    // 简单的堆合并
    binomialnode<T> * merge_heap(binomialnode<T> *h1, binomialnode<T> *h2)
    {
        binomialnode<T> * root = nullptr;
        binomialnode<T> ** p = &root;
        while (h1 && h2) {
            if(h1->degree <= h2->degree) {
                *p = h1;
                h1 = h1->sibling;
            }
            else {
                *p = h2;
                h2 = h2->sibling;
            }
            p = & (*p)->sibling;
        }
        if(h1) *p = h1;
        else *p = h2;
        
    }
public:
    // 创建并返回一个不含任何元素的新堆
    binomialheap() {};
    ~binomialheap() {};
    
    // 将节点x插入到堆中
    void insert(T x)
    {
        binomialnode<T> *node;
        node = new binomialnode<T>(x);
        if(node == nullptr)
            return;
        root = union_heap(root, node);
    }

    // 删除一个堆节点
    void remove(binomialnode<T> *root, T key)
    {
       
    }

    // 减少关键字值
    void decreasekey(binomialnode<T> * node, T key) 
    {
        if(key >= node->key) {
            cout<<"That's Not decrease the key"<<endl;
            return ;
        }
        node->key = key;

        binomialnode<T> *child, *parent;
        child = node;
        parent = node->p;
        while (parent && child->key < parent->key) {
            auto tmp = parent->key;
            parent->key = child->key;
            child->key = tmp;
            
            child = parent;
            parent = child->p;
        }
        
    }

    // 返回一个指向堆中最小的关键字的指针
    binomialnode<T> * minimum()
    {
        binomialnode<T> *y = nullptr;
        binomialnode<T> *x = this;
        T min = INT32_MAX;
        while (x!=nullptr) {
            if(x->key < min) {
                min = x->key;
                y = x;
            }
            x = x->sibling;
        }
        return y;
    }
    
    // 将堆中最小关键字的堆提取并返回
    binomialnode<T> * extract_min()
    {
        
    }
    
    // 创建并返回一个包含有h1和h2两个堆的新堆
    binomialnode<T> * union_heap(binomialnode<T> *h1, binomialnode<T> * h2)
    {
        binomialnode<T> *root;
        binomialnode<T> *prev_x, *x, *next_x;

        // 先将h1和h2合并为一个简单的堆 再进行调整
        root = merge_heap(h1, h2);
        if(root == nullptr)
            return nullptr;
        
        prev_x = nullptr;
        x = root;
        next_x = x->sibling;

        while (next_x) {
            if(x->degree != next_x->degree || 
                (next_x->sibling && next_x->sibling->degree == x->degree)) {
                    prev_x = x;
                    x = next_x;
            }
            else if(x->key <= next_x->key) {
                x->sibling = next_x->sibling;
                link(next_x, x);
            }
            else {
                if(prev_x == nullptr)
                    root = next_x;
                else prev_x->sibling = next_x;
                link(x, next_x);
                x = next_x;
            }
            next_x = x->sibling;
        }
        return root;
    }
};


int main(int argc, char const *argv[])
{
    binomialheap<int> *head = new binomialheap<int>();

    delete head;
    return 0;
}
