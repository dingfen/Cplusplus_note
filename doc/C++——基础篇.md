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

### 四、可变参数函数

在编写程序时，我们经常会遇到函数在编译期间接收到的参数数量不确定的情况。比如，经常使用的`printf()`函数，我们可以传入任意数量的参数。那么，这一切究竟如何实现？

事实上，C语言就已经提供了这样的一个解决方案，示例代码在[这里](../src/basic/vargument)：

```C++
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
```

 `add()` 最后一个参数写成省略号，即三个点号（**...**），省略号之前的那个参数是 `int`，代表了要传递的可变参数的总数。为了使用这个功能，需要使用 `stdarg.h` 头文件，该文件提供了实现可变参数功能的函数和宏。具体步骤如下：

1. 定义一个函数，最后一个参数为省略号，省略号前面可以设置自定义参数。

2. 在函数定义中创建一个 `va_list` 类型变量，该类型是在 `stdarg.h` 头文件中定义的。

3. 使用 `int` 参数和 `va_start` 宏来初始化 `va_list` 变量为一个参数列表。宏 `va_start` 是在 `stdarg.h` 头文件中定义的。

4. 使用` va_arg` 宏和 `va_list` 变量来访问参数列表中的每个项。

5. 使用宏` va_end` 来清理赋予 `va_list` 变量的内存。

除此之外，还有一个不被用到的`va_copy`，示例如下：

```C
    va_list valist1;
    va_list valist2;

    va_start(valist1, num);
    // valist2 is dest  after that valist2 equals to valist1
    va_copy(valist2, valist1);

    for (int i = 0; i < num; i++) {
        printf("%d, ", va_arg(valist2, int));
    }

    va_end(valist1);
    va_end(valist2);
```

### 五、C中的类型

C语言中有哪些类型是被重新命名的？它们的用处是什么？本质又是什么呢？

注意：在C++中用到`size_t`或其他在C中定义的类型时，推荐使用`#include<stddef.h>`。

| 类型                        | 实质                  | 描述                                                         |
| --------------------------- | --------------------- | ------------------------------------------------------------ |
| ptrdiff_t                   | signed int            | 两个指针相减的结果的有符号整数类型                           |
| size_t                      | unsigned int          | 通常用于数组的索引以及表示变量存储空间的大小                 |
| wchar_t                     | int                   | 宽字节字符类型，可以表示，在支持的语言环境中，指定的最大扩展字符集的所有成员 |
| nullptr_t<sup>C++11</sup>   | 关键字`nullptr`的类型 | 该类型表示它不是指向某个东西的指针类型，当多个重载函数接收不同的指针类型时，重载一个接收`std::nullptr_t`用于处理传入null指针的情况 |
| max_align_t<sup>C++11</sup> | 实现定义              | 一种对齐要求，至少与每个标量类型一样严格（大）的类型         |
| NULL                        | 宏 （void*) 0 or 0    | C语言中表示空指针，但在C++中仅表示0，C++中的空指针推荐使用`nullptr` |

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

类成员被`protected`关键字限制时，只能

- 该类内部的成员或者友员访问。
- 派生类的成员函数和友元函数可以使用继承下来的`protected`成员，但不能显示地直接使用该类来访问`protected`成员。

文字描述不太容易，示例程序更加清楚：

```C++
struct Base {
 protected:
    int i;
 private:
    void g(Base& b, struct Derived& d);
};
 
struct Derived : Base {
    void f(Base& b, Derived& d) // member function of a derived class
    {
        ++d.i; // okay: the type of d is Derived
        ++i; // okay: the type of the implied '*this' is Derived
//      ++b.i; // error: can't access a protected member through Base
//                 (Otherwise it would be possible to change other derived classes, 
//                  like a hypothetical Derived2, base implementation)
    }
    
    void f()
    {   // a pointer to a protected member must be used by derived class
//      int Base::* ptr = &Base::i;    // error: must name using Derived
        int Base::* ptr = &Derived::i; // okay
    }
};
 
void Base::g(Base& b, Derived& d) // member function of Base
{
    ++i; // okay
    ++b.i; // okay
    ++d.i; // okay
}
 
void x(Base& b, Derived& d) // non-member non-friend
{
//    ++b.i; // error: no access from non-member
//    ++d.i; // error: no access from non-member
}
```

<span id="three">

### 三、引用与指针</span>

C++的不同之处在于，它既支持指针，又支持引用。表面上看，指针和引用十分相似，但又有一点不同。

> [Pointers](https://www.geeksforgeeks.org/pointers-in-c-and-c-set-1-introduction-arithmetic-and-array/): A pointer is a variable that holds memory address of another variable. 
>
> [References](https://www.geeksforgeeks.org/references-in-c/) : A reference variable is an alias, that is, another name for an already existing variable. A reference, like a pointer, is also implemented by storing the address of an object. A reference can be thought of as a constant pointer (not to be confused with a pointer to a constant value!) with automatic indirection

从本质上讲，**指针**只不过是存了另一个变量的地址，而**引用**就是另一个变量的别名，但是在具体实现时，仍然只是存储了另一个变量的地址而已。

- 初始化时，指针可以不被初始化，但是引用必须在声明的同时初始化。因为它的本质就是给变量取名，初始化时没有变量是无法取名的，因此引用也不能被赋值为`NULL`
- 地址，指针变量自身有自己的地址，而引用变量与被引用者共享相同的地址（但也确实占了空间）。
- 指针可以指向指针，而引用不可以引用引用。`int &p = a; int &&q = p;`是错误的。

那么，什么时候该用指针，什么时候该用引用？

- 使用引用
  - 在传递函数的参数和返回值时
  - 需要避免拷贝带来的性能损失
- 使用指针
  - 需要用到指针偏移计算或者有时候传递`NULL`指针是必要的
  - 实现数据结构，比如树、链表等需要用指针

### 四、virtual相关

首先，我们来探究一下什么是虚函数(Virtual Function)。

虚函数就是指在基类中被声明，在派生类被重写的函数，并且该函数用一个重要的特性：**用一个声明为基类的指针，指向任意一个子类对象，调用相应的虚函数，那么具体调用哪一个函数将是在运行时动态指定的。**

因为编写代码的时候并不能确定到底调用基类函数还是哪个派生类函数，所以被成为虚函数。如果没有使用虚函数的话，则利用基类指针调用相应的函数的时候，将总被限制在基类函数本身，而无法调用到子类中被重写的函数，那么C++的多态就无法实现了。

那么到底虚函数有什么好处呢？我们从一个[例子](../src/basic/virtual)开始了解，顺便也提一下多态的概念

```C++
class Shape{
public:
    virtual void show() { cout << "I am Shape." <<endl; }
    void print() { cout << "It is Shape." <<endl; }
};

class Rectangle : public Shape {
public:
    void show() { cout << "I am Rectangle." <<endl; }
    void print() { cout << "It is Rectangle." <<endl; }
};

class Triangle : public Shape {
public:
    void show() { cout << "I am Triangle." <<endl; }
    void print() {cout << "It is Triangle." <<endl; }
};

class Circle : public Shape {
public:
    void show() { cout << "I am Circle." <<endl; }
    void print() { cout << "It is Circle." <<endl; }
};

Shape* spt[3];
Circle c;
Rectangle r;
Triangle t;    
    spt[0] = &c;
    spt[1] = &r;
    spt[2] = &t;
    for (int i = 0; i < 3; i++) {
        spt[i]->show();
        spt[i]->print();
    }
```

在该例中，我们定义了一个基类`Shape`和三个子类`Rectangle`、`Triangle`、`Circle`，并实现了两个函数

最后运行结果如下：当用`virtual`关键字时，该函数就是虚函数，可以在**运行时根据指针指向的具体类型来调用相应的函数**，而不是直接根据指针的类型在编译时决定调用的函数。

```
I am Circle.
It is Shape.
I am Rectangle.
It is Shape.
I am Triangle.
It is Shape.
```

运行时的多态只有通过基类的指针指向其派生类来实现。注意：如果在基类中的声明已经加上了`virtual`关键字，那么在派生类中的实现不需要添上`virtual`了。

### 五、抽象类

### 六、命名空间与作用域解析符

在学C++过程中，有时会遇到这样的语法

```C++
T* tmp = (T*)::operator new((size_t)sizeof(T));
T* tmp = new T;   // that's the same ?
```

一般情况下，`::`很好理解，就是特指一个class内部或者一个namespace内部的某一个变量或者函数，这样的写法可以使读者一目了然，比如`void A::fun()`就很容易明白指的是A类（或A空间）下的fun()函数。但是在`::`前什么都不写，表示的是什么？

其实是为了**访问全局变量**，具体说，当有一个局部变量与全局变量重名时，就需要用到这种方法去区分。

```C++
int x = 2;

int main(int argc, char const *argv[])
{
    int x = 5;
    std::cout << "global x" << ::x << std::endl;
    std::cout << "local x" << x << std::endl;
    return 0;
}
```

在明白了`::`的意思是调用全局变量后，再回到`::operator new`，事实上，我们平时使用的`new`都是重载于类的操作符，并不是全局的`new()`函数，平时我们写的这句话

```C++
Obj* p = new Obj(value);
```

等同于下列三句话

```C++
void* v = ::operator new(sizeof(Obj)); //just allocate memory, not do construction.
p = reinterpret_cast<Obj*>(v);
p->Obj::Obj(value);
```

上面的第一句仅仅是在做分配空间的活，没有进行对象构建，这就可以把分配空间和构建对象活动分开，这样精密性分工有利于STL提升效率。

匿名命名空间。简而言之，就是一个没有命名的命名空间，它的作用域也就是从文件中的声明点开始到末尾。如：

```C++
namespace {
  char c;
  int i;
  std::string s;
};
```

编译时，编译器会为这个没有名字的命名空间定义一个唯一的名字，还会在后面加上一天using指令，也就是说，上面的代码等价于：

```c++
namespace _COMPILER_GIVEN_NAME_ {
  char c;
  int i;
  std::string s;
};
using namespace _COMPILER_GIVEN_NAME_;
```

匿名命名空间的一个很重要的性质，就是匿名命名空间的变量都具有内部链接属性，名称的作用域被限制在了当前文件中，因为匿名命名空间没有名称，因此不能显式地使用`using`来使它在其他文件可用。这和声明为`static`的全局名称的链接属性是相同的，它无法通过在其他文件中使用`extern`声明来进行外部链接。

若不想使用全局`static`声明一个名称拥有内部链接属性，匿名命名空间可以作为一种更好的的方法。

## C++ 新特性

### 一、左值与右值

C++ 11新特性中，为了避免临时变量的拷贝，提高性能，提出了左值(lvalue)右值(rvalue)以及其他的概念。

首先，我们直观地理解一下左值(lvalue)与右值(rvalue)。左值一般出现在`=`左边，右值一般出现在`=`右边。在C++中，也有严格的定义。

> In C++ an *lvalue* is **something that points to a specific memory location**. On the other hand, a *rvalue* is **something that doesn't point anywhere**. It's also fun to think of lvalues as *containers* and rvalues as *things contained in the containers*. Without a container, they would expire.

```C++
int x = 666;  // ok
int y;
666 = y;  // error
```

在这里`666`当然是个右值，因为它没有特定的内存，而`x`是个左值，在程序运行时一定有自己的内存地址。下一句话就明显错误了，因为666仅仅是一个数字，没有内存地址，不可能将y存入的。

```C++
int *y = &x;
int *y = &666;
```

这里先是取了`x`的地址，将它放到`y`中。通过取地址符号`&`，可以将左值`x`传入然后变为一个右值`&x`。而`&666`是错误的，因为`666`没有地址，`&`的输入必须时左值，才可能正确运行。

---

在充分理解了左值和右值后，再来看一下下面的例子：

```C++
int setValue()
{
    return 6;
}

setValue() = 3; // error!

///////////////////////

int global = 100;

int& setGlobal()
{
    return global;    
}

// ... somewhere in main() ...

setGlobal() = 400; // OK it is useful when doing advanced stuff like implementing some overloaded operators.
```

为什么第一个程序是不正确的？因为第一个程序仅仅返回的时一个立即数`6`，是一个右值，因此不能在`=`的左边。而第二个程序，返回了一个`global`的引用，前面也提到过，引用本质就是别名，他指向了一个存在的内存位置`global`，因此是左值，那么这样操作就是正确的。

---

那么，左值与右值可以相互转换么？

首先，左值是可以转到右值的。比如`int z = x + y`就存在左值转到右值。根据C++的定义，`+`需要两个右值参数，相加后返回一个右值。但`x`和`y`显然都是左值，这里就需要一个隐藏的`lvalue-to-rvalue`转换。

右值不可以转到左值，这不是技术上的困难，C++设计者为了避免烦恼而这样规定的。

```C++
int y = 10;
int &yref = y; // ok you can do it

void fnc(const int &x) {...}
fnc(10); // ok
////////////////
int &yref = 10; // error

void fnc(int &x) {...}
fnc(10);	// also error
int x = 10;
fnc(x);		// that's ok
```

因为`10`没有特定的内存地址，而现在左值要取它的引用，指向一个存在的东西，这是矛盾的。从第二个例子来看，如果为了避免拷贝开销，使用引用`int &`，那么传入一个立即数是非法的，一定需要再声明一个变量才行。这就感觉非常不变且容易出错。解决办法就是使用`const int &`。

那么为什么`const int &x = 10`这样的语法就是可行的呢？直观来看，`10`这个立即数被认为是随时会变动并立马消亡的，因此对它做引用没有任何意义，但如果这个引用自身是常量，意味着指向的东西不能被更改，那么右值的这个易被更改的问题就得到了解决（有`const`限制）。这不是技术上的难题，而是为了避免麻烦才规定的。

在内部编译器实现中，事实上会发生这样的转换：

```C++
const int& ref = 10;

// ... would translate to:
int __internal_unique_name = 10;
const int& ref = __internal_unique_name;
```

### 二、引用

C++ 11新特性推出后，引用的特性又丰富起来。从[引用与指针](#Three)中我们知道了引用的一些C++ 98的用法，现在我们继续学习一些新特性新用法。

究竟什么是引用(Reference)？下面这句话很好地回答了这个问题：

> Declares a named variable as a reference, that is, an alias to an already-existing object or function.

因此，不管C++ 11的引用中有多少新的东西，其本质仍然是，某样东西的别名。

- 左值引用：alias  to an existing object

  - 更改引用就是在更改原变量本身
  - `const xxx &`就不能通过引用更改原变量
  - 当一个函数返回一个左值引用时，函数本身就可以被当做左值来赋值

  ```C++
      std::string s = "Ex";
      std::string& r1 = s;
      const std::string& r2 = s;
   
      r1 += "ample";           // modifies s
  //  r2 += "!";               // error: cannot modify through reference to const
      std::cout << r2 << '\n'; // prints s, which now holds "Example"
  ```

- 右值引用：used to extend the lifetimes of temporary objects. Through lvalue reference can extend lifttime, but can't modify through reference to const.

  ```C++
      std::string s1 = "Test";
  //  std::string&& r1 = s1;           // error: can't bind to lvalue
   
      const std::string& r2 = s1 + s1; // okay: lvalue reference to const extends lifetime
  //  r2 += "Test";                    // error: can't modify through reference to const
   
      std::string&& r3 = s1 + s1;      // okay: rvalue reference extends lifetime
      r3 += "Test";                    // okay: can modify through reference to non-const
      std::cout << r3 << '\n';
  ```

  右值引用的好处在于，它可以延长一些暂时的立即数或者结果的生命周期，让它更好地被用起来，减少拷贝过程中的性能消耗。

- 转发引用(Forwarding reference)：

- 悬挂引用(Dangling reference)：

他们的使用示例在[这里](../src/basic/reference)。

> reference to reference is not allowed, but it is permitted to form references to references through type manipulations in templates or typedefs

初看，这是一个非常令人困惑的说明，为何使用了`typedef`和`template`后，引用的引用就是合法的呢？事实上，这与`std::forward`的规则相关。

### 三、移动语义与转发

[参考本文](https://zhuanlan.zhihu.com/p/55856487)

在之前的C++ 98中，C++传值的方式都是拷贝传值，然而，若遇到一个比较大的参数，拷贝传值的开销非常大。例如：

```C++
void Demo::set(const string &s) {
    m_s = s;	// copy
}

Demo d;
d.set("a very very long string");
```

上面的代码在执行过程中，可能有下列几步操作：

1. 临时变量字符串先被构造，将字符串复制一遍
2. 临时变量将内容再复制到成员变量中
3. 临时变量被销毁

这里能不能优化一下呢? 临时变量反正都要被回收, 如果能直接把临时变量的内容, 和成员变量内容交换一下, 就能避免复制了呀，如下:

1. 成员变量内部的指针指向字符串所在的内存
2. 临时变量内部的指针指向成员变量以前所指向的内存
3. 临时变量被销毁

上面这个操作避免了一次拷贝，其实这就是所谓的move语义。

事实上，之前我们介绍的左值、右值引用等都是为了避免不必要的拷贝操作而引入的。

于是，现在避免拷贝操作时，我们需要如下两个函数

```C++
void Demo::set(const string &s) {
    m_s = s;	// copy
}

void Demo::set(const string &&s) {
    m_s = std::move(s);	// avoid unnecessary copy
}

Demo d;
string s1("a very very long string");
d.set(s1);
d.set("a very long string");	// temporary var use move not copy
```

于是，在传递的参数是左值时，使用第一个`set()`，而参数为临时变量时，使用第二个`set()`。可是，这很明显增加了代码的重复率！

这时候就需要用到转发了，相关示例代码在[这里](../src/basic/move_forward)

```C++
template<typename T>
void Demo::set(T && s) {
    m_s = std::forward<T>(s);
}

//when s is a rvalue, std::forward<T> equals to static_cast<[const] T1 &&>(s)
//when s is a lvalue, std::forward<T> equals to static_cast<[const] T1 &>(s)
```

那么有了`std::forward`为什么还保留`std::move`？首先，forward常用于template函数中，使用的时候必须要多带一个template参数T: `forward<T>`，代码略复杂。还有，明确只需要`move`的情况而用`forward`，代码意图不清晰，其他人看着理解起来比较费劲。更技术上来说，他们都可以被`static_cast`替代。为什么不用`static_cast`呢？ 也就是为了读着方便易懂。

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

## 关键字

### 1. extern关键字什么用处？

extern是C/C++语言中表明函数和全局**变量作用范围**的关键字，该关键字告诉编译器，其声明的函数和变量可以在本模块或其它模块中使用。

 通常，在模块的头文件中对**本模块提供给其它模块引用的函数和全局变量以关键字extern声明**。例如，如果模块B欲引用该模块A中定义的全局变量和函数时，只需包含模块A的头文件即可。这样，模块B中调用模块A中的函数时，在编译阶段，模块B虽然找不到该函数，但是并不会报错；它会在**链接阶段**中从模块A编译生成的目标代码中找到此函数。

在多个文件中，可以在一个文件（且只能在一个文件中）定义一个外部变量，使用该变量的其他文件必须使用关键字`extern`声明它。

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

### 5. mutable关键字含义？

