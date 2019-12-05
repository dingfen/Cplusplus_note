[TOC]

# C++——基础篇

## 技能点

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

### 二、不可拷贝类

在开发过程中，有时候我们希望构造的对象是不可以被拷贝复制的。

比如在`boost::progress_timer`中，计时类`progress_timer`就是不可以被拷贝的，因为它在构造函数中开始计时，在析构函数中结束计时并输出，拷贝该类不仅没有意义，还会造成计时错误。

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

## 疑问解答

### 1. extern关键字什么用处？

extern是C/C++语言中表明函数和全局**变量作用范围**的关键字，该关键字告诉编译器，其声明的函数和变量可以在本模块或其它模块中使用。

 通常，在模块的头文件中对**本模块提供给其它模块引用的函数和全局变量以关键字extern声明**。例如，如果模块B欲引用该模块A中定义的全局变量和函数时只需包含模块A的头文件即可。这样，模块B中调用模块A中的函数时，在编译阶段，模块B虽然找不到该函数，但是并不会报错；它会在**链接阶段**中从模块A编译生成的目标代码中找到此函数。

## 2. constexpr关键字什么用处

`constexpr`表示，被声明的函数或变量的值可以在编译期间被计算出来。那么这些变量或者函数的值就可以被当做编译期间的常量去使用