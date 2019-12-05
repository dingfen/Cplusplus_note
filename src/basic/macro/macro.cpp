#include <iostream>
#include <cstdio>

// 字符串连接 变为字符串
#define Con(x,y) x##y
#define ToString(x) #x

#define max(x, y) ({    \
    typeof(x) _max1 = (x);  \
    typeof(y) _max2= (y);   \
    (void) (&_max1 == &_max2);   /* 通过比较x,y的指针类型，得到警告 \
                                    若类型一致 通过void消除 */ \
    _max1 > _max2 ? _max1: _max2;   \
    })

// 使用宏快速定义一批变量
#define LIST_OF_VARIABLES \
	X(value1) \
    X(value2) \
    X(value3)

#define LISTS(z)   \
    Y(value1, z) \
    Y(value2, z) \
    Y(value3, z)

// 定义了 int value1 int value2 int value3
#define X(name) int name;
LIST_OF_VARIABLES
#undef X

#define Y(x, z) double x = (z);

#define print(t) printf("t = %d\n", t);
#define prints(x) printf("x = %s\n", x);

void fun0()
{
    int x = 29;
    int y = 31;
    int t = max(x, y);
    print(t)
    t = max(7*7, 2+9);
    print(t)
    // t = max(123.4, x);
    // print(t)
}

void fun1()
{
	int x  = Con(21,232);
	int y = Con(231, 79);
    print(x)
    print(y)

	char s[] = ToString(x234s);
	char d[] = ToString(123);
	char w[] = ToString(xyGWEW2); 
    prints(s)
    prints(d)
    prints(w)
}


void fun2()
{
#define X(name) printf("%s=%d\n",#name,name);
	LIST_OF_VARIABLES
#undef X

LISTS(2.0)
#define X(name) printf("%s = %lf\n",#name, name);
    LIST_OF_VARIABLES
#undef X
}

void fun3()
{
    printf("The file is %s\n", __FILE__);
    printf("now the line number is %d\n", __LINE__);
    printf("The time is %s\n", __TIME__);
    printf("Today is %s\n", __DATE__);
}

int main(int argc, char const *argv[])
{
    fun0();
    fun1();
    fun2();
    fun3();
    return 0;
}
