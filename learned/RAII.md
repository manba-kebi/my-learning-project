# 今天学到的东西

## 1.学习路线

今天了解到了很多东西，比如RAII、move语义、TCP/IP、进程与线程、CMake等等，还有一些东西，我才发现这些都与C++有关，并且都是面向公司真实业务的。以前没人问，没有关系，不了解这些东西，并且问豆包、deepseek，要么得到的回答是不真实的，要不然就是规划不合理的，然后想去b站上找些课程学习，发现也大多都是要收费的课程，想去外国的网站上学写真东西但又不会翻墙。直到今天用了航哥的chatgpt我才发现原来规划的居然可以这么清晰。所以我感到我不再迷茫，我也应朝着这条路不断的深耕下去。

## 2.RAII

**资源获取及初始化** （RAII）是一种C++编程技术（但似乎这就是一种编程思想），它将**必须在使用前获取的资源生命周期（分配的堆内存、执行线程、开放套接字、打开文件、锁定的互斥体、磁盘空间、数据库链接--任何有限供应的资源**）绑定到对象的生命周期。

RAII 保证资源对任何可能访问该对象的函数开放（资源可用性为[类不变 ](https://en.wikipedia.org/wiki/Class_invariant)，消除冗余的运行时测试）。他还保证所有资源在其控制对象生命周期结束时释放，**按获取顺序逆序**。（我感觉这有些像虚析构函数的使用，当释放资源的时候，按析构函数的实现，依次从子类->父类的顺序调用析构函数）。同样，如果资源获取失败（构造函数以例外退出），每个完全构建的成员和基础子对象获取的所有资源将按初始化的逆序释放。这利用了核心语言特性（[ 对象寿命 ](https://en.cppreference.com/cpp/language/lifetime)、[ 作用域退出 ](https://en.cppreference.com/cpp/language/statements)、[ 初始化顺序](https://en.cppreference.com/cpp/language/initializer_list#Initialization_order)和[栈展开 ](https://en.cppreference.com/cpp/language/throw#Stack_unwinding)）来消除资源泄漏，保证异常安全。该技术的另一个名称是*范围限制资源管理* （SBRM），源自基本用例：RAII 对象因作用域退出而生命周期终止。



**始终通过RAII类实例使用该资源，满足以下条件：**

1. **具有自动存储时长或临时寿命，或**

2. **其寿命受自动或临时对象的寿命限制**

   

## 3.为什么要用RAII

- 不用记住关闭

- 不会漏写

- 不会因为 return 泄漏

- 不会因为异常泄漏

- 多线程不会死锁

- 大型项目永远安全

如果不用RAII，那么在中途 return → 你根本执行不到 fclose、抛异常 → fclose 永远不执行、写 100 个函数打开文件 → 你一定会忘关



## 4.RAII 类 与 非RAII类的区别

### **非RAII类**

具有 open()/close()、lock()/unlock() 或 init()/copyFrom()/destroy() 成员函数的类是非RAII类的典型例子。

```c++
std::mutex m;

void bad() 
{
    m.lock();             // acquire the mutex
    f();                  // if f() throws an exception, the mutex is never released
    if (!everything_ok())
        return;           // early return, the mutex is never released
    m.unlock();           // if bad() reaches this statement, the mutex is released
}
```

这里有两个致命问题:

1. **`f()` 抛出异常**：程序直接跳走，`unlock()` 永远不会执行 → 锁永远没释放，别的线程永远拿不到锁 → 死锁。
2. **提前 `return`**：`everything_ok()` 为 false 时，直接 return，同样不会执行 `unlock()` → 锁泄漏。

核心特点：**资源的获取和释放，需要你手动成对调用**，编译器和运行时不会帮你自动处理。只要你漏了其中一步，就会造成**资源泄漏**（文件句柄泄漏、死锁、内存泄漏）。



### **RAII类**

核心特点：**把这对操作交给对象的构造 / 析构函数自动完成，让你再也不用手动配对。**

```c++
void good()
{
    std::lock_guard<std::mutex> lk(m); // RAII class: mutex acquisition is initialization
    f();                               // if f() throws an exception, the mutex is released
    if (!everything_ok())
        return;                        // early return, the mutex is released
}                                      // if good() returns normally, the mutex is released
```

`std::lock_guard` 就是一个典型的 RAII 类，它的工作流程是：

1. **构造时（初始化）**：自动调用 `m.lock()`，拿到锁
2. **析构时（离开作用域）**：自动调用 `m.unlock()`，释放锁



## 5.[Stroustrup C++ FAQ 中的 RAII](https://www.stroustrup.com/bs_faq2.html#finally)

因为 C++支持一种几乎总是更好的替代方案：“资源获取即初始化”技术（TC++PL3 第 14.4 节）。基本思路是用本地对象表示资源，使得本地对象的解构器释放资源。这样程序员就不会忘记释放资源。例如：

![image-20260418165434069](C:\Users\28251\AppData\Roaming\Typora\typora-user-images\image-20260418165434069.png)

在系统中，我们需要为每个资源创建一个“资源句柄”类。然而，我们不必为每次资源的获取设定“最终”条款。在现实系统中，资源获取远多于资源种类，因此“资源获取即初始化”技术导致的代码比“最终”构造的使用更少。

另外，看看资源管理的例子 [附录 E](https://www.stroustrup.com/3rd_safe0.html) C[++编程语言 ](https://www.stroustrup.com/3rd.html)。

（我的想法啊：RAII在这里的实现是将自己的资源创建时机绑定到了这个类的对象的创建时机上，也就是构造函数调用的时候。并且将自己资源的释放也绑定到了这个类的析构函数上，这样在函数退出时，或者其他对象销毁的时候，触发析构函数，RAII的资源也被释放掉了。可能这段表达不太准确）

## 6.注意

RAII 不适用于未在使用前获得的资源管理：CPU 时间、核心可用性、缓存容量、熵池容量、网络带宽、电力消耗、堆栈内存。对于这些资源，C++类构造器无法保证对象生命周期内资源的可用性，必须采用其他资源管理方式。



# RAII与智能指针的关系



**RAII** 是一种**C++编程思想，设计原则**，而**智能指针**是RAII思想最经典的、最常用的一个**应用**。

RAII用一句话来说就是：

​		**用对象的生命周期，来管理资源的生命周期**，它是一种技巧。

而智能指针（unique_ptr/shared_ptr/weak_ptr）的本质就是：

​		**专门用来管理堆内存的RAII类**





# 额外学到的一些相关内容

## **1. explicit**

`explicit` 是 C++ 里一个用来**限制隐式类型转换**的关键字，主要用在**单参数的构造函数**上，是个非常实用的安全工具。**目的**主要是为了**防止意外的隐式转换带来的bug**，让代码更安全、更清晰

当不加`explicit` 的时候，默认情况下，单参数的构造函数会支持**隐式转换**

给构造函数加上`explicit`的时候，就**禁止了这种隐式转换**

### 什么时候必须用 explicit？

1. **所有单参数的构造函数**：只要不是故意要做类型转换，都建议加上 `explicit`，这是现代 C++ 的最佳实践。
2. **RAII 类 / 智能指针 / 锁类**：比如你之前看到的 `std::lock_guard`、`std::unique_ptr`，它们的构造函数都是 `explicit` 的，防止你不小心把一个裸指针直接传进去，导致资源管理混乱。（最认可的一点）
3. **有多个参数的构造函数**：C++11 以后，`explicit` 也可以用在多参数构造函数上，防止列表初始化的隐式转换



## 2. 成员初始化列表

在 4.[Stroustrup C++ FAQ 中的 RAII] 中出现的   ：**冒号初始化列表**   `: std::runtime_error("File open failed"), errno_(errno_)`，这个 **`:` 后面的东西叫「成员初始化列表」**，**是 C++ 构造函数里专门用来初始化成员 / 父类的语法**，前一句std::runtime_error("File open failed") 是在初始化父类，后一句 errno_(errno_) 是在初始化自己的成员变量。最后是空的函数体{}，因为所有初始化都在 **`:` 后面做完了**，所以大括号里不用写代码。

最重要的结论：**冒号 + 初始化列表 = C++ 构造函数专用初始化语法**

它的作用只有两个：

1. **初始化父类**
2. **初始化自己的成员变量**

