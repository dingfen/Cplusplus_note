[TOC]

# C++——基础篇

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

### 五、抽象类与纯虚函数

什么是抽象类？包含了纯虚函数的类就是抽象类，那么什么是纯虚函数呢，形如：

```C++
virtual void func() = 0;
```

的函数（即虚函数后面加`= 0`），**该函数的实现不是由该类提供的，而是由派生类提供的。**因此，纯虚函数要求派生类必须提供实现，否则编译会失败。

抽象类，即形如：

```C++
// An abstract class 
class Test 
{    
    // Data members of class 
public: 
    // Pure Virtual Function 
    virtual void show() = 0; 
   /* Other members */
}; 
```

更多[示例](../src/basic/abstract)。

抽象类应用注意点

- 抽象类不能被**实例化**，使用时，只能构造出pointer to abstract class
- 抽象类一般是用于接口(Interface)，即类中所有的函数都是纯虚函数，接口可以被看作为方法声明的集合，使用抽象类作为接口，利于工程开发

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

匿名命名空间的一个很重要的性质，就是匿名命名空间的变量都具有内部链接属性，名称的作用域被限制在了当前文件中，因为匿名命名空间没有名称，因此不能显式地使用`using`来使它在其他文件可用。**这和声明为`static`的全局名称的链接属性是相同的**，它无法通过在其他文件中使用`extern`声明来进行外部链接。

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

### 四、Lambda表达式

其他主流的语言（如Python、Java等）都提供了`Lambda`表达式的语法，C++ 11也加入了这个内容。

为了描述一个`lambda`表达式，你必须提供：

* 捕捉列表：除了形参之外，表达式可以使用的变量列表。
  * `[&]` 意味着**所有的局部变量都将按照引用的方式进行传递**。
  * `[]` 意味着不需要捕捉任何变量。
  * `[=]` 表示值传递。
  * `[a, &b]` `a`以值方式被表达式捕获，`b` 则是以引用的方式；
  * `[this]` 以值的方式捕获 this 指针。
* （可选的）所有参数及其类型（例如：` (int a, int b)` ）。
* 组织成一个块的函数行为（例如：`{ return v[a].name < v[b].name; }` ）。
* （可选的）使用**返回值类型后置语法**（下文会介绍）来指明返回类型。但一般情况下，我们仅从 return 语句中去推断返回类型，如果没有返回任何值，则推断为 void。

Lambda表达式可以与STL算法一起使用。例如：

```C++
for_each(arr, arr+5, [](int n) { cout << n << " "; });
```

使用`lambda` 函数的最简单方法之一是将其存储在一个变量中，然后就像使用一个函数名一样使用该变量，这里就需要 `auto` 关键字了。例如：

```C++
auto add = [](int a, int b) {return abs(a) + abs(b); };

int sum = add(5, -6);
```

对于`lambda`表达式，有人认为这“相当简洁”，也有人认为这是一种可能产生危险且晦涩的代码的方式。我认为，这两种观点都正确。**如果某一函数的行为既不通用也不简单，那么建议采用命名函数对象或者函数。**

### 五、返回类型后置语法

考虑下面这段代码：

```
template<class T, class U>
??? mul(T x, U y)
{
    return x*y;
}
```

函数`mul()`的返回类型要怎么写呢？当然，是“x×y类型”，但是这并不是一个数据类型，我们如何才能一开始就得到它的真实数据类型呢？C++ 11 之后，程序员就可以 `decltype` 来推断“x×y”的数据类型。C++ 11将返回类型放在它所属的函数名的后面：

```C++
template<class T, class U>
auto mul(T x, U y) -> decltype(x*y) // 注意这里的作用域
{
    return x*y;
}
```

这里需要使用`auto`关键字，常见的`auto` 在C++ 11中还有根据初始值推导数据类型的意义，在这里它的意思变为“返回类型将会稍后引出或指定”。

## 杂项

### regex

虽然使用C/C++来处理正则化表达式的情景已经不多见（而且也很麻烦），但是在后端开发中处理xml或html信息时，会一些regex的API还是可以省下很多麻烦的。这里重点介绍C++ 11中的regex运用，关于C语言中使用regex，可以参考如下[示例](../src/basic/regex/regex.c)。

首先，介绍一些常用元字符：

| 符号   | 意义                                                         | 注意                                        |
| ------ | ------------------------------------------------------------ | ------------------------------------------- |
| .      | 匹配除"\n"之外的任何单个字符                                 |                                             |
| ^      | 匹配输入字符串的开始位置，不匹配任何字符                     |                                             |
| $      | 匹配输入字符串结尾的位置，不匹配任何字符                     |                                             |
| *      | 零次或多次匹配前面的字符或子表达式                           |                                             |
| +      | 一次或多次匹配前面的字符或子表达式                           |                                             |
| ?      | 前面的字符或子表达式 出现或不出现                            |                                             |
| \|     | 将两个匹配条件进行逻辑"或"运算                               |                                             |
| \      | 将下一字符标记为特殊字符、文本、反向引用或八进制转义符       | ^ 字符本身需使用 <br>\^$ . *也是如此        |
| [xyz]  | 字符集，匹配包含的任一字符                                   | [a-z]表示所有小写字母                       |
| [^xyz] | 反向字符集，匹配未包含的任何字符                             | [^a-z]表示不是小写字母                      |
| {n,m}  | ”n”和”m”是非负整数，n<=m，匹配至少n次，至多m次               | {n}正好出现n次<br>{n,}至少出现n次           |
| ( )    | 将(和)之间的表达式定义为组，并且将匹配这个表达式的字符保存到一个临时区域 | 最多可以保存9个<br>用”\1”到”\9”的符号来引用 |

还有一些转义字符，使用它们表示更加方便

| 符号 | 意义                                                 | 注意                 |
| ---- | ---------------------------------------------------- | -------------------- |
| \w   | 匹配字母或数字或下划线，任意一个字母或数字或下划线   |                      |
| \W   | 匹配任意不是字母、数字、下划线的字符                 |                      |
| \s   | 匹配任意的空白符，含空格、制表符、换页符等的任意一个 | 与”[\f\n\r\t\v]”等效 |
| \S   | 匹配任意不是空白符的字符                             |                      |
| \d   | 匹配数字，任意一个数字，0~9中的任意一个              | 等效于”[0-9]”        |
| \D   | 匹配任意非数字的字符                                 |                      |
| \b   | 匹配一个字边界，即字与空格间的位置，不匹配任何字符   | "er\b"匹配"never"    |
| \B   | 非字边界匹配                                         |                      |
| \f   | 匹配一个换页符                                       | 等价于”\x0c”和”\cL”  |
| \n   | 匹配一个换行符                                       | 等价于”\x0a”和”\cJ”  |
| \t   | 匹配一个制表符                                       | 等价于”\x09”和”\cI”  |
| \v   | 匹配一个垂直制表符                                   | 等价于”\x0b”和”\cK”  |
| \cx  | 匹配”x”指示的控制字符                                | \cM匹配Ctl-M或回车符 |
| \r   | 匹配一个回车符                                       | 等价于”\x0d”和”\cM”  |

下面，我们来看一下一些常用的API

- `std::regex_match`全文匹配，要求整个字符串都要符合匹配规则，返回`bool`true（匹配成功）或false（匹配失败），可以加入一个可选参数`regex_constants::match_flag_type`，指示如何匹配。有时候，我们不关心字符串是否与regex匹配，还需要匹配的结果，那么可以这么做：

  ```C++
  const char cstr[] = "subject";
  std::regex e("(sub)(.*)");
  std::cmatch cm; // same as std::match_results<const char*> cm;
  std::regex_match(cstr, cm, e);	// cm is the vector of results
  
  std::string s("subject");
  std::smatch sm;	// same as std::match_results<string::const_iterator> sm;
  std::regex_match(s, sm, e);
  ```

  更多详细[示例](../src/basic/regex/regex_match.cpp)

- `std::regex_search`搜索匹配，搜索字符串中符合正则化规则的子串，参数与返回类型与`std::regex_match`一致，详细[示例](../src/basic/regex/regex_search.cpp)

- `std::regex_replace`替换匹配，将符合匹配规则的字符串替换成另一个字符串，详细[示例](../src/basic/regex/regex_search.cpp)

  ```C++
  std::string s ("there is a subsequence in the string\n");
  std::regex e ("\\b(sub)([^ ]*)");   // matches words beginning by "sub"
  
  // using string/c-string (3) version:
  std::cout << std::regex_replace (s,e,"sub-$2");
  ```

### locale

C/C++程序中，**locale（即系统区域设置），用于决定程序所使用的当前语言编码、日期格式、数字格式及其它与区域有关的设置，**locale设置的正确与否将影响到程序中字符串处理（`wchar_t`如何输出、`strftime()`的格式等）。

C与C++的locale是独立的，接下来，我们分别讨论它们的用法。

C标准库中使用`locale.h`头文件定义了特定地域的设置，比如日期格式和货币符号。库中含有一些宏和函数，还定义了`lconv`数据结构。

| 常量        | 解释                                         |
| ----------- | -------------------------------------------- |
| LC_ALL      | 选择整个 C 本地环境                          |
| LC_COLLATE  | 影响 strcoll 和 strxfrm 函数                 |
| LC_CTYPE    | 影响所有字符函数                             |
| LC_MONETARY | 影响 localeconv 函数提供的货币信息           |
| LC_NUMERIC  | 影响 localeconv 函数提供的小数点格式化和信息 |
| LC_TIME     | 影响 strftime 函数                           |

```C
char *setlocale(int category, const char *locale)	//用于设置地域化信息
struct lconv *localeconv(void)						//用于读取地域化信息 
```

更多示例请看[这里](../src/basic/locale)

C++的中也可以使用C中的那一套规则，但在C的基础上，也增加了很多新的内容

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

### linux下字体颜色设置

在编写编译器工程的时候，遇到过一个很棘手的问题，如何将不同词性的词语用颜色区分出来，就像vim或者VS code做的那样好。

现在开始详细介绍一下，在linux平台下，如何让黑框中的字体有颜色。

先来看一个例子：

```C
printf("\033[32mHello World\033[0m");
```

执行语句的效果是：<font color=#00ff00>Hello World</font>

第一部分`\033[32m`就是指示字体显示为绿色，后一部分`\033[0m`指示的是，关闭所有的属性，使得该`printf`语句不会影响接下来的输出。可以发现，只要加上`\033[`就可以实现一些花里胡哨的东西了。

那么，到底可以表示出多少的颜色呢，查阅资料后整理出如下：

| 代码        | 效果                                            |
| ----------- | ----------------------------------------------- |
| \033[0m     | 关闭所有属性                                    |
| \033[1m     | 设置高亮度                                      |
| \033[2m     | 设置低亮度                                      |
| \033[3m     | *设置斜体*                                      |
| \033[4m     | <u>下划线</u>                                   |
| \033[7m     | 反显（即背景色和前景色都取反色调                |
| \033[8m     | 消失（即要打印的东西消失                        |
| \033[30m    | 设置前景色为黑色                                |
| \033[31m    | <font color=red>设置前景色为红色</font>         |
| \033[32m    | <font color=#00ff00>设置前景色为绿色</font>     |
| \033[33m    | <font color=yellow>设置前景色为黄色</font>      |
| \033[34m    | <font color=blue>设置前景色为蓝色</font>        |
| \033[35m    | <font color=purple>设置前景色为紫色</font>      |
| \033[36m    | <font color=darkgreen>设置前景色为深绿色</font> |
| \033[37m    | <font color=grey>设置前景色为白色</font>        |
| \033[40-47m | 全部表示背景颜色                                |
| \033[2J     | 清屏                                            |
| \033[x;yH   | 设置光标位置，x表示行数，y列数                  |
| \033[u      | 恢复光标位置                                    |
| \033[s      | 保存光标位置                                    |

具体示例请看[这里](../src/basic/color/color.c)

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

