[TOC]



# C++——STL篇

## 简介

什么是STL？

简单来说，是C++语言中，为了提高代码的复用性而加入的标准模板库(Standard Template Library)。STL不仅提供了常用的数据结构（如栈、数组等）还提供了一些常用算法。因此熟练使用STL库可以提高编程效率，减少出错可能，美观代码，更好地使用C++完成一些高难度任务，而不是忙于造轮子。

STL有六大组件

- 容器(Containers) 各种各样的数据结构，有顺序的（如vector、list、array），也有组合式的（如set、map）也有非顺序的（如unordered_set）有不同的功能，适用于不同的场合，但本质就是class template
- 算法(Algorithms) 提供各种常用算法，比如sort、search、copy等
- 迭代器(Iterators) 可以把迭代器理解成一个万能的指针，它们都实现了指针相关的运算，如`operator*`、`operator->`，几乎所有的STL容器都有自己的迭代器，用于遍历自己的元素。
- 仿函数(functors) 这是一种重载了`operator()`的class或者class template，可以理解为一种高级的函数指针。
- 配接器(Adapters) 有时候，配接器被归在了容器内部，但其实配接器有些不同于容器，如queue、stack，它们的底层事实上都完全借助deque实现。
- 配置器(allocators) 用于分配内存空间，负责管理空间配置的class template

我对仿函数的理解不够深刻，一开始甚至不懂仿函数是在干什么，这里有个[例子](../src/STL/introduction)：

```C++
template<class T>
class print {
public:
    void operator() (const T & elem) {
        cout << elem << endl;
    };
};

int main(int argc, char const *argv[])
{
    vector<int> a{1,2,3,4,5};
    for_each(a.begin(), a.end(), print<int>());
}
```

这里的`class print`就是仿函数，重载了`operator()`的class template。这里最后一行，就是用`print<int>`的一个临时对象，传入到`for_each()`中，进行打印操作，用法非常类似于函数指针。

那么，仿函数与函数指针的区别在哪里呢？首先，仿函数是一个对象，对象就可以有自己的内部状态，可以根据自身的状态不同来作出不同的反应，这是函数指针做不到的。比如之前的例子中，若`print`类中再添加一个成员变量，根据成员变量的数值来决定输出格式，这是函数指针做不到的。当然你可以说函数指针使用静态变量来完成，但静态变量又有可能被其他程序修改导致不确定性，封装也没有对象好。其次，编译器可以内联(inline)仿函数，而函数指针不能这样，因此效率会高一些。

## 配置器

### 设计一个简单的空间配置器

根据STL规范，`allocator`必须有以下接口

- allocate() 函数，用于分配内存空间
- deallocate() 函数，用于收回内存空间
- construct() 函数，用于构建对象
- destroy() 函数，用于析构对象
- address() 函数用于返回对象的地址
- rebind() 函数

外加上一些构造函数和析构函数，以及STL规范中要求的traits特性。从[示例](../src/STL/alloc)中可以看出各个函数的功能实现。

一般而言，常用的C++内存配置操作时这样的。

```C++
class Abc { ... };
Abc* pf = new Abc;	// 配置内存 构造对象
delete pf;			// 析构对象 释放内存
```

这其中，`new`事实上做了两步操作：1、调用`::operator new`配置内存。2、调用`Abc()`构造对象。`delete`也做了两步操作：1、调用`~Abc()`函数析构对象。2、调用`::operator delete`释放内存。

现在STL将这两步分开，内存配置由`allocator::allocate()`完成，构造对象由`::construct`完成，内存释放由`allocator::deallocate()`完成，析构对象由`::destroy()`完成。

### 关于空闲块链表

STL作为一个C++标准库，效率是非常关键的指标。理解STL源码，必须要理解为什么STL有这么高的运行效率，理解为提升效率编写者的良苦用心。

回归正题，小额区块带来的内存碎片是非常头疼的，系统不仅要配置并维护内存碎片，而且还需要额外的空间去管理记录，这都是很沉重的管理负担。内存碎片的存在还会使得内存利用不充分，浪费宝贵的内存空间。

为解决（或者说缓解）这一问题，SGI STL实现了二级配置器，二级配置器从内存池中索要分配空间，并将空闲的块用**自由链表**维护。这相当于给所有的内存碎片用链表串起来，在遇到一些符合要求的内存需求时，直接从链表中拨出，这样可以充分利用内存碎片，也就提高了效率。

维护链表时，为了进一步减少额外负担，采用如下数据结构：

```C++
union obj {
    union obj * free_list_link;
    char client_data[1];
};
```

如此一来，每个`obj`事实上只占用了8个字节（64位下），若一个块64字节，那么只有头8字节被赋值，指向了下一个块的地址（见下图），当分配块时，直接就将头部的64字节返回，并将free_list指针指向第二个块。

![](./freelist.png)

从第一个字段看，只需要使用`free_list_link`指针就可以指向下一个`obj`，而从第二个字段看，可以被视作一个指针，指向实际区块。但是这么一个简陋的节点，究竟是怎样链接在一起的？

在分配(allocate)时，只需要从链表头部拿走一个块，然后把新头部定在`free_list_link`就行。

```C++
my_free_list = free_list + FREELIST_INDEX(n);
result = *my_free_list;
if (result == 0) {
   void *p = refill(n);
   return p;
}
*my_free_list = result->free_list_link;
```

在回收(deallocate)空间时，将回收到的块插入到链表头部就行。

```C++
// p 指针指向被回收的块   n是指针指向块的大小
obj * q = (obj *)p;

my_free_list = free_list + FREELIST_INDEX(n);
q->free_list_link = *my_free_list;
*my_free_list = q;
```

若链表中没有了空闲块，那么就需要从堆中或从内存池中分配一些空间，使用`refill()`函数就可以做到。

```C++
void* refill(size_t n)
{
    obj * volatile * my_free_list = free_list+ FREELIST_INDEX(n);
    obj * next_obj, * current_obj;
    char * chunk = (char *)malloc(n*4);		// 直接从堆中 malloc
    obj * result = (obj *)chunk;

    // 首先让 next_obj 和 my_free_list 指向第二个chunk
    *my_free_list = next_obj = (obj *)(chunk + n);
    for(int i = 1; ; i++) {
        current_obj = next_obj;
        next_obj = (obj *)((char *)next_obj + n);
        if (i == 3) {
            // 最后为 0
            current_obj->free_list_link = 0;
            break;
        } else {
            // 首尾相连接
            current_obj->free_list_link = next_obj;
        }
    }
    return result;
}
```

上述代码做了简化，`chunk`的内存空间是直接通过`malloc()`从堆中获取的，但是事实上SGI STL还在二级配置器中维护了内存池，先从内存池中取出内存空间，若内存池空间不足，在从堆中获取。