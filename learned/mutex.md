# mutex

`Mutex`要求扩展了[*Lockable*](https://www.cppreference.com/cpp/named_req/Lockable) 要求，以包含线程间同步功能。

### 要求

- [*Lockable 可上锁*](https://www.cppreference.com/cpp/named_req/Lockable)
- [*DefaultConstructible 默认构造*](https://www.cppreference.com/cpp/named_req/DefaultConstructible)
- [*Destructible 可破坏*](https://www.cppreference.com/cpp/named_req/Destructible)
- not copyable 不可复制
- not movable 不可移动

对于 *Mutex* 类型的对象 `m`：

- 表达式` m.lock() `具有以下特性:
  - 表现为原子操作。
  - 阻塞调用线程，直到可以获得`mutex`的独占所有权。
  - 之前对同一互斥体的`m.unlock()`操作与此锁操作同步（相当于*释放-获取`std::memory_order`*）。
  - 如果调用线程已经拥有mutex，则行为未定义（除非 `m` 是`std::recursive_mutex`或`std::recursive_timed_mutex`）。
  - `std::system_error`类型的异常可能会在错误时抛出，错误代码如下：
    - `std::errc::operation_not_permined`，如果调用线程没有所需的权限。
    - 如果实现检测到此操作将导致死锁，则使用`std::errc::resource_deadlock_would_occurce`。
- 表达式`m.try_lock()`具有以下属性:
  - 表现为原子操作。
  - 尝试在不阻塞的情况下获得调用线程 `mutex` 的独占所有权。如果未获得所有权，则立即归还。即使 `mutex` 当前不属于另一个线程，该函数也可以虚假地失败并返回。
  - 如果`try_lock()`成功，则同一对象上的先前`unlock()`操作将与此操作同步（相当于*释放-获取`std::memory_order`*）。`lock()`不与失败的`try_lock()`同步。
  - 不抛出异常。
- 表达式 `m.unlock()` 具有以下属性:
  - 表现为原子操作。
  - 释放调用线程对 `mutex` 的所有权，并与同一对象上的后续成功锁操作同步。
  - 如果调用线程不拥有 `mutex` ，则行为未定义。
  - 不抛出异常。
- 对单个 `mutex` 的所有锁定和解锁操作都以单个总顺序发生，可以将其视为原子变量的[修改顺序 ](https://www.cppreference.com/cpp/atomic/memory_order#Modification_order)：该顺序特定于该单个 `mutex` 。

### 标准库

以下标准库类型满足Mutex要求：

|  **[mutex](https://www.cppreference.com/cpp/thread/mutex)**  |                 **提供基本的互斥机制**                 |
| :----------------------------------------------------------: | :----------------------------------------------------: |
| **[recursive_mutex](https://www.cppreference.com/cpp/thread/recursive_mutex)** |         **提供互斥功能，可由同一线程递归锁定**         |
| **[recursive_timed_mutex](https://www.cppreference.com/cpp/thread/recursive_timed_mutex)** | **提供互斥功能，可由同一线程递归锁定，并实现超时锁定** |
| **[shared_mutex](https://www.cppreference.com/cpp/thread/shared_mutex)** |                  **提供共享互斥功能**                  |
| **[shared_timed_mutex](https://www.cppreference.com/cpp/thread/shared_timed_mutex)** |          **提供共享互斥功能，并实现超时锁定**          |
| **[timed_mutex](https://www.cppreference.com/cpp/thread/timed_mutex)** |             **提供互斥功能，实现超时锁定**             |





## std::mutex

定义在头部 `<mutex>`

```c++
class mutex;
```

**互斥锁（`mutex`）类是一种同步原语，可用于保护共享数据免受多个线程同时访问。**

互斥锁**提供独占的非递归所有权**语义：

- 调用线程拥有一个`mutex`，从它成功调用`lock`或`try_lock`到调用`unlock`。
- 当一个线程拥有一个`mutex`时，如果所有其他线程试图声称拥有该`mutex`的所有权，它们将阻塞（用于锁定调用）或接收`false`的返回值（用于`try_lock`）。
- 在调用`lock`或`try_lock`之前，调用线程不得拥有`mutex`。

如果`mutex`在仍由任何线程拥有时被销毁，或者线程在拥有`mutex`时终止，则程序的行为是未定义的。`mutex`类满足 [*Mutex*](https://www.cppreference.com/cpp/named_req/Mutex)和[*StandardLayoutType*](https://www.cppreference.com/cpp/named_req/StandardLayoutType)的所有要求。

`std::mutex`既不可复制也不可移动。



### Nested types 嵌套类型

|              Name  名称              | Definition  定义 |
| :----------------------------------: | :--------------: |
| **`native_handle_type` (optional*)** |   **实现定义**   |



### Member functions

| [(constructor)](https://www.cppreference.com/cpp/thread/mutex/mutex) |     构造互斥体(公共成员函数)      |
| :----------------------------------------------------------: | :-------------------------------: |
| **[(destructor)](https://www.cppreference.com/cpp/thread/mutex/~mutex)** |   **销毁互斥体 (公共成员函数)**   |
|                    **operator=[deleted]**                    | **不可复制和分配 (公共成员函数)** |



### Locking

|  [lock](https://www.cppreference.com/cpp/thread/mutex/lock)  |      锁定互斥体，如果互斥体不可用则阻止 (公共成员函数)      |
| :----------------------------------------------------------: | :---------------------------------------------------------: |
| **[try_lock](https://www.cppreference.com/cpp/thread/mutex/try_lock)** | **尝试锁定互斥体，如果互斥体不可用，则返回 (公共成员函数)** |
| **[unlock](https://www.cppreference.com/cpp/thread/mutex/unlock)** |                **解锁互斥锁 (公共成员函数)**                |



### Native handle

| [native_handle](https://www.cppreference.com/cpp/thread/mutex/native_handle) | 返回底层实现定义的本机句柄对象(公共成员函数) |
| :----------------------------------------------------------: | :------------------------------------------: |



### 示例

这个例子展示了如何使用互斥体来保护两个线程之间共享的`std::map`。

```c++
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include <string>

using namespace std;
map<string, string> g_pages;
mutex g_pages_mutex;

void save_page(const string& url) {
	//模拟长页面提取
	this_thread::sleep_for(chrono::seconds(2));
	string result = "fake content";

	lock_guard<mutex> guard(g_pages_mutex);
	g_pages[url] = result;
}


int main() {
	thread t1(save_page, "http://foo");
	thread t2(save_page, "http://bar");
	t1.join();
	t2.join();

	//现在无需锁即可安全访问g_pages，因为线程已连接
	for (const auto& [url, page] : g_pages)
		cout << url << "=>" << page << "\n";
}
```

Output:

```c++
http://bar => fake content
http://foo => fake content
```

在执行这一行代码时：`lock_guard<mutex> guard(g_pages_mutex);` 。`guard` 会**立即锁定** `g_pages_mutex`；当 `save_page` 函数结束时，`guard` 会被销毁，它也会**自动解锁**

这种机制叫 **RAII**，可以确保无论在函数里因为什么原因退出（正常返回或抛出异常），锁都会被安全释放-。为了避免因忘记 `unlock` 导致的死锁，C++ 社区会推荐使用 `lock_guard` 等 RAII 工具，而不是直接调用 `lock()/unlock()`。

在这里，t1和t2两个线程它们会**同时进入** `save_page` 函数，然后**各自独立地完成耗时的爬取工作**，只有在真正需要修改 `g_pages` 时，才会受到锁的限制。



## std::recursive_mutex

定义在头部  `<mutex>`

```c++
class recursive_mutex;
```

`recursive_mutex`类是一个同步原语，可用于保护共享数据不被多个线程同时访问。

`recursive_mutex`提供**独占的递归所有权**语义：

- 调用线程在成功调用`lock`或`try_lock`时开始的一段时间内拥有`recursive_mutex`。在此期间，线程可能会进行额外的`lock`或`try_lock`调用。当线程发出匹配数量的`unlock`调用时，所有权期结束。
- 当一个线程拥有`recursive_mutex`时，如果所有其他线程试图声称拥有`recursive_mutex`，它们将阻塞（用于`lock`调用）或收到`false`的返回值（用于`try_lock`）。
- `recursive_mutex`可能被锁定的最大次数未指定，但达到该次数后，对`lock`的调用将抛出`std::system_error`，对`try_lock`的调用将返回`false`。

如果`recursive_mutex`被销毁，但仍由某个线程拥有，则程序的行为是未定义的。`recursive_mutex`类满足[*Mutex*](https://www.cppreference.com/cpp/named_req/Mutex)和[*StandardLayoutType*](https://www.cppreference.com/cpp/named_req/StandardLayoutType)的所有要求。

### 成员类型

|             Member type              | Definition  定义 |
| :----------------------------------: | :--------------: |
| **`native_handle_type` (optional*)** |   **实现定义**   |



### Member functions

| [(constructor)](https://www.cppreference.com/cpp/thread/mutex/mutex) |     构造互斥体(公共成员函数)      |
| :----------------------------------------------------------: | :-------------------------------: |
| **[(destructor)](https://www.cppreference.com/cpp/thread/mutex/~mutex)** |   **销毁互斥体 (公共成员函数)**   |
|                    **operator=[deleted]**                    | **不可复制和分配 (公共成员函数)** |



### Locking

|  [lock](https://www.cppreference.com/cpp/thread/mutex/lock)  |      锁定互斥体，如果互斥体不可用则阻止 (公共成员函数)      |
| :----------------------------------------------------------: | :---------------------------------------------------------: |
| **[try_lock](https://www.cppreference.com/cpp/thread/mutex/try_lock)** | **尝试锁定互斥体，如果互斥体不可用，则返回 (公共成员函数)** |
| **[unlock](https://www.cppreference.com/cpp/thread/mutex/unlock)** |                **解锁互斥锁 (公共成员函数)**                |



### Native handle

| [native_handle](https://www.cppreference.com/cpp/thread/mutex/native_handle) | 返回底层实现定义的本机句柄对象(公共成员函数) |
| :----------------------------------------------------------: | :------------------------------------------: |



### 示例

`recursive_mutex`的一个用例是保护类中的共享状态，该类的成员函数可以相互调用。

```c++
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

class X {
	recursive_mutex m;
	string shared;
public:
	void fun1() {
		lock_guard<recursive_mutex> lk(m);
		shared = "fun1";
		cout << "in fun1, shared varible is now " << shared << '\n';
	}
	void fun2() {
		lock_guard<recursive_mutex> lk(m);
		shared = "fun2";
		cout << "in fun2, shared variable is now " << shared << '\n';
		fun1();		//recursive lock 在这里变得有用
		cout << "back in fun2,shared variable is " << shared << '\n';	//这里shared变成fun1了
	}
};

int main() {
	X x;
	//thread t1(&X::fun1, &x);
	thread t2(&X::fun2, &x);
	//t1.join();
	t2.join();
}
```

### 关于上面代码两个疑问的解释

#### 1. 递归锁在 fun2 里调 fun1 为什么没阻塞？

“另一个线程”指的是真正操作系统层面的**不同的线程**。在上面 `main` 函数里只起了 `t2` 这一个线程。

当 `t2` 执行 `fun2()` 时，`fun2` 里的 `lock_guard` 已经成功锁住了 `m`，**也就是说锁的拥有者是 `t2`**。接着 `fun2` 内部调用了 `fun1`，`fun1` 里的 `lock_guard` 再次尝试锁定 `m`。
因为 `recursive_mutex` 允许**同一个线程**重复加锁（它内部有个计数器），**所以它发现还是 `t2`，就直接把计数器加一，放过去了，没有阻塞**。所以这里根本不存在“另一个线程”来抢锁的情况，`fun1` 顺顺利利就执行完了。 

等 `fun1` 返回、`fun2` 也返回之后，那些 `lock_guard` 依次析构，计数器归零，锁才真正释放。

####  2.关于 `thread t2(&X::fun2, &x)` 的语法

这个语法是调用**非静态成员函数**的**标准固定写法**，可以拆解为：

```c++
std::thread 线程对象(指向成员函数的指针, 指向对象的指针, 参数1, 参数2...);
```

- **`&X::fun2`**：这表示“类 `X` 的成员函数 `fun2`”，这是一个**成员函数指针**。你需要用 `&` 取地址，并带上类名限定 `X::` 来告诉编译器它的归属。这是固定的，和普适函数不同。
- **`&x`**：这是指向要操作的**具体对象 `x` 的指针**。成员函数总得在一个具体的对象上调用，你得让线程知道是哪个对象的 `fun2`。这里用 `&` 取对象 `x` 的地址。



## STD::shared_mutex

定义在头部 `<shared_mutex>`

```c++
class shared_mutex;
```

`shared_mutex`类是一种同步原语，可用于保护共享数据，防止多个线程同时对其进行访问。与其他支持独占访问的互斥量类型不同，**`shared_mutex`具有两级访问权限**：

- 共享(*shared*) - 多个线程可以共享同一个互斥锁的所有权。
- 互斥(*exclusive*) - 只有一个线程可以拥有互斥锁。

**如果一个线程已经获取了互斥锁（通过 `lock`、`try_lock`），则其他线程无法获取该锁（包括共享线程）。**

**如果一个线程已经获取了共享锁（通过lock_shared、try_lock_shared），则其他线程无法获取独占锁，但可以获取共享锁。**

只有当互斥锁未被任何线程获取时，共享锁才能被多个线程获取。

在一个线程中，同一时间只能获取一个锁（共享锁或互斥锁）。

**当共享数据可以被任意数量的线程同时安全读取**，但一个线程只有在没有其他线程同时进行读取或写入时才能写入相同的数据时，共享互斥锁特别有用。

`shared_mutex`类满足[*SharedMutex*](https://www.cppreference.com/cpp/named_req/SharedMutex)和[*StandardLayoutType*](https://www.cppreference.com/cpp/named_req/StandardLayoutType)的所有要求。

### Member types

|             Member type              |  Definition  |
| :----------------------------------: | :----------: |
| **`native_handle_type` (optional*)** | **实现定义** |

### Member functions

| **[(constructor)](https://www.cppreference.com/cpp/thread/shared_mutex/shared_mutex)** | **构造互斥锁 （公有成员函数）**  |
| :----------------------------------------------------------: | :------------------------------: |
| **[(destructor)](https://www.cppreference.com/cpp/thread/shared_mutex/~shared_mutex)** |  **销毁互斥量（公有成员函数）**  |
|                    **operator=[deleted]**                    | **不可复制赋值（公有成员函数）** |

#### Exclusive locking

| **[lock](https://www.cppreference.com/cpp/thread/shared_mutex/lock)** |   **锁定互斥量，若互斥量不可用则阻塞 （公有成员函数）**    |
| :----------------------------------------------------------: | :--------------------------------------------------------: |
| **[try_lock](https://www.cppreference.com/cpp/thread/shared_mutex/try_lock)** | **尝试锁定互斥量，如果互斥量不可用则返回（公有成员函数）** |
| **[unlock](https://www.cppreference.com/cpp/thread/shared_mutex/unlock)** |               **解锁互斥锁（公有成员函数）**               |

#### Shared locking

| **[lock_shared](https://www.cppreference.com/cpp/thread/shared_mutex/lock_shared)** | **锁定共享所有权的互斥锁，如果互斥锁不可用则阻塞 （公有成员函数）** |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[try_lock_shared](https://www.cppreference.com/cpp/thread/shared_mutex/try_lock_shared)** | **尝试锁定互斥量以实现共享所有权，如果互斥量不可用则返回（公有成员函数）** |
| **[unlock_shared](https://www.cppreference.com/cpp/thread/shared_mutex/unlock_shared)** |         **解锁互斥锁（共享所有权）（公有成员函数）**         |

#### Native handle(原生句柄)

| [native_handle](https://www.cppreference.com/cpp/thread/shared_mutex/native_handle) | 返回底层实现定义的原生句柄对象（公有成员函数） |
| :----------------------------------------------------------: | :--------------------------------------------: |



### 示例

```c++
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <mutex>
#include <syncstream>

class ThreadSafeCounter {
public:
	ThreadSafeCounter() = default;

	//多个线程 / 读取器可以同时读取计数器的值。
	unsigned int get() const {
		std::shared_lock lock(mutex_);
		return value_;
	}

	// 只有一个线程/写入器可以增加/写入计数器的值。
	void increment() {
		std::unique_lock lock(mutex_);
		++value_;
	}

	//只有一个线程/写入器可以重置/写入计数器的值。
	void reset() {
		std::unique_lock lock(mutex_);
		value_ = 0;
	}

private:
	mutable std::shared_mutex mutex_;
	unsigned int value_{};
};

int main() {
	ThreadSafeCounter counter;

	auto increment_and_print = [&counter]() {
		for (int i{}; i != 3; ++i) {
			counter.increment();
			std::osyncstream(std::cout)
				<< std::this_thread::get_id() << ' ' << counter.get() << '\n';
		}
		};

	std::thread thread1(increment_and_print);
	std::thread thread2(increment_and_print);

	thread1.join();
	thread2.join();
}
```

![image-20260515162348354](C:\Users\28251\AppData\Roaming\Typora\typora-user-images\image-20260515162348354.png)

#### **`std::shared_lock<std::shared_mutex> lock(mutex_);`**与**`std::shared_lock lock(mutex_);`**的区别

- **`std::shared_lock<std::shared_mutex> lock(mutex_);`**

  显式指定 `shared_lock` 的模板参数为 `std::shared_mutex`。它要求 `mutex_` 的类型**严格匹配** `std::shared_mutex&`。 

- **`std::shared_lock lock(mutex_);`**

  使用 C++17 引入的 **类模板实参推导（CTAD）**，编译器根据 `mutex_` 的类型自动推导出 `shared_lock` 的模板参数。`mutex_` 是什么类型，推导结果就是什么类型（比如 `std::shared_mutex`、`std::shared_timed_mutex` 等）。



## std::lock_guard

定义在头部  `<mutex>`

```c++
template< class Mutex >
class lock_guard;
```

`lock_guard` 类是一个 `mutex` 包装器，它提供了一种方便的 [RAII](https://www.cppreference.com/cpp/language/raii)风格机制，用于在作用域块的持续时间内拥有 `mutex` 。

**当创建 `lock_guard` 对象时，它会尝试获取所给 `mutex` 的所有权。当控件离开创建 `lock_guard` 对象的作用域时， `lock_guard` 将被销毁， `mutex` 将被释放。**

`lock_guard` 是不可复制的。

### 模板参数

| Mutex | 要锁定的 `mutex` 类型。该类型必须满足[*BasicLockable*](https://www.cppreference.com/cpp/named_req/BasicLockable)要求 |
| :---: | :----------------------------------------------------------: |



### 成员类型

|  Member type   | Definition  定义 |
| :------------: | :--------------: |
| **mutex_type** |    **Mutex**     |



### Member functions

| [(constructor)](https://www.cppreference.com/cpp/thread/mutex/mutex) | 构造一个 `lock_guard` ，可以选择锁定给定的 `mutex`(公共成员函数) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[(destructor)](https://www.cppreference.com/cpp/thread/mutex/~mutex)** | **销毁 `lock_guard` 对象，解锁底层的 `mutex` (公共成员函数)** |
|                    **operator=[deleted]**                    |              **不可复制和分配 (公共成员函数)**               |



### 注意

*一个常见的初学者错误是“忘记”给 `lock_guard` 变量命名，例如 `std::lock_guard(mtx);` （默认构造一个名为 `mtx` 的`lock_guard` 变量）或 `std::lock_guard{mtx};` （它构造了一个立即被销毁的 `prvalue` 对象），因此实际上并没有构造一个在其余作用域中保存 `mutex` 的锁。*



### 示例

通过两个线程演示volatile变量的安全和不安全增量。

```c++
#include <iostream>
#include <thread>
#include <mutex>
#include <string_view>
#include <syncstream>

using namespace std;

volatile int g_i = 0;
mutex g_i_mutex;	//保护 g_i

void safe_increment(int iterations) {
	const lock_guard<mutex> lock(g_i_mutex);
	while (iterations-- > 0)
		g_i = g_i + 1;
	cout << "thread #" << this_thread::get_id() << ", g_i: " << g_i << '\n';
	//当lock超出作用域时，g_i_mutex会自动释放
}

void unsafe_increment(int iterations) {
	while (iterations-- > 0)
		g_i = g_i + 1;
	osyncstream(cout) << "thread #" << this_thread::get_id() << " ,g_i: " << g_i << '\n';
}

int main() {
	auto test = [](string_view fun_name, auto fun) {
		g_i = 0;
		cout << fun_name << ":\nbefore,g_i: " << g_i << '\n';
		{
			std::jthread t1(fun, 1'000'000);
			std::jthread t2(fun, 1'000'000);
		}
		cout << "after, g_i: " << g_i << "\n\n";
	};
	test("safe_increment", safe_increment);
	test("unsafe_increment", unsafe_increment);
}

```

#### 1. `volatile` 在多线程中的真实作用与陷阱

虽然 `g_i` 是全局的 `volatile int`，两个函数访问的都是同一个 `volatile` 变量，但 `volatile` 并不能保证线程安全。

- **`volatile` 只保证“每一次读写都真实发生在内存上”**，禁止编译器将变量缓存到寄存器或把循环优化成一条加法指令（例如 `g_i += iterations`）。因此，它在示例里能**稳定地复现数据竞争导致的错误值**，让演示更直观。
- **`volatile` 不提供原子性**：`g_i = g_i + 1` 在 CPU 层面至少是“读-改-写”三条指令，多线程下会互相交叉，导致丢失计数。
- 真正的线程安全来自 `std::mutex`（或 `std::atomic`）。`safe_increment` 里的锁保证了同一时间只有一个线程执行“读-改-写”，因此结果是正确的。

#### 2. `<string_view>`：轻量的字符串视图

- `string_view` 本质上就是一个指针加一个长度，不拥有字符串数据本身。传递它时只复制这两个字段，不复制字符串内容。
- 代码中 `test("safe_increment", ...)` 传入的字符串字面量存储在程序的静态数据区，生命周期贯穿整个程序，所以用 `string_view` 指向它绝对安全。
- **注意**：如果 `string_view` 指向一个临时 `std::string`，临时对象销毁后视图会悬空，使用时必须保证数据有效。

#### 3. `<syncstream>`：线程安全的输出缓冲

- 多线程直接往 `cout` 写内容时，多个 `operator<<` 调用可能交错，导致输出乱掉。
- `osyncstream(cout)` 会创建一个内部带缓冲的临时对象，所有的 `<<` 操作都先写入这个缓冲区，**在语句结束时，再将整条消息原子地推送给 `cout`**，保证每一行的输出都是完整、不混乱的。
- 这在调试多线程代码时非常有用，能以较小的性能代价换来清晰的日志。

#### 4. `const lock_guard<mutex> lock(...)` 中 `const` 的意义

`std::lock_guard` 本身不可拷贝、不可移动，也没有 `unlock()` 成员函数，对象一旦创建就不能被修改。加 `const` 更大程度上是一种**防御性编程**：

- 明确传达“这个锁在整个作用域内绝不会被重新绑定或提前释放”的语义。
- 防止将来的代码维护者不小心写出试图修改锁的代码，或者将 `lock_guard` 替换为其他带 `unlock()` 的 RAII 包装器时误用。
- 虽然对 `lock_guard` 本身性能无影响，但在更复杂的类中能增强整体 `const` 正确性。

#### 5. `auto test = [](string_view fun_name, auto fun)` 泛型 Lambda

这是 C++14 引入的**泛型 Lambda**，参数表中的 `auto` 表示该参数的类型由编译器根据调用时的实参推导：

- 相当于编译器生成了一个带模板 `operator()` 的匿名函数对象。
- 调用 `test("safe_increment", safe_increment)` 时，`fun` 的类型被推导为 `void(*)(int)`。
- 这种写法的好处是：可以接受任何可调用对象（函数指针、Lambda、函数对象等），而无需使用 `std::function` 带来额外的包装开销。
- Lambda 本身是 `const` 的，且通过值捕获，`string_view` 和函数指针的传递都非常轻量。





## std::unique_lock

定义在头部  `<mutex>`

```c++
template< class Mutex >
class unique_lock;
```

类 `unique_lock` 是一个通用的 `mutex` 所有权包装器，允许延迟锁定、有时间限制的锁定尝试、递归锁定、锁所有权的转移以及与条件变量一起使用。

类 `unique_lock` 是可移动的，但不可复制的 `——` 它符合[*MoveConstructible*](https://www.cppreference.com/cpp/named_req/MoveConstructible)和[*MoveAssignable*](https://www.cppreference.com/cpp/named_req/MoveAssignable)的要求，但不符合 [*CopyConstructible*](https://www.cppreference.com/cpp/named_req/CopyConstructible)或 [*CopyAssignable*](https://www.cppreference.com/cpp/named_req/CopyAssignable)的规定。

类 `unique_lock` 符合[*BasicLockable*](https://www.cppreference.com/cpp/named_req/BasicLockable)要求。如果 `Mutex` 满足可锁定要求，那么 `unique_lock` 也满足可锁定的要求（例如：可以在 `std::lock` 中使用）；如果 `Mutex` 满足  [*TimedLockable*](https://www.cppreference.com/cpp/named_req/TimedLockable)要求，则 `unique_lock` 也满足 [*TimedLockable*](https://www.cppreference.com/cpp/named_req/TimedLockable)要求。

### 模板参数

| Mutex | 要锁定的 `mutex` 类型。该类型必须满足[*BasicLockable*](https://www.cppreference.com/cpp/named_req/BasicLockable)要求 |
| :---: | :----------------------------------------------------------: |



### 成员类型

|  Member type   | Definition  定义 |
| :------------: | :--------------: |
| **mutex_type** |    **Mutex**     |



### Member functions

| [(constructor)](https://www.cppreference.com/cpp/thread/mutex/mutex) | 构造一个 `unique_lock` ，可以选择锁定（即拥有）提供的互斥体 `mutex`(公共成员函数) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[(destructor)](https://www.cppreference.com/cpp/thread/mutex/~mutex)** | **如果拥有，则解锁（即释放）相关 ` mutex` 的所有权 (公共成员函数)** |
| **[operator=](https://www.cppreference.com/cpp/thread/unique_lock/operator%3D)** | **解锁（即释放） ` mutex` （如果拥有），并获得另一个 ` mutex` 的所有权 (公共成员函数)** |



### Locking

|  [lock](https://www.cppreference.com/cpp/thread/mutex/lock)  |        锁定（即拥有）关联的 ` mutex`  (公共成员函数)         |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[try_lock](https://www.cppreference.com/cpp/thread/mutex/try_lock)** | **尝试在不阻塞的情况下锁定（即拥有）关联的 ` mutex`  (公共成员函数)** |
| **[try_lock_for](https://www.cppreference.com/cpp/thread/unique_lock/try_lock_for)** | **尝试锁定（即拥有）关联的[*TimedLockable*](https://www.cppreference.com/cpp/named_req/TimedLockable)  ` mutex` ，如果 ` mutex` 在指定的持续时间内不可用，则返回(公共成员函数)** |
| **[try_lock_until](https://www.cppreference.com/cpp/thread/unique_lock/try_lock_until)** | **尝试锁定（即拥有）关联的[*TimedLockable*](https://www.cppreference.com/cpp/named_req/TimedLockable)  ` mutex` ，如果 ` mutex` 在达到指定时间点之前不可用，则返回(公共成员函数)** |
| **[unlock](https://www.cppreference.com/cpp/thread/unique_lock/unlock)** |    **解锁（即释放）相关 ` mutex` 的所有权(公共成员函数)**    |



### 修饰符

| **[swap](https://www.cppreference.com/cpp/thread/unique_lock/swap)** |    **用另一个 `std::unique_lock` 交换状态(公共成员函数)**    |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[release](https://www.cppreference.com/cpp/thread/unique_lock/release)** | **在不解锁（即释放所有权）的情况下解除关联 ` mutex` 的关联**(公共成员函数) |



### 观察者

| **[mutex](https://www.cppreference.com/cpp/thread/unique_lock/mutex)** |        **返回指向关联 ` mutex` 的指针**(公共成员函数)        |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[owns_lock](https://www.cppreference.com/cpp/thread/unique_lock/owns_lock)** | **测试锁是否拥有（即已锁定）其关联的 ` mutex` **(公共成员函数) |
| **[operator bool](https://www.cppreference.com/cpp/thread/unique_lock/operator_bool)** | **测试锁是否拥有（即已锁定）其关联的 ` mutex` **(公共成员函数) |



### 非成员函数

| [std::swap(std::unique_lock)](https://www.cppreference.com/cpp/thread/unique_lock/swap2) | 专门研究 `std::swap` 算法（功能模板） |
| :----------------------------------------------------------: | :-----------------------------------: |



### 示例

```c++
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

struct Box {
	explicit Box(int num):num_things{num}{}

	int num_things;
	mutex m;
};

void transfer(Box& from, Box& to, int num) {
	//还没把锁拿走
	unique_lock lock1{ from.m,defer_lock };
	unique_lock lock2{ to.m,defer_lock };

	//锁定两个unique_locks而不死锁
	lock(lock1, lock2);
	from.num_things -= num;
	to.num_things += num;

	//“from.m”和“to.m”互斥体在unique_lock dtors中解锁
}

int main() {
	Box acc1{ 100 };
	Box acc2{ 50 };
	
	thread t1{ transfer,ref(acc1),ref(acc2),10 };
	thread t2{ transfer,ref(acc2),ref(acc1),5 };

	t1.join();
	t2.join();

	cout << "acc1: " << acc1.num_things << "\n"
		"acc2: " << acc2.num_things << "\n";
}
```



####  1. `explicit` 关键字

```c++
explicit Box(int num):num_things{num}{}
```



它明确禁止了隐式类型转换。如果不加 `explicit`，像 `Box b = 10;` 这样的代码就能编译通过，编译器会“偷偷地”用整数 `10` 构造一个 `Box` 对象。加上后，就必须显式地使用 `Box b(10);` 来创建对象，这避免了那种不合常理或非预期的隐式转换，让代码意图更清晰。



#### 2. `num_things{num}` 语法

这是 **C++11 引入的“列表初始化”**，用大括号 `{}` 来给成员变量赋初值，是一种更现代、更安全的写法。

- **直观理解**：`num_things{num}` 就等价于你熟悉的 `num_things(num)`，都是用 `num` 的值来初始化成员。
- **主要优势**：大括号初始化最大的好处是**不允许窄化转换**。例如，用 `int` 去初始化 `short` 成员，小括号会默默截断导致数据丢失，而大括号会在编译阶段直接报错，把风险扼杀在摇篮里。



#### 3. `defer_lock` 与 `unique_lock` 的构造

```c++
unique_lock lock1{ from.m, defer_lock };
```



这一行的意思是：“创建一个 `unique_lock` 对象 `lock1` 来关联 `from.m` 这个互斥量，但是，**先别急着上锁（defer locking）**。我只是接管了管理权，锁还是开着的。”

这种做法通常是多步加锁策略的一部分，常见的有以下三种：

- **`std::defer_lock`：先关联互斥量，但不上锁。由我来决定何时调用 `lock()`。**
- **`std::adopt_lock`**：假设当前线程已经锁住了该互斥量。由我来接管这个互斥量的解锁责任。
- **`std::try_to_lock`**：试图锁定，但如果互斥量已经被他人持有，则立即返回，不阻塞当前线程。



#### 4. `std::lock(lock1, lock2)` 的多锁防死锁机制

```c++
lock(lock1, lock2);
```



这是防止死锁的“王牌”。它使得 `lock1` 和 `lock2` 所关联的互斥量**同时**被锁定。

- **原子操作**：从最终效果看，`lock1` 和 `lock2` 像是作为一个不可分割的整体被锁定的。
- **死锁避免算法**：底层会使用类似“排序后尝试”或“回退重试”的算法。比如，如果发现两个锁无法同时获取，它会先解锁已经拿到的，等待一会儿再试，从而巧妙地打破循环等待的死锁条件，避免了 `t1(A, B)` 和 `t2(B, A)` 这类经典死锁。

反过来想，`safe_increment` 里的 `lock_guard` 只锁一个变量，所以不存在死锁问题，直接构造就锁定最为高效。`transfer` 需要锁两个不同账号，才必须引入 `defer_lock` 配合 `std::lock` 来解决死锁问题。

####  总结

这个 `transfer` 函数是一个经典的“多资源”无死锁转移模型：

1. **申请所有权，但不上锁**：两行 `unique_lock lock{ m, defer_lock }` 分别声明了对两个互斥量的管理权。
2. **原子性、无死锁地加锁**：关键的一行 `std::lock(lock1, lock2)`，以原子方式同时锁定两个锁，从根源上避免了死锁。
3. **执行临界区操作**：锁全部拿下后，安全地修改 `from` 和 `to` 的数据。
4. **RAII自动解锁**：函数结束时，`lock1` 和 `lock2` 这两个 RAII 对象析构，自动调用`unlock`释放两个互斥量。



### `unique_lock` 是什么？

如果说 `lock_guard` 更像一个轻量级的“一次性门锁”（构造即锁，析构即解），`unique_lock` 则更像一个功能全面的“多模式智能门锁”。它除了提供基础的 RAII 机制，还解锁了以下几项关键能力：

- **支持延迟锁定与手动控制**：可以创建后暂不锁定，手动调用 `lock()`、`try_lock()` 和 `unlock()`。这让它能在“准备工作时”先不持锁，或是在代码块中途**临时解锁**再重新锁定。
- **支持移动语义（move semantics）**：“锁的所有权”是可以转移的（比如通过函数返回或放入容器），这是 `lock_guard` 做不到的。这一点对于实现一些复杂的锁管理模式至关重要。
- **配合条件变量**：`std::condition_variable::wait()` 等函数要求传入一个 `std::unique_lock`，这是它的一个“独家”特权，也是用它的最常见理由之一。
- **因为灵活，所以有代价**：功能更多，也意味着它在内存占用和执行效率上比简单的 `lock_guard` 要稍微高一点。

简单来说：**能用 `lock_guard` 搞定的简单场景就用 `lock_guard`，但一旦需要延迟锁定、配合条件变量或转移所有权，就必须用 `unique_lock`**。



## std::shared_lock

Defined in header `<shared_mutex>`

```c++
template< class Mutex >
class shared_lock;
```

`shared_lock`类是一个通用的共享互斥量所有权包装器，支持延迟锁定、定时锁定和锁定所有权的转移。**锁定shared_lock会以共享模式锁定相关的共享互斥量（若要以独占模式锁定，可使用`std::unique_lock`）。**

**`shared_lock`类是可移动的，但不可复制的**——它满足[*MoveConstructible*](https://www.cppreference.com/cpp/named_req/MoveConstructible)和[*MoveAssignable*](https://www.cppreference.com/cpp/named_req/MoveAssignable)的要求，但不满足[*CopyConstructible*](https://www.cppreference.com/cpp/named_req/CopyConstructible)或 [*CopyAssignable*](https://www.cppreference.com/cpp/named_req/CopyAssignable)的要求。

`shared_lock`满足[*Lockable*](https://www.cppreference.com/cpp/named_req/Lockable)要求。如果`Mutex`满足 [*SharedTimedLockable*](https://www.cppreference.com/cpp/named_req/SharedTimedLockable)要求，则`shared_lock`也满足 [*TimedLockable*](https://www.cppreference.com/cpp/named_req/TimedLockable)要求。

**为了在共享所有权模式下等待共享互斥量，可以使用`std::condition_variable_any`**（`std::condition_variable`需要`std::unique_lock`，因此只能在独占所有权模式下等待）。



### 模板参数

Mutex -  要锁定的共享互斥体的类型。该类型必须满足[*SharedLockable*](https://www.cppreference.com/cpp/named_req/SharedLockable)要求

### Member types

|     Type     | Definition |
| :----------: | :--------: |
| `mutex_type` |  `Mutex`   |

### Member functions

| **[(constructor)](https://www.cppreference.com/cpp/thread/shared_lock/shared_lock)** | **构造一个`shared_lock`，可选择是否锁定提供的互斥锁（公共成员函数）** |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[(destructor)](https://www.cppreference.com/cpp/thread/shared_lock/~shared_lock)** |             **解锁关联的互斥锁（公共成员函数）**             |
| **[operator=](https://www.cppreference.com/cpp/thread/shared_lock/operator%3D)** | **如果拥有互斥锁，则解锁该互斥锁，并获取另一个互斥锁的所有权（公共成员函数）** |

#### Shared locking

| **[lock](https://www.cppreference.com/cpp/thread/shared_lock/lock)** |            **锁定关联的互斥锁（公共成员函数）**            |
| :----------------------------------------------------------: | :--------------------------------------------------------: |
| **[try_lock](https://www.cppreference.com/cpp/thread/shared_lock/try_lock)** |         **尝试锁定相关联的互斥锁（公共成员函数）**         |
| **[try_lock_for](https://www.cppreference.com/cpp/thread/shared_lock/try_lock_for)** | **尝试在指定的持续时间内锁定相关的互斥锁（公共成员函数）** |
| **[try_lock_until](https://www.cppreference.com/cpp/thread/shared_lock/try_lock_until)** | **尝试锁定关联的互斥锁，直到指定的时间点（公共成员函数）** |
| **[unlock](https://www.cppreference.com/cpp/thread/shared_lock/unlock)** |            **解锁关联的互斥锁（公共成员函数）**            |

#### Modifiers

| **[swap](https://www.cppreference.com/cpp/thread/shared_lock/swap)** | **用另一个`shared_lock`来交换数据成员（公共成员函数）** |
| :----------------------------------------------------------: | :-----------------------------------------------------: |
| **[release](https://www.cppreference.com/cpp/thread/shared_lock/release)** |        **互斥锁解关联而不解锁（公共成员函数）**         |

#### Observers

| **[mutex](https://www.cppreference.com/cpp/thread/shared_lock/mutex)** |  **返回一个指向相关互斥锁的指针（公共成员函数）**   |
| :----------------------------------------------------------: | :-------------------------------------------------: |
| **[owns_lock](https://www.cppreference.com/cpp/thread/shared_lock/owns_lock)** | **测试该锁是否拥有其关联的互斥量（公共成员函数）**  |
| **[operator bool](https://www.cppreference.com/cpp/thread/shared_lock/operator_bool)** | **测试该锁是否拥有其关联的互斥量 （公共成员函数）** |

### Non-member functions

| [std::swap(std::shared_lock)](https://www.cppreference.com/cpp/thread/shared_lock/swap2)(C++14) | 专用于std::swap算法（函数模板） |
| :----------------------------------------------------------: | :-----------------------------: |



## std::scoped_lock

定义在头部  `<mutex>`

```c++
template< class... MutexTypes >
class scoped_lock;
```

`scoped_lock` 类是一个 `mutex` 包装器，它提供了一种方便的 [RAII-style](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization)机制，在作用域块的持续时间内拥有零个或多个互斥体。

当创建 `scoped_lock` 对象时，它会尝试获取所给互斥对象的所有权。当控件离开创建 `scoped_lock` 对象的作用域时， `scoped_lock` 将被销毁，互斥体将被释放。若给定了多个互斥体，则使用死锁避免算法，就像使用 `std::lock` 一样。

 `scoped_lock` 类是不可复制的。

### 模板参数

| MutexTypes | 要锁定的互斥类型。类型必须满足[*Lockable*](https://www.cppreference.com/cpp/named_req/Lockable) 要求，除非 `sizeof...(MutexTypes) == 1` ，在这种情况下，唯一的类型必须满足[*BasicLockable*](https://www.cppreference.com/cpp/named_req/BasicLockable) |
| :--------: | :----------------------------------------------------------: |



### 成员类型

|  Member type   |                       Definition  定义                       |
| :------------: | :----------------------------------------------------------: |
| **mutex_type** | **如果 `sizeof...（MutexTypes）==1` ，则成员类型 `mutex_type` 与 `mutex` 相同， `mutex` 是互斥类型中的唯一类型。…否则，没有成员 `mutex_type` 。** |



### Member functions

| [(constructor)](https://www.cppreference.com/cpp/thread/mutex/mutex) | 构造一个 `scoped_lock` ，可以选择锁定给定的互斥对象(公共成员函数) |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[(destructor)](https://www.cppreference.com/cpp/thread/mutex/~mutex)** |  **销毁 `scoped_lock` 对象，解锁底层互斥体 (公共成员函数)**  |
|                    **operator=[deleted]**                    |              **不可复制和分配 (公共成员函数)**               |



### 注意

一个常见的初学者错误是“忘记”给 `scoped_lock` 变量命名，例如 `std::scoped_lock(mtx);` （默认构造一个名为 `mtx` 的 `scoped_lock` 变量）或 `std::scoped_lock{mtx};` （它构造了一个立即被销毁的 `prvalue` 对象），因此实际上并没有构造一个在其余作用域中保存互斥体的锁。

| [Feature-test](https://www.cppreference.com/cpp/utility/feature_test) **macro**(功能测试宏) |                          **Value**                           | **Std** |                         **Feature**                          |
| :----------------------------------------------------------: | :----------------------------------------------------------: | :-----: | :----------------------------------------------------------: |
| [`__cpp_lib_scoped_lock`](https://www.cppreference.com/cpp/feature_test#cpp_lib_scoped_lock) | [`201703L`](https://www.cppreference.com/cpp/compiler_support/17#cpp_lib_scoped_lock_201703L) | (C++17) | [`std::scoped_lock`](https://www.cppreference.com/cpp/thread/scoped_lock#Top) |



### 示例

以下示例使用 `std::scoped_lock` 来锁定成对的互斥体，而不会死锁，并且是RAII风格的。

```c++
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <syncstream>
#include <functional>
#include <vector>

using namespace std;
using namespace chrono_literals;

struct Employee {
	vector<string> lunch_partners;
	string id;
	mutex m;
	Employee(string id):id(id){}
	string partners()const {	//const 约束的是函数体内的操作，你不能在这个函数里修改成员变量 id 或 lunch_partners。
		string ret = "Employee " + id + " has lunch partners: ";
		for (int count{}; const auto& partner:lunch_partners)
			ret += (count++ ? ", " : " ") + partner;
		return ret;
	}
};

void send_mail(Employee&, Employee&) {
	//模拟耗时的消息传递操作
	this_thread::sleep_for(1s);		//或者：chrono::seconds(1)
}

void assign_lunch_partner(Employee& e1, Employee& e2) {
	osyncstream synced_out(cout);
	synced_out << e1.id << " and " << e2.id << " are waiting for locks" << endl;

	{
		//使用std::scoped_lock获取两个锁，而不必担心其他对assign_lunch_partner的调用会使我们死锁，它还提供了一种方便的RAII风格机制
		scoped_lock lock(e1.m, e2.m);

		//等效代码1（使用std::lock和std::lock_guard）
		//std::lock(e1.m, e2.m);
		//std::lock_guard<std::mutex> lk1(e1.m, std::adopt_lock);
		//std::lock_guard<std::mutex> lk2(e2.m, std::adopt_lock);

		//等效代码2（如果需要unique_locks，例如用于条件变量）
		//std::unique_lock<std::mutex> lk1(e1.m, std::defer_lock);
		//std::unique_lock<std::mutex> lk2(e2.m, std::defer_lock);
		//std::lock(lk1, lk2);

		synced_out << e1.id << " and " << e2.id << " got locks " << endl;
		e1.lunch_partners.push_back(e2.id);
		e2.lunch_partners.push_back(e1.id);
	}

	send_mail(e1, e2);
	send_mail(e2, e1);
}

int main() {
	Employee alice("Alice"), bob("Bob"), christina("Christina"), dave("Dave");

	//以并行线程进行分配，因为向用户发送午餐分配的邮件需要很长时间
	vector<thread> threads;
	threads.emplace_back(assign_lunch_partner, ref(alice), ref(bob));
	threads.emplace_back(assign_lunch_partner, ref(christina), ref(bob));
	threads.emplace_back(assign_lunch_partner, ref(christina), ref(alice));
	threads.emplace_back(assign_lunch_partner, ref(dave), ref(bob));

	for (auto& thread : threads)
		thread.join();
	osyncstream(cout) << alice.partners() << '\n'
		<< bob.partners() << '\n'
		<< christina.partners() << '\n'
		<< dave.partners() << '\n';
}
```



#### 1. `string partners() const { ... }` 中的 `const`

这是**常成员函数**。它承诺此函数**不会修改当前对象的任何成员变量**（`mutable` 修饰的除外）



#### 2. `for (int count{}; const auto& partner: lunch_partners)` 循环语法

这是 **C++20 增强的基于范围的 `for` 循环**，可以在循环头部第一句添加一个初始化语句-。

- **拆解**：`int count{};` 是**初始化语句**。它定义并初始化了变量 `count`（`{}` 表示值初始化为0），这个变量的作用域仅限于 `for` 循环内部。
- **传统等价写法**：相当于把 `int count = 0;` 移到了 `for` 循环内部。
- **优势**：语法更紧凑，让辅助变量（如这里的计数器）的作用域最小化，代码意图更清晰。



#### 3. `osyncstream synced_out(cout);` 在干嘛？

它在**创建一个具名的、线程安全的输出包装器**。与你之前看到的 `lock_guard` 示例中的临时对象用法不同，这里 `synced_out` 是一个可以多次使用的具名变量-。

- **目的**：保证后续所有通过 `synced_out` 对象写入 `cout` 的操作，最终会作为一个整体被提交，不会被其他线程的输出打断、造成乱码。
- **对比之前的用法**：之前 `lock_guard` 示例中是 `osyncstream(cout) << ...`，那是一个**临时对象**，生命周期仅限于该条输出语句。而这里创建了一个具名变量，可以像使用普通流那样，在后续代码中多次调用 `synced_out << ...`，直到它析构时才原子地提交所有输出，这在需要连续输出多段信息时非常有用。



### `std::scoped_lock` 到底是什么？和其他锁有何不同？

简单来说，`scoped_lock` 是 C++17 引入的 RAII 锁管理器，专门用于**一次性锁定多个互斥量（mutex）** 的场景。**它是 `lock_guard` 的多锁版本**，内部使用了和 `std::lock` 相同的死锁避免算法，**能确保获取多个锁的操作是“全或无”的，且不会发生死锁**。

为了让你更清晰地理解三者的区别，可以参考下表：

| 类 (Class)                     | 核心用途 (Core Purpose)            | 关键特性 (Key Feature)                                       | 主要限制 (Main Limitation)                 |
| :----------------------------- | :--------------------------------- | :----------------------------------------------------------- | :----------------------------------------- |
| **`std::lock_guard`** (C++11)  | 简单的**单锁** RAII 管理。         | 极致的轻量和简单，构造即锁，析构即解。                       | 功能单一，无法手动解锁或转移所有权。       |
| **`std::scoped_lock`** (C++17) | 无死锁的**多锁** RAII 管理。       | C++17起替代`lock_guard`的首选，可安全地同时锁定多个互斥量。  | 与`lock_guard`类似，功能固定，缺少灵活性。 |
| **`std::unique_lock`** (C++11) | 灵活的、功能全面的 RAII 锁管理器。 | **可手动`lock`/`unlock`**、**可移动**（所有权可转移）、**可与条件变量配合使用**。 | 比前两者稍重，有额外的性能开销。           |

**总结选择原则**：

- **首选 `scoped_lock`**：在C++17及以上的标准中，无论是锁定单个还是多个互斥量，它都是最安全、最现代的默认选择。
- **只在需要以下特性时用 `unique_lock`**：需要配合条件变量（`std::condition_variable`），或者需要在作用域内手动解锁、重新锁定以及转移锁的所有权。
- **`lock_guard` 可视为历史遗留**：如果你的代码需要兼容C++11，或者你确定只需要最轻量的单锁管理，可以使用它。



## std::condition_variable

**条件变量是一种线程间的信号通知机制：让一个线程停下来休眠，直到另一个线程触发某个条件后再唤醒它。**

定义在头部\<[condition_variable](https://www.cppreference.com/cpp/header/condition_variable)>

```c++
class condition_variable;
```

`std::condition_variable`是一个同步原语，与`std::mutex`一起使用，用于阻塞一个或多个线程，直到另一个线程修改共享变量（条件）并通知`std::condition`变量。

打算修改共享变量的线程必须：

1. 获取一个`std::mutex`（通常通过`std::lock_guard`）。
2. 在锁被拥有时修改共享变量。
3. 在`std::condition_variable`上调用`notify_one`或`notify_all`（可以在释放锁后完成）。

即使共享变量是原子变量，也必须在拥有互斥体的同时对其进行修改，以便将修改正确([correctly](https://stackoverflow.com/questions/38147825/))发布到等待的线程。

任何打算等待`std::condition_variable`的线程都必须：

1. 在用于保护共享变量的`mutex`上获取一个`std::unique_lock<std::mutex>`。

2. 执行以下操作之一：

   1. 检查情况，以防已经更新并通知。
   2. 在`std::condition_variable`上调用`wait`、`wait_for`或`wait_until`（原子性地释放`mutex`并暂停线程执行，直到条件变量被通知、超时到期或发生虚假唤醒([spurious wakeup](https://en.wikipedia.org/wiki/Spurious_wakeup))，然后在返回之前原子性地获取互斥体）。
   3. 检查情况，如果不满意，继续等待。

   or

   1. 使用`wait`、`wait_for`和`wait_until`的断言重载，它执行相同的三个步骤。



`std::condition_variable`仅适用于`std::unique_lock＜std::mutex＞`，这允许在某些平台上实现最高效率。`std::condition_variable_any`提供了一个条件变量，该变量适用于任何[*BasicLockable*](https://www.cppreference.com/cpp/named_req/BasicLockable) 对象，例如`std::shared_lock`。

条件变量允许并发调用`wait`、`wait_for`、`wait_until`、`notify_one`和`notify_all`成员函数。

类`std::condition_variable`是一个 [*StandardLayoutType*](https://www.cppreference.com/cpp/named_req/StandardLayoutType)。它不是可复制构造的([*CopyConstructible*](https://www.cppreference.com/cpp/named_req/CopyConstructible))、可移动构造的( [*MoveConstructible*](https://www.cppreference.com/cpp/named_req/MoveConstructible))、可复制赋值的([*CopyAssignable*](https://www.cppreference.com/cpp/named_req/CopyAssignable))或可移动赋值的([*MoveAssignable*](https://www.cppreference.com/cpp/named_req/MoveAssignable))。



### Nested types 嵌套类型

|         Name         |  Definition  |
| :------------------: | :----------: |
| `native_handle_type` | **实现定义** |



### Member functions

| **[(constructor)](https://www.cppreference.com/cpp/thread/condition_variable/condition_variable)** |    **构造对象(公共成员函数)**     |
| :----------------------------------------------------------: | :-------------------------------: |
| **[(destructor)](https://www.cppreference.com/cpp/thread/condition_variable/~condition_variable)** |    **销毁对象(公共成员函数)**     |
|                    **operator=[deleted]**                    | **不可复制和分配 (公共成员函数)** |

#### Notification

| **[notify_one](https://www.cppreference.com/cpp/thread/condition_variable/notify_one)** | **通知一个等待线程 (公共成员函数)**  |
| :----------------------------------------------------------: | :----------------------------------: |
| **[notify_all](https://www.cppreference.com/cpp/thread/condition_variable/notify_all)** | **通知所有等待的线程(公共成员函数)** |

#### Waiting

| **[wait](https://www.cppreference.com/cpp/thread/condition_variable/wait)** |       **阻塞当前线程，直到唤醒条件变量(公共成员函数)**       |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[wait_for](https://www.cppreference.com/cpp/thread/condition_variable/wait_for)** | **阻塞当前线程，直到唤醒条件变量或在指定的超时持续时间之后(公共成员函数)** |
| **[wait_until](https://www.cppreference.com/cpp/thread/condition_variable/wait_until)** | **阻塞当前线程，直到唤醒条件变量或达到指定时间点(公共成员函数)** |

#### Native handle

| [native_handle](https://www.cppreference.com/cpp/thread/condition_variable/native_handle) | 返回本机句柄(公共成员函数) |
| :----------------------------------------------------------: | :------------------------: |



### 示例 

`std::condition_variable`与`std::mutex`结合使用，以促进线程间的通信。

```c++
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <string>

using namespace std;

mutex m;
condition_variable cv;
string a_data;
bool ready = false;
bool processed = false;

void worker_thread() {
	//等待main（）发送数据
	unique_lock lk(m);
	cv.wait(lk, [] {return ready; });

	//等待之后，我们拥有了锁
	cout << "Worker thread is processing data\n";
	a_data += " after processing";

	//将数据发送回main（）
	processed = true;
	cout << "Worker thread signals data processing completed\n";

	//手动解锁在通知之前完成，以避免唤醒等待的线程只是再次阻塞（有关详细信息，请参阅notify_one）
	lk.unlock();
	cv.notify_one();
}

int main() {
	thread worker(worker_thread);
	
	a_data = "Example data";
	//将数据发送到工作线程
	{
		lock_guard lk(m);
		ready = true;
		cout << "main() signals data ready for processing\n";
	}
	cv.notify_one();

	//等待工人
	{
		unique_lock lk(m);
		cv.wait(lk, [] {return processed; });
	}
	cout << "Back in main(), data = " << a_data << '\n';

	worker.join();
}

```



条件变量核心用法：
- `wait`：**当前线程释放锁并阻塞，等待被通知。**带谓词的重载等价于一层 while 循环检查，它能保证返回时锁还被持有且谓词为 true，同时自动处理虚假唤醒。
- `notify_one`：**唤醒一个正在等待的线程。**如果没有任何线程在等待，通知直接丢弃。
- `notify_all`：**唤醒所有正在等待的线程，只有一个能抢到锁，其余继续等待**。

*注意：通知线程应该在释放锁之后再调用 `notify_one`，避免被唤醒的线程立即再次阻塞。*



#### **`cv.wait(lk, pred)` 内部机制**

当调用带谓词的 `wait` 时，它等价于执行了下面的循环：

```c++
while (!ready) {	//这个ready是上面代码里的：cv.wait(lk, [] { return ready; });
    wait(lk);  // 这里的 wait 是关键
}
```

而 `wait(lk)` 这个操作会**原子地**完成三件事：

1. **解锁 `m`**（工作线程不再持有锁）。
2. **让当前线程进入休眠状态**，等待 `notify_one()`。
3. 当被唤醒后，**重新锁住 `m`**，然后函数返回。

所以，**当工作线程进入 `wait` 后，它实际上已经释放了锁**。这一点非常重要：`wait` 在让线程休眠前，必须释放锁，否则别的线程永远无法获取锁去修改等待的条件。



### std::condition_variable::notify_one

```c++
void notify_one() noexcept;
```

如果有任何线程正在等待`*this`，则调用`notify_one`会解锁其中一个等待的线程。

#### Parameters 	 (none)

#### Return value	(none)

#### 注意

 `notify_one()`/`notify_all()`的效果和 `wait()`/`wait_for()`/`wait_until()`的三个原子部分（解锁+等待、唤醒和锁定）中的每一个都以一个总顺序发生，**可以看作是原子变量的修改顺序( [modification order](https://www.cppreference.com/cpp/atomic/memory_order#Modification_order))**：该顺序特定于这个单独的条件变量。例如，这使得`notify_one()`无法延迟并解除阻止在调用`notify_one()`后立即开始等待的线程。

通知线程无需将锁保持在与等待线程相同的互斥体上；事实上，这样做是一种悲观，**因为被通知的线程会立即再次阻塞，等待通知线程释放锁**。然而，一些实现（特别是 `pthreads `的许多实现）认识到了这种情况，并通过将等待线程从条件变量的队列直接转移到`notify`调用中 ` mutex` 的队列来避免这种**“hurry up and wait”**的情况，而不会将其唤醒。

然而，当需要对事件进行精确调度时，在锁下通知可能是必要的，例如，如果满足条件，等待线程将退出程序，导致通知线程的条件变量被破坏。互斥体解锁后但通知前的虚假唤醒将导致对已销毁对象调用通知。



