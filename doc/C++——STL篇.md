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

