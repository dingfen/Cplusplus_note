#ifndef RBTREE__H
#define RBTREE__H

enum class Color {BLACK, RED};

template<class T=int>
class RBNode
{
private:
    T key;
    RBNode * lchild;
    RBNode * rchild;
    RBNode * parent;
    Color color;
public:
    RBNode(T key):key(key),lchild(nullptr),rchild(nullptr),parent(nullptr),color(Color::BLACK) {};
    ~RBNode();

    T getKey() {return key;};
    
};

template<class T=int>
class RBTree
{
private:
    /* data */
public:
    RBTree(/* args */);
    ~RBTree();
};


#endif