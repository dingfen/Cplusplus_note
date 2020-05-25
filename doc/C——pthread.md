# POSIX多线程编程

[TOC]

POSIX线程（POSIX threads），简称Pthreads，是线程的POSIX标准。该标准定义了创建和操纵线程的一整套API。在类Unix操作系统（Unix、Linux、Mac OS X等）中，都使用Pthreads作为操作系统的线程。Windows操作系统也有其移植版pthreads-win32。

## Pthreads API

- 线程管理（Thread management）: 第一类函数直接用于线程：创建（creating），分离（detaching），连接（joining）等等。包含了用于设置和查询线程属性（可连接，调度属性等）的函数。
- 互斥量（Mutexes）: 第二类函数是用于线程同步的，称为互斥量（mutexes），是"mutual exclusion"的缩写。
- Mutex函数提供了创建，销毁，锁定和解锁互斥量的功能。同时还包括了一些用于设定或修改互斥量属性的函数。
- 条件变量（Condition variables）：第三类函数处理共享一个互斥量的线程间的通信，基于程序员指定的条件。这类函数包括指定的条件变量的创建，销毁，等待和受信（signal）。设置查询条件变量属性的函数也包含其中。
- 命名约定：线程库中的所有标识符都以pthread开头

| Routine Prefix     | Functional Group       |
| ------------------ | ---------------------- |
| pthread_           | 线程本身和各种相关函数 |
| pthread_attr_      | 线程属性对象           |
| pthread_mutex_     | 互斥量                 |
| pthread_mutexattr_ | 互斥量属性对象         |
| pthread_cond_      | 条件变量               |
| pthread_condattr_  | 条件变量属性对象       |
| pthread_key_       | 线程数据键             |

## 创建与终止线程

首先，在使用pthread时，需要包含头文件`pthread.h`，编译时，需要加上`-lpthread`链接项。现在，我们开辟一个新的线程，让新线程调用某一个函数，并将函数的参数传递过去。

- `pthread_create` 创建线程
- `pthread_exit` 终止线程

`pthread_create`的API如下：

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
                   void *(*start_routine) (void *), void *arg);
```

若创建成功，返回0；若出错，则返回错误编号，且`*thread`是未定义的。`thread` 是线程标识符，其本质是一个`unsigned long`，但这个参数不是由用户指定的，而是由 `pthread_create` 函数在创建时将新的线程的标识符放到这个变量中。`attr` 指定线程的属性，可以用 NULL 表示默认属性。`start_routine` 指定线程开始运行的函数，`arg` 是 `start_routine` 所需要的参数，是一个无类型指针。若需要传递多个参数，可以将它们封装在一个结构体中，见[例2](../src/pthreads/args2.c)。

默认地，线程在被创建时要被赋予一定的属性，这个属性存放在数据类型 `pthread_attr_t` 中，它包含了线程的调度策略，堆栈的相关信息，join or detach 的状态等。

`pthread_exit`函数比较简单：

```c
void pthread_exit(void *retval);
```

`retval`是由用户指定的参数， `pthread_exit` 完成之后可以通过这个参数获得线程的退出状态。

一般来说，一个线程通过以下几种方式终结：

- 线程运行的函数`start_rountine()`返回了，也就是线程的任务已经完成；
- 线程调用了 `pthread_exit`函数。
- 其他线程调用 `pthread_cancel` 结束这个线程；
- 进程调用 `exec()` or `exit()`，结束这个线程；
- `main()` 函数先结束了，而且 `main()` 自己没有调用 `pthread_exit` 来等所有线程完成任务.

`pthread_attr_init`和 `pthread_attr_destroy` 函数分别用来创建和销毁 `pthread_attr_t`，具体见下文。

示例程序见[这里](../src/pthreads/args1.c)。`pthread_create`创建线程，接收多参数，每一个线程都会打印一条表明自己线程id的消息，然后调用`pthread_exit`终止线程。

## 获取调用线程标识号

`pthread_self`函数返回了调用该函数的线程的ID。返回类型为`pthread_t`

```c
pthread_t pthread_self(void);
```

有时，为判断两个线程的编号是否相等，需要用到`pthread_equal`函数：

```c
int pthread_equal(pthread_t _thread1, pthread_t _thread2);
```

但大多是情况下，都是在判断当前运行的线程是否就是给定的线程编号的线程，因此，通常都是如此用：

```c
pthread_equal(thread_id, pthread_self());
```

[示例](../src/pthreads/equal.c)中介绍了`pthread_self`和`pthread_equal`的用法。

## 一次初始化

`pthread_once`函数保证了线程一次初始化，在多线程环境中，有些事仅需要执行一次。通常当初始化应用程序时，可以比较容易地将其放在main函数中。但当你写一个库时，就不能在main里面初始化了，你可以用静态初始化，但使用一次初始化（pthread_once）会比较容易些。

```c
// 初值为PTHREAD_ONCE_INIT的变量 保证init_routine()函数在本进程执行序列中仅执行一次
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
```

*在多线程编程环境下，尽管pthread_once()调用会出现在多个线程中，init_routine()函数仅执行一次，究竟在哪个线程中执行是不定的，是由内核调度来决定。*[示例](../src/pthreads/initonce.c)展示了其用法。

Linux线程使用**互斥锁**和**条件变量**保证由`pthread_once()`指定的函数执行且仅执行一次。

- once_control表示是否执行过。如果once_control的初值不是`PTHREAD_ONCE_INIT`（其值为0），`pthread_once()`的行为就会不正常。
- 实际"一次性函数"的执行状态有三种：NEVER(0)、IN_PROGRESS(1)、DONE(2)，如果once初值设为1，则由于所有`pthread_once()`都必须等待其中一个激发"已执行一次"信号，因此所有`pthread_once ()`都会陷入永久的等待中；如果设为2，则表示该函数已执行过一次，从而所有`pthread_once()`都会立即返回0。

## 线程取消

`pthread_cancel`线程取消API:

```c
int pthread_cancel(pthread_t thread);
```

线程调用`pthread_cancel`并不等待线程终止，它只是向线程`thread`提出请求。在请求(pthread_cancel)发出后线程会继续运行。一个线程可以通过调用 `pthread_cancel` 函数来请求取消同一进程中的线程，这个线程由thread参数指定。 如果操作成功则返回0，失败则返回对应的错误编号。

何时以及是否对目标线程做出取消操作，取决于这两个线程的attributes。函数`pthread_setcancelstate`设置了线程取消条件状态。

[示例](../src/pthreads/cancel2.c)很好地说明了线程取消的功能。

## 线程信号发送

```c
int pthread_kill(pthread_t thread, int sig)
```

`pthread_kill`发送sig信号，*`pthread_kill`并不是结束进程的意思，而是向指定ID的线程发送一个信号*，只是大部分信号的默认动作是终止线程的运行。如果线程不处理该信号，则按照信号默认的行为作用于整个进程。信号值0为保留信号，作用是根据函数的返回值判断线程是不是还活着，事实上此时没有信号被发送出去。

`pthread_kill`的返回值：成功（0）线程不存在（ESRCH） 信号不合法（EINVAL）

## 线程属性 - 线程连接和分离

连接（join）是线程之间同步的一种方法。

```c
// 主线程挂起直至目标进程返回
int pthread_join(pthread_t thread, void ** thread_return);
// 初始化属性对象
int pthread_attr_init(pthread_attr_t *attr);
// 销毁属性对象
int pthread_attr_destroy(pthread_attr_t *attr);
// 设置属性对象的分离状态
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
// 获得属性对象
int pthread_attr_getdetachstate(pthread_attr_t *attr, int detachstate);
```

[示例程序](../src/pthreads/attr.c)在这里。当一个线程被创建，它有一个属性定义了它是可连接的(joinable)还是分离的(detached)。只有是可连接的线程才能被连接(joined)，如果创建的线程是分离的，则不能连接。

- `pthread_join`函数阻塞主线程直到指定的线程终止。返回值：0代表成功。若失败，返回的则是错误号。该函数会让调用它的线程等待 `thread`线程运行结束之后再运行。`thread_return`存放了其他线程的返回值. 一个可以被join的线程，仅可以被其他的一个线程 join，如果同时有多个线程尝试 join 同一个线程时，最终结果是未知的。 另外，线程不能 join 自己。

- `pthread_attr_init`函数用于初始化`pthread_attr_t`变量，将里面的值全部用默认值代替。之后该变量就可以被设置到线程上去了。在已经初始化的变量中再调用`pthread_attr_init`会导致未定义行为。返回值：0代表成功。若失败，返回的则是错误号。
- `pthread_attr_destroy`是在`pthread_attr_t`不再需要时调用。返回值：0代表成功。若失败，返回的则是错误号。
- `pthread_attr_setdetachstate`设置了线程的属性状态。`attr`指向线程属性对象的指针。`detachstate`设置可分离(`PTHREAD_CREATE_DETACHED`）或可连接属性(`PTHREAD_CREATE_JOINABLE`)。函数返回0表示成功。
- `pthread_attr_getdetachstate`获得线程属性状态的分离、连接情况。

使用`pthread_create`的`attr`参数可以显式的创建可连接或分离的线程，典型四步如下：

1. 声明一个`pthread_attr_t`数据类型的线程属性变量
2. 用`pthread_attr_init`初始化该属性变量
3. 用`pthread_attr_setdetachstate`设置可分离状态属性
4. 完了后，用`pthread_attr_destroy`释放属性所占用的库资源

`pthread_detach`将线程`thread`设成分离态。

```c
int pthread_detach(pthread_t thread);
```

一些困惑：为什么要设置分离状态？既然分离状态线程不可以连接，有何存在的必要？一般情况下，线程终止后，其终止状态一直保留到其它线程调用`pthread_join`获取它的状态为止。但**线程也可以被置为detach状态， 这样的线程一旦终止就立刻回收它占用的所有资源，而不保留终止状态，相当于节省资源。**若创建一个线程不关心它的返回值,也不想使用pthread_join来回收资源，最好用detach状态。[示例程序](../src/pthreads/detach.c)详细说明了如果一个进程设置为detach，那么主进程直接退出，会直接将子进程杀掉。

## 线程属性 - 栈管理

`pthread_attr_getstacksize` 获取当前线程的栈空间

 `pthread_attr_setstacksize` 设置当前线程的栈空间

```c
// 参数：attr - 指向线程属性的指针
// stacksize - 栈空间大小
// 返回值：0代表成功。若失败，返回的则是错误号。

int pthread_attr_getstacksize(const pthread_attr_t *restrict attr, size_t *restrict stacksize);

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
```

**POSIX标准并没有指定线程栈的大小，依赖于实现并随实现变化。**因而很容易超出默认的栈大小，常见结果：程序终止或者数据损坏。安全和可移植的程序应该不依赖于默认的栈限制，但是取而代之的是用`pthread_attr_setstacksize`分配足够的栈大小。

`pthread_attr_getstackaddr`和`pthread_attr_setstackaddr`函数可以被程序用于将栈设置在指定的内存区域。[示例](../src/pthreads/stack.c)演示了这两个函数的用法。

## 互斥锁

`pthread_mutex_init` 互斥锁的初始化

`pthread_mutex_destroy` 互斥锁的销毁

`pthread_mutex_lock` 加锁

`pthread_mutex_unlock` 解锁

```c
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
                       const pthread_mutexattr_t *restrict attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

另外，`pthread_mutex_trylock`是`pthread_mutex_lock`的非阻塞版本，调用后立即返回。

```c
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```

互斥锁(Mutex)是实现线程同步，和保护同时写、边读边写的共享数据的主要方法，可以防止竞争条件。

互斥锁对共享数据的保护就像一把锁。在POSIX线程中，任何时候**仅有一个线程可以锁定互斥锁**，*因此，当多个线程尝试去锁定该互斥锁时仅有一个会成功。* 直到锁定互斥锁的线程解锁互斥锁后，其他线程才可以去锁定互斥锁。线程必须轮着访问受保护数据。

用互斥锁的典型顺序如下：

1. 创建和初始一个互斥锁，有两种方式初始化一个互斥锁：第一种，利用已经定义的常量`PTHREAD_MUTEX_INITIALIZER`初始化，第二种就是使用`pthread_mutex_init`初始化。
2. 多个线程尝试去锁定该互斥锁，失败的线程会被阻塞，直到这个锁被解锁。`pthread_mutex_trylock`函数会返回一个错误。
3. 仅有一个线程可以成功锁定改互斥锁。
4. 锁定成功的线程做一些处理。
5. 线程解锁该互斥锁。
6. 另外一个线程获得互斥锁，重复上述过程。
7. 最后销毁互斥锁。

[示例1](../src/pthreads/lock1.c)展示了两个线程如何读取、写入同一个变量。

## 读写锁

`pthread_rwlock_init` 创建锁

`pthread_rwlock_destroy` 销毁锁

`pthread_rwlock_rdlock` 获取一个读出锁

`pthread_rwlock_wrlock` 获取一个写入锁

`pthread_rwlock_unlock` 释放一个写入锁或者读出锁

`pthread_rwlock_tryrdlock` 非阻塞获得读锁

`pthread_rwlock_trywrlock` 非阻塞获得写锁

```c
int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwptr);
int pthread_rwlock_rdlock(pthread_rwlock_t *rwptr); //获取一个读出锁
int pthread_rwlock_wrlock(pthread_rwlock_t *rwptr); //获取一个写入锁
int pthread_rwlock_unlock(pthread_rwlock_t *rwptr); //释放一个写入锁或者读出锁
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
```

读写锁与互斥锁类似，不过读写锁允许更高的并行性。互斥锁要么是锁住状态，要么是不加锁状态，而且一次只有一个线程对其加锁。而**读写锁可以有三种状态：读模式下加锁状态，写模式下加锁状态，不加锁状态。一次只有一个线程可以占有写模式的读写锁，但是多个线程可用同时占有读模式的读写锁。**

读写锁也叫做共享-独占锁，当读写锁以读模式锁住时，它是以共享模式锁住的，当它以写模式锁住时，它是以独占模式锁住的。

- 当读写锁是写加锁状态时，在这个锁被解锁之前，所有试图对这个锁加锁的线程都会被阻塞；

- 当读写锁在读加锁状态时，所有试图以读模式对它进行加锁的线程都可以得到访问权，但是以写模式对它进行枷锁的线程将阻塞；

- 当读写锁在读模式锁状态时，**如果有另外线程试图以写模式加锁，读写锁通常会阻塞随后的读模式锁请求，这样可以避免读模式锁长期占用**，而等待的写模式锁请求长期阻塞；

[示例程序](../src/pthreads/lock3.c)简单展示了线程对读写锁的使用。

## 条件变量

多线程中经常需要使用到锁(pthread_mutex_t)来完成多个线程之间的互斥操作。**但互斥锁有一个明显到缺点: 只有两种状态，锁定和非锁定。而条件变量则允许线程阻塞，等待另一个线程发送唤醒信号，它常和互斥锁一起使用。**

`pthread_cond_init`创建条件变量。

`pthread_cond_destroy`删除条件变量。

`pthread_cond_wait`等待`cond`为真，会阻塞调用它的线程，直到收到某个信号。这个函数需要在 `mutex`已经被锁之后进行调用，并且当线程被阻塞时，会自动解锁`mutex`，并将线程阻塞在`cond`上。线程被`pthread_cond_signal`唤醒后，这时`mutex`又会被这个线程锁定。为了避免死锁，`pthread_cond_wait`总和一个互斥锁结合使用。在调用`pthread_cond_wait`前要先获取锁。

`pthread_cond_signal`解锁阻塞在`cond`上的进程，函数结束时，必须解锁`mutex`，以供 `pthread_cond_wait()`锁定`mutex`。当不止一个线程在等待信号时，要用 `pthread_cond_broadcast()` 代替 `pthread_cond_signal()`来告诉所有被该条件变量阻塞的线程结束阻塞状态。

```c
// 初始化
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *condattr);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
```

条件变量是用来通知共享数据状态信息的。条件变量机制允许进程停止当前的执行，让出CPU时间，直到条件为真。*每个条件变量必须与一个特定的互斥量相关联以避免死锁。*

`pthread_cond_wait`函数执行时先自动释放指定的锁，然后等待条件变量的变化。在函数调用返回之前，自动将指定的互斥量重新锁住。调用`pthread_cond_signal`后要立刻释放互斥锁。

[示例](../src/pthreads/cond1.c)演示了这一点。

## 障碍同步

`pthread_barrier_init`障碍初始化。

`pthread_barrier_wait`障碍等待，当一个线程到达障碍时，就会等待，所有线程都到达，就会放行。

`pthread_barrier_destroy`障碍销毁。

障碍同步(Barrier Synchronization)是POSIX线程中另外一种同步机制。**barrier意为栏杆，形象的说就是把先后到达的多个线程挡在同一栏杆前，直到所有线程到齐，然后撤下栏杆同时放行。**[示例程序](../src/pthreads/barrier.c)程序进行了详细说明

```c
// 需要等待count个线程后，才可放行
int pthread_barrier_init(pthread_barrier_t *barrier, 
                         const pthread_barrierattr_t *attr, unsigned count);
int pthread_barrier_wait(pthread_barrier_t *barrier);
int pthread_barrier_destroy(pthread_barrier_t *barrier);
```

## 线程私有数据

`pthread_key_create` 创建一个键

`pthread_setspecific` 为一个键设置线程私有数据

`pthread_getspecific` 从一个键读取线程私有数据

`pthread_key_delete` 删除一个键

```c
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
int pthread_setspecific(pthread_key_t key,const void *pointer));
void *pthread_getspecific(pthread_key_t key);
int pthread_key_delete(pthread_key_t key);
```

在多线程的环境下，进程内的所有线程共享进程的数据空间。因此全局变量为所有线程共享。在程序设计中有时需要保存线程自己的全局变量，这种特殊的变量仅在线程内部有效。

线程私有数据采用了一键多值的技术，即一个键对应多个值。访问数据时都是通过键值来访问，好像是对一个变量进行访问，其实是在访问不同的数据。主要过程如下：

1. 创建一个键
2. 为一个键设置线程私有数据`int pthread_setspecific(pthread_key_t key,const void *pointer));`
3. 从一个键读取线程私有数据 `void *pthread_getspecific(pthread_key_t key);`
4. 线程退出（退出时，会调用destructor释放分配的缓存，参数是key所关联的数据）
5. 删除一个键

[示例程序](../src/pthreads/key.c)简单介绍了用法。

