#include <stdio.h>
#include <stdarg.h>

int add(int num, ...) {
    va_list valist;
    int sum = 0;
    int i;

    // 为 传入的参数初始化 valist
    va_start(valist, num);

    // 访问 所有的参数
    for(i = 0; i < num; i++) {
        sum += va_arg(valist, int);
    }

    // 结束调用valist 清理内存
    va_end(valist);

    return sum;
}


int copy(int num, ...) {
    va_list valist1;
    va_list valist2;

    va_start(valist1, num);
    va_copy(valist2, valist1);

    for (int i = 0; i < num; i++) {
        printf("%d, ", va_arg(valist2, int));
    }

    va_end(valist1);
    va_end(valist2);
}

int main(int argc, char const *argv[])
{
    printf("sum = %d\n", add(5, 1, 2, 3, 4, 5));

    // 如果 参数数目不一致呢？
    printf("sum = %d\n", add(5, 2, 3, 4, 5));   // 结果错误
    printf("sum = %d\n", add(5, 1, 2, 3, 4, 5, 6));

    copy(5, 2,3,4,5,6);
    return 0;
}
