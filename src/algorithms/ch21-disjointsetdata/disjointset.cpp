/*
 * 不相交集合上的操作 链表表示
 */

#include <iostream>
#include <vector>

using namespace std;

class disjointset;

typedef struct linknode {
    struct linknode * proot;
    int key;
    struct linknode * next;
    disjointset * find_set;

    linknode(int key):proot(nullptr), key(key), next(nullptr) {};
    linknode() {};
    ~linknode() {};

    bool operator==(linknode a) {
        if(this->key == a.key)
            return true;
        else return false;
    }

    bool operator!=(linknode a) {
        if(this->key != a.key)
            return true;
        else return false;
    }

} LinkNode;

class disjointset
{
private:
    LinkNode * head;
    LinkNode * tail;
public:
    disjointset() {};
    disjointset(LinkNode & x) : head(&x), tail(&x) {x.proot = head; x.next = nullptr; };
    disjointset(LinkNode * h, LinkNode *t) : head(h), tail(t) {};
    ~disjointset() {};

    LinkNode * gethead() { return head; }
    LinkNode * gettail() { return tail; }

    disjointset union_set(disjointset y) {
        LinkNode * head_y = y.gethead();
        this->tail->next = head_y;
        while(head_y) {
            head_y->proot = this->head;
            head_y->find_set = this;
            head_y = head_y->next;
        }
        this->tail = y.gettail();
    }

    void print() {
        LinkNode *p = head;
        while(p) {
            cout<<p->key<<" ";
            p = p->next;
        }
        cout<<endl;
    }

    bool operator==(disjointset a) {
        if(*this->head == *a.gethead())
            return true;
        else return false;
    }
};


int main(int argc, char const *argv[])
{
    LinkNode nodes[10];
    disjointset sets[10];
    for(int i=0;i<10;i++) {
        nodes[i].key = i;
        disjointset tmp(nodes[i]);
        sets[i] = tmp;
        nodes[i].find_set = &sets[i];
    }
    int a,b;
    while (cin>>a>>b) {
        // 在同一个连通集合内
        if(a == -1 && b == -1)
            break;
        disjointset * pA = nodes[a-1].find_set;
        disjointset * pB = nodes[b-1].find_set;

        if(*pA == *pB) {
            cout<<"they are in same connect, nothing happened."<<endl;
        }
        else {
            pA->union_set(*pB);
        }
    }
    for(int i=0;i<10;i++) {
        sets[i].print();
    }
    
    return 0;
}
