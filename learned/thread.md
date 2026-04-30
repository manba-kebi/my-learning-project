# std::thread

Defined in header `<thread>`

```c++
class thread;
```

 class `thread`表示[单个执行线程](https://en.wikipedia.org/wiki/Thread_(computing))。线程允许多个函数并发执行。

线程在关联的线程对象构造完成后立即开始执行（可能受操作系统调度延迟影响），从作为构造函数参数提供的顶层函数开始。顶层函数的返回值被忽略，若其通过抛出异常终止，则会调用`std::terminate`。顶层函数可通过`std::promise`或修改共享变量（可能需要同步，参见`std::mutex`和`std::atomic`）向调用者传递其返回值或异常。

`std::thread`对象也可能处于不表示任何线程的状态（在默认构造、移出、分离或连接之后），并且执行线程可能与任何`thread`对象都不相关联（在分离之后）。

没有两个`std::thread`对象可以表示同一个执行线程；`std::thread`不是[*CopyConstructible*](https://en.cppreference.com/cpp/named_req/CopyConstructible)或[*CopyAssignable*](https://en.cppreference.com/cpp/named_req/CopyAssignable)的，尽管它是 [*MoveConstructible*](https://en.cppreference.com/cpp/named_req/MoveConstructible)和[*MoveAssignable*](https://en.cppreference.com/cpp/named_req/MoveAssignable)的。（意思应该是线程不可以通过拷贝得到，因为这样的话两个线程就会表示同一个执行线程了。但另一个线程可以由一个线程move过来）



## Member types 成员类型

|             Member type              | Definition |
| :----------------------------------: | :--------: |
| **`native_handle_type` (optional*)** |  实现定义  |



## Member classes 成员类

| [id](https://en.cppreference.com/cpp/thread/thread/id) | 表示线程的id(公共成员函数) |
| :----------------------------------------------------: | :------------------------: |



## Member functions 成员函数

| [(constructor)](https://en.cppreference.com/cpp/thread/thread/thread) |              构造新线程对象(公共成员函数)               |
| ------------------------------------------------------------ | :-----------------------------------------------------: |
| **[(destructor)](https://en.cppreference.com/cpp/thread/thread/~thread)** | **破坏线程对象，必须连接或分离底层线程 (公共成员函数)** |
| **[operator=](https://en.cppreference.com/cpp/thread/thread/operator%3D)** |             **移动线程对象 (公共成员函数)**             |



## Observers 观察者

| [joinable](https://en.cppreference.com/cpp/thread/thread/joinable) | 检查线程是否可连接，即是否可能在并行上下文中运行 **(公共成员函数)** |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[get_id](https://en.cppreference.com/cpp/thread/thread/get_id)** |               **返回线程的id(公共成员函数) **                |
| **[native_handle](https://en.cppreference.com/cpp/thread/thread/native_handle)** |        **返回底层实现定义的线程句柄 (公共成员函数)**         |
| **[hardware_concurrency](https://en.cppreference.com/cpp/thread/thread/hardware_concurrency)**[static] |       **返回实现支持的并发线程数 (公共静态成员函数)**        |



## Operations 操作

|  [join](https://en.cppreference.com/cpp/thread/thread/join)  |       等待线程完成其执行(公共成员函数)       |
| :----------------------------------------------------------: | :------------------------------------------: |
| **[detach](https://en.cppreference.com/cpp/thread/thread/detach)** | **允许线程独立于线程句柄执行(公共成员函数)** |
| **[swap](https://en.cppreference.com/cpp/thread/thread/swap)** |      **交换两个线程对象(公共成员函数)**      |



## Non-member functions 非成员函数

| [std::swap(std::thread)](https://en.cppreference.com/cpp/thread/thread/swap2)(C++11) | 专门研究std::swap算法 (function) |
| :----------------------------------------------------------: | :------------------------------: |





# std::jthread

在标题\<thread>中定义

```c++
class jthread;
```

类`jthread`表示 [单个执行线程](https://en.wikipedia.org/wiki/Thread_(computing))。它具有与`std::thread`相同的一般行为，**除了`jthread`在销毁时会自动重新加入，并且在某些情况下可以取消/停止。**

**线程在构建相关线程对象后立即开始执行**（等待任何操作系统调度延迟），从作为构造函数参数提供的顶级函数开始。顶级函数的返回值被忽略，如果它通过抛出异常终止，则调用`std::terminate`。顶级函数可以通过`std::promise`或修改共享变量（这可能需要同步，请参阅`std::mutex`和`std::atomic`）将其返回值或异常传递给调用者。

与`std::thread`不同，`jthread`在逻辑上持有`std::stop_source`类型的内部私有成员，该成员维护共享停止状态。`jthread`构造函数接受一个函数，该函数将`std::stop_token`作为其第一个参数，`jthread`将从其内部`std::stop_source`传入该参数。这允许函数检查在执行过程中是否请求了停止，如果请求了，则返回。

`std::jthread`对象也可能处于不表示任何线程的状态（在默认构造、移出、分离或连接之后），并且执行线程可能与任何jthread物体都不关联（在分离之后）。

没有两个`std::jthread`对象可以表示同一个执行线程；`std::jthread`不是 [*CopyConstructible*](https://en.cppreference.com/cpp/named_req/CopyConstructible)或[*CopyAssignable*](https://en.cppreference.com/cpp/named_req/CopyAssignable)的，尽管它是 [*MoveConstructible*](https://en.cppreference.com/cpp/named_req/MoveConstructible)和MoveAssignable的。（这一点跟thread一模一样）



## Member types 成员类型

|             Member type              |                          Definition                          |
| :----------------------------------: | :----------------------------------------------------------: |
| **`native_handle_type`** (optional*) | [`std::thread::native_handle_type`](https://en.cppreference.com/cpp/thread/thread) |
|                **id**                | [`std::thread::id`](https://en.cppreference.com/cpp/thread/thread/id) |



## Member functions 成员函数

| [(constructor)](https://en.cppreference.com/cpp/thread/thread/thread) |            构造新的`jthread`对象(公共成员函数)             |
| ------------------------------------------------------------ | :--------------------------------------------------------: |
| **[(destructor)](https://en.cppreference.com/cpp/thread/thread/~thread)** | **如果线程是可连接的，则请求停止并连接线程(公共成员函数)** |
| **[operator=](https://en.cppreference.com/cpp/thread/thread/operator%3D)** |            **移动`jthread`对象 (公共成员函数)**            |



## Observers 观察者

| [joinable](https://en.cppreference.com/cpp/thread/thread/joinable) | 检查线程是否可连接，即是否可能在并行上下文中运行 **(公共成员函数)** |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[get_id](https://en.cppreference.com/cpp/thread/thread/get_id)** |               **返回线程的id(公共成员函数) **                |
| **[native_handle](https://en.cppreference.com/cpp/thread/thread/native_handle)** |        **返回底层实现定义的线程句柄 (公共成员函数)**         |
| **[hardware_concurrency](https://en.cppreference.com/cpp/thread/thread/hardware_concurrency)**[static] |       **返回实现支持的并发线程数 (公共静态成员函数)**        |



## Operations 操作

|  [join](https://en.cppreference.com/cpp/thread/thread/join)  |       等待线程完成其执行(公共成员函数)       |
| :----------------------------------------------------------: | :------------------------------------------: |
| **[detach](https://en.cppreference.com/cpp/thread/thread/detach)** | **允许线程独立于线程句柄执行(公共成员函数)** |
| **[swap](https://en.cppreference.com/cpp/thread/thread/swap)** |   **交换两个`jthread`对象(公共成员函数)**    |



## Stop token handling 停止令牌处理

| [get_stop_source](https://en.cppreference.com/cpp/thread/jthread/get_stop_source) | 返回与线程的共享停止状态关联的stop_source对象(公共成员函数) |
| :----------------------------------------------------------: | :---------------------------------------------------------: |
| **[get_stop_token](https://en.cppreference.com/cpp/thread/jthread/get_stop_token)** | **返回与线程的共享停止状态关联的stop_token (公共成员函数)** |
| **[request_stop](https://en.cppreference.com/cpp/thread/jthread/request_stop)** |    **通过线程的共享停止状态请求执行停止 (公共成员函数)**    |



## Non-member functions 非成员函数

| [std::swap(std::thread)](https://en.cppreference.com/cpp/thread/thread/swap2)(C++11) | 专门研究std::swap算法 (function) |
| :----------------------------------------------------------: | :------------------------------: |



## 注意

| [Feature-test](https://en.cppreference.com/cpp/utility/feature_test) **macro** |                          **Value**                           |   **Std**   |      **Feature**       |
| :----------------------------------------------------------: | :----------------------------------------------------------: | :---------: | :--------------------: |
| **[`__cpp_lib_jthread`](https://en.cppreference.com/cpp/feature_test#cpp_lib_jthread)** | **[`201911L`](https://en.cppreference.com/cpp/compiler_support/20#cpp_lib_jthread_201911L)** | **(C++20)** | **停止令牌并加入线程** |





# 示例



## 示例一

```c++
#include <iostream>
#include <thread>
#include <windows.h>	//这个头文件与平台相关，如果不是windows平台下的用户，建议把这个头文件以及下面直接使用的Sleep()函数相关内容删掉，不过可能输出的内容可能会乱到一块去
using namespace std;

void worker(int id) {
	cout << "工人" << id << "开始干活了" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "工人" << id << "干完活了" << endl;
}

void increment(int& x) {
	x++;
}

int main() {
	thread t1(worker, 1), t2(worker, 2), t3(worker, 3);	//线程创建时就立刻开始执行、并且是并行执行的谁都不会阻拦到谁

	t1.join();
	t2.join();
	t3.join();
	cout << "主线程：知道工人们都干完活了，好，收工！" << endl;
	Sleep(1200);

	cout << "===============================================================================" << endl;
	int Count = 0;
	cout << "调用线程前Count = " << Count << endl;
	//thread t4(increment, Count);		编译错误！因为无法用右值（拷贝的临时对象）绑定到 int&
	thread t5(increment, ref(Count));
	t5.join();
	cout << "调用用 std::ref 包装的拷贝线程后Count = " << Count << endl;

}
```

在`main`函数的第一行创建的三个线程：`thread t1(worker, 1), t2(worker, 2), t3(worker, 3);`，*注意*：**线程在创建的那一刻就立刻开始并发执行了**，并且由于后续函数调用入栈出栈这些细微的操作导致的时间上细微的差异会使不同线程在运行输出后的顺序奇奇怪怪。**而`join()` 会让调用它的线程（这里是主线程）阻塞，直到 被 `join` 的那个线程 执行完毕。但其他线程完全不受影响，它们早就开始跑了，不会因为主线程在等某个线程而停下来。**

`std::thread` 的构造函数默认是**拷贝**参数。即使线程函数要的是引用，它也会先拷贝一份，把拷贝的引用传进去。所以你想让线程修改主线程的变量，光写引用符号没用。因此，**想让线程修改主线程中的变量，必须显式用 `std::ref()` 包装**，以传递真正的引用。**这就是 `std::ref` 的意义：明确告诉线程，我要传真正的引用，别拷贝。**



## 示例二

```c++
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void worker() {
	cout << "工人 jthread 要开始干活了" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "工人 jthread 干完活了" << endl;
}

void stoppable_worker(stop_token st) {
	int progress = 0;
	while (!st.stop_requested()) {		//只要没被请求停止，就继续循环
		cout << "进度：" << progress++ << endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		if (progress > 10) {
			cout << "工作自己完成了" << endl;
			return;
		}
	}
	cout << "收到停止请求，清理并退出" << endl;
}

int main() {
	//jthread j(worker);
	// 注意，这里没有写 j.join()
	//cout << "主线程：jthread 会在销毁前自动 join，我不需要手动等。" << endl;

	jthread j(stoppable_worker);

	this_thread::sleep_for(chrono::seconds(2));
	cout << "主线程：让他停下来吧" << endl;
	j.request_stop();	//发出停止请求

} // j 离开作用域，析构函数自动 join，安全
```

自动`join()`:

- `std::jthread` 是一个符合 RAII 的“听话”对象。
- 当 `j` 离开作用域（比如 `main` 结束）时，它的**析构函数会自动等待线程执行完毕（自动 `join`）**。
- 我们再也不用担心忘记 `join` 而导致程序崩溃。

协作式停止：

`jthread` 内部维护了一个 `stop_source`，并给线程函数传 `stop_token`。这是一种**优雅、协作式的**停止机制：

- **外部**通过 `j.request_stop()` 发出“请停止”的请求；
- **线程内部**在循环里定期检查 `st.stop_requested()`，一旦发现请求，便自行清理并返回。
  这样绝不可能暴力杀线程，资源可以安全释放，就像“鸣金收兵”一样，士兵自己收兵回营。
