#include "AVLTree.h"

#include <cassert>

int main(int argc, char const *argv[])
{
    const int num = 5;
    AVLNode<int> array[num];

    for (int i = 0; i < num; i++) {
        array[i].key = 1+i;
    }
    AVLTree<int> t(array, num);
    cout << t.Root() << endl;
    AVLNode<int> a(6);
    t.insert(a);
    cout << t.Root() << endl;
    return 0;
}
