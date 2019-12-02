#include "fun.h"
#include <stdio.h>


int main(int argc, char const *argv[])
{
    int vec[] = {3,4,5,6,7};
    int a = 4;
    int b = 5;
    // 调用了Cpp里面的函数
    int c = add(a, b);
    printf("%d\n", c);
    // 复杂点的例子
    int d = sum(vec, 5);
    printf("%d\n", d);
    return 0;
}
