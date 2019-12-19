[TOC]

# C++——基础篇

## 与C相关

### 一、在C文件中引用C++函数

1. 将要使用的函数声明放在一个头文件中
2. 把要被c函数调用的c++函数的声明放在`extern "C"{ ... }`语句块里
3. 注意：标准c++的头文件包含不能放在`extern "C"{ ... }`语句块里

特别的，若要在C++文件调用C文件的函数也是可行的。

示例如下：

```c++
#ifdef __cplusplus
extern "C" {
#endif

int add(int a, int b);

int sum(int a[], int num);

#ifdef __cplusplus
}
#endif
```

详细实例可见[示例代码](../src/basic/c_use_cpp)

为什么要这么做?

> extern "C"的主要作用就是为了能够正确实现C++代码调用其他C语言代码。<font color='#FF8800'>加上extern "C"后，会指示编译器这部分代码按C语言的进行编译，而不是C++的。</font>由于C++支持函数重载，因此编译器编译函数的过程中会将函数的参数类型也加到编译后的代码中，而不仅仅是函数名；而C语言并不支持函数重载，因此编译C语言代码的函数时不会带上函数的参数类型，一般之包括函数名。

### 二、宏操作——陷阱

首先需要注意到，宏是C语言中的历史产物，C++中完全继承了C语言中的预处理器和宏操作。但是，还是**尽量不要用宏，改用内联函数或者模板代替**，

因为宏以及处理它的预处理器只是将用宏的地方进行简单的文本替换，根本不会进入symbol table，那么编译器在报错时就会出现魔数情况，这还只是最经常出现的小问题。

大问题在于括号有无或者`++`、`--`等情况，例如：

```C++
#define MAX(a,b) ((a)>(b)?(a):(b))
int i = 0;
int j = 2;
MAX(i++,j++)
```

那么i和j最终等于多少呢？得到的结果又是多少呢？非常烧脑的问题。而且就算我们理清了`i++`了几次，那么当宏的具体实现出现变化，结果可能会出现变化。

又比如：

```C++
#define DOSOMETHING() cmd1; \
					  cmd2; 

if (some_condition()) {
    DOSOMETHING()
}

if (some_condition())
    DOSOMETHING()
```

在定义宏时候，为避免用户使用不当（用户是真的不知道该用上面的哪一种），可以加一个小技巧：

```C++
#define DOSOMETHING() do {cmd1; cmd2; } while(0)
```

此时无论选取那一种写法，都是正确的。此外好处多多，在外部变量很多的时候，使用`do{}while(0);`，在里面可以定义变量而不用考虑变量名会同外部的重复。还有一个好处是，可以避免使用`goto`，使用`do {...} while(0)`当出现错误异常时，直接`break`就行，不需要`goto`。这在程序结构、编译优化上有很多好处。

因此，使用宏一定要在对宏有充分了解的情况下，不然有时候会出现非常多奇怪的问题。

### 三、宏操作——技巧

不过，即使宏的缺点很多，而且可以被内联函数、模板替代，但是仍有存在的意义。

`__FILE__` ：编译的文件的绝对路径；

`__LINE__` ：当前行号；

`__TIME__` ：当前时间；

`__DATE__` ：当前日期。

在定义一个类时候，总会发现在头文件中会有：

```C++
#ifndef XXXX_XXX_XX
#define XXXX_XXX_XX

class {
    // ...
}

#endif XXXX_XXX_XX
```

这是用宏来确保类不会被二次`include`，从而报出重定义的错误。不过也有`#pragma once`这种写法。

还有很多有趣的用法，比如：

```C++
// 字符串连接 将数字变为字符串
#define Con(x, y)  x##y
#define ToString(x) #x

// 用宏来求两个数的最大值  （仍建议使用内联函数）
 /* 通过比较x,y的指针类型，得到警告 若类型一致 通过void消除 */ 
#define max(x, y) ({    \
    typeof(x) _max1 = (x);  \
    typeof(y) _max2= (y);   \
    (void) (&_max1 == &_max2);  \
    _max1 > _max2 ? _max1: _max2;   \
    })

// 使用宏可以快速定义一批变量
#define LIST_OF_VARIABLIES \
        X(value1)  \
        X(value2)  \
        X(value3)
```

具体事例可以参见[如此代码](../src/basic/macro)

## C++类

### 一、不可拷贝类

在C++中定义一个类时，如果不明确定义拷贝构造函数和拷贝赋值操作符，编译器会自动生成这两个函数，虽然很方便，但是有时我们不需要某些类有这样的功能。

```C++
class empty_class {}

// 相当于增加了这两个函数
class empty_class {
    public:
    	empty_class(const empty_class &);
    	empty_class & operator=(const empty_class &);
}
```



在开发过程中，有时候我们希望构造的对象是不可以被拷贝复制的。比如在`boost::progress_timer`中，计时类`progress_timer`就是不可以被拷贝的，因为它在构造函数中开始计时，在析构函数中结束计时并输出，拷贝该类不仅没有意义，还会造成计时错误。此时只需要把拷贝构造函数和拷贝赋值操作符声明为`private`。但每次遇到这样的类都需要如此书写，不仅麻烦还容易出错，不过有一种更简单的办法

其实只需要：

```C++
class noncopyable
{
private:
    noncopyable(const noncopyable &);
    noncopyable & operator=(const noncopyable &);
protected:
    noncopyable() {}; 
    ~noncopyable() {};
};
```

将`noncopyable`类作为父类，那么子类都不能被拷贝，因为我们已经将拷贝构造函数和赋值函数全都声明为`private`。

在C++11中，还有更好、更清晰的写法，详细[示例代码](../src/basic/noncopyable)：

```C++
class noncopyable
{
    noncopyable(const noncopyable &) = delete;
    noncopyable & operator=(const noncopyable &) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};
```

### 二、protected相关

## 编译与操作系统相关

### 堆、栈、静态数据区

在阅读很多材料时，经常会提到某些变量是存放在堆/栈/全局区域里的，那么，到底这些都是什么呢？

事实上，一个程序在运行时会把内存空间分为这么几块区域

- 代码段(code segment)：被编译器生成的二进制代码段存放于此，通常是可读的
- 未初始化数据段(bss segment)：未被初始化的全局变量和静态变量存放在这里
- 初始化数据段(data segment)：初始化了的全局变量和静态变量存放在此
- 堆(heap)：动态申请内存的变量会被放在这里
- 调用栈(call stack)：局部变量、函数参数或者其他与函数相关的信息存放于此

---

可以看到，堆是一段被动态申请释放的内存空间。在C/C++中，使用`new`分配的空间，全部位于heap中，例如：

```C++
int *ptr = new int; // ptr is assigned 4 bytes in the heap
int *array = new int[10]; // array is assigned 40 bytes in the heap
```

内存的地址会被`operator new`传送过来存放到指针中。因此`delete`只是将已分配的内存还给堆，并没有将指针变量删去。

堆内的分配和释放没有限制，可以随时分配块并随时释放它，但**堆内的空间必须被显性地释放**，这就是为什么如果没有`delete`会导致内存泄漏。而且**堆内分配的空间是不连续的**，因此**跟踪堆的哪些部分被分配或空闲变得更加复杂**，分配内存也相对较慢，好处是**堆的空间非常大**，因此许多大规模的数组或者结构体都不得不存放在这里。有许多用户自定义的堆分配器可用于针对不同的使用模式调整堆性能。

---

栈一般指的都是调用栈(call stack)，在程序运行时，栈中存放着所有正在被执行的函数，以及它们的局部变量。

```C++
int main()
{
    char name[25]; // name is assigned 25 bytes in the stack
    int len;
}
```

栈是作为程序执行的暂存空间。调用函数时，在堆栈顶部保留一个块，用于本地变量和其他数据。当该函数返回时，该块就会被清空，并且在下次调用函数时使用。栈始终按照LIFO的原则弹出或压入块。最近放入的块始终是最先会被释放的块。因此栈的实现非常简单，而且分配的空间也是连续的，从堆栈中释放块只不过是调整一个指针。

栈的内存空间是比较小的，Visual Studio默认栈的大小为1MB，当变量使用的内存超过这一限制时，操作系统会报出stack overflow的错误然后将程序关闭。

下面我们来看这样的一个简单的程序，探讨一下这些变量会在哪里存放地址

```C++
const int A = 10;       // global in data segment
int a = 20;             // global in data segment
static int b = 30;      // global in data segment
int c;                  // global in bss segment

int main(int argc, char const *argv[])
{
    static int d = 40;      // local in data segment 
    char e[] = "hello";     // local in the stack "hello" is in .rodata
    register int f = 50;    // `register` tells the compiler that the var will be heavily 							  // used and recommend to be kept in register if possible.
    void *p1 = (char *)malloc(10);  // local for p1 pointer, but 10 bytes in the heap
    return 0;
}
```

## 疑问解答

### 1. extern关键字什么用处？

extern是C/C++语言中表明函数和全局**变量作用范围**的关键字，该关键字告诉编译器，其声明的函数和变量可以在本模块或其它模块中使用。

 通常，在模块的头文件中对**本模块提供给其它模块引用的函数和全局变量以关键字extern声明**。例如，如果模块B欲引用该模块A中定义的全局变量和函数时只需包含模块A的头文件即可。这样，模块B中调用模块A中的函数时，在编译阶段，模块B虽然找不到该函数，但是并不会报错；它会在**链接阶段**中从模块A编译生成的目标代码中找到此函数。

### 2. static关键字什么用处？

`static`关键字有很多用法，不同用法含义也不同。

- 函数中的`static`变量

  > When a variable is declared as static, space for **it gets allocated for the lifetime of the program**. Even if the function is called multiple times, space for the static variable is allocated only once and the value of variable in the previous call gets carried through the next function call.

- 类中的`static`变量

  > As the variables declared as static are initialized only once as they are allocated space in separate static storage so, the static variables **in a class are shared by the objects.** There can not be multiple copies of same static variables for different objects. Also because of this reason static variables can not be initialized using constructors.
  >
  > a static variable inside a class should be initialized explicitly by the user using the class name and scope resolution operator outside the class

- 类中的`static`函数

  > Just like the static data members or static variables inside the class, static member functions also does not depend on object of class. We are allowed to invoke a static member function using the object and the `.` operator but it is recommended to invoke the static members using the class name and the scope resolution operator.
  > **Static member functions are allowed to access only the static data members or other static member functions**, they can not access the non-static data members or member functions of the class.

### 3. constexpr关键字什么用处

`constexpr`表示，被声明的函数或变量的值可以在编译期间被计算出来。那么这些变量或者函数的值就可以被当做编译期间的常量去使用

### 4. final关键字是什么含义？

> Specifies that a [virtual function](https://en.cppreference.com/w/cpp/language/virtual) cannot be overridden in a derived class or that a class cannot be [inherited from](https://en.cppreference.com/w/cpp/language/derived_class).（译：指示 在继承类中虚函数不能再被重载或者该类不能再被派生）

当被用于成员函数时，`final`紧跟着成员函数的声明或定义部分出现;当被用于一个类时，`final`出现在类定义开始之处，紧跟着类名。

个人理解为，`final`是为了防止一个虚函数被其他程序员再重载，从而其实现被覆盖;也防止一个类被派生。