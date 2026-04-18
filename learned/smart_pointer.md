# 智能指针

## 概况

### 是什么？

**智能指针**是带自动回收功能的指针，比如可以帮助把new出来的内存，在不用的时候自动delete，不用手动写delete了。它本质上是个**模板类**，用起来和普通裸指针*p一样，支持 \* 解引用、->访问成员，但多了自动释放的安全特性。（这里安排的计划是学完RAII，再学智能指针的，没想到这两块主要都是为了安全的）



### 三个核心智能指针

|    智能指针     |                核心特点                |             适用场景             |
| :-------------: | :------------------------------------: | :------------------------------: |
| std::unique_ptr |  **独占所有权**，不允许拷贝，只能移动  | 绝大多数场景，单个对象的专属管理 |
| std::shared_ptr |   **共享所有权**，引用计数，多人共用   | 需要多个地方同时使用同一个对象时 |
|  std::weak_ptr  | **弱引用**，不增加计数，配合shared_ptr |   解决shared_ptr循环引用的问题   |



### 用法示例

std::unique

```c++
#include <memory>
#include <iostream>

int main(){
	//推荐使用make_unique初始化，要比new更安全
	std::unique_ptr<int> p = std::make_unique<int>(10);
	//像普通指针一样用
	std::cout<<*p<<std::endl;	//输出10
	
	//离开作用域时，p自动销毁，内存自动释放，不用delete
}
```



std::shared_ptr

```c++
#include <memory>
#include <iostream>

int main(){
	std::shared_ptr<int> p1 = std::make_shared<int>(20);
	std::shared_ptr<int> p2 = p1;	//拷贝，引用计数+1
	
	std::cout<<*p1<<" "<<*p2<<std::endl;
	std::cout<<p1.use_count()<<std::endl;
	
	//离开作用域，p2先销毁，计数-1；p1再销毁，计数为0，内存释放
}
```



### 新手需记的三个要点

1. **优先用 `make_unique` / `make_shared`**，别直接用 `new` 初始化智能指针，更安全。
2. ` unique_ptr `**不能拷贝**,但可以用`std::move`转移所有权。
3. 别用裸指针和智能指针混用，不然会导致重复释放、内存泄漏等问题。



## unique_ptr

