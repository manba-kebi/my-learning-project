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

`STD::unique_ptr` 是一个智能指针，它拥有（负责）并通过指针管理另一个对象，当 `unique_ptr` 超出作用域时，它会处理该对象。

```c++
template< class T, class Deleter = std::default_delete<T> > class unique_ptr;
template < class T, class Deleter > class unique_ptr<T[], Deleter>;
//如果 T* 不是有效类型（例如 T 是引用类型），那么实例化定义 std::unique_ptr<T， Deleter> 的程序是错误的。
```

当发生以下任一情况时，使用关联的删除器处理该对象：

- 管理 `unique_ptr` 对象被销毁。
- 管理 `unique_ptr` 对象通过运算符= 或 reset（） 被赋予另一个指针。



通过调用 `get_deleter()（ptr）`，使用可能由用户提供的删除器来丢弃该对象。默认的删除器（`std::default_delete`）使用`删除`操作符，它会销毁对象并释放内存。



`unique_ptr` 也可以选择不拥有任何对象，此时称为*空*的。



`unique_ptr` 有两个版本：

- 管理单个对象（例如，分配为`new`对象）。
- 管理动态分配的对象数组（例如，分配为 `new[]` 的对象）。



### 注意

只有非 const `unique_ptr` 才能将被管理对象的所有权转移到其他 `unique_ptr`。如果一个对象的生命周期由 `const std::unique_ptr` 管理，则其寿命受限于指针创建的范围。

`unique_ptr` 通常用于管理对象的生命周期，包括：

- 为处理动态生命周期对象的类和函数提供异常安全，通过保证在正常退出和通过异常退出时都被删除。
- 将具有动态寿命的唯一拥有对象的所有权转移到函数中。
- 通过函数获取具有动态寿命的唯一拥有对象的所有权。
- 作为移动感知容器中的元素类型，如 Std::Vector，这些容器持有指向动态分配对象的指针（例如，如果需要多态行为）。



#### unique_str与shared_ptr的一些区别

（官方文档里放到 注意 这里了，我感觉跟像是一些需要注意的区别)

`unique_ptr `可以用于不完全类型 T 的构造，例如在 `pImpl `模式中作为句柄使用。**若使用默认删除器，则 T 必须在代码中删除器被调用时是完整的类型，这发生在 `unique_ptr` 的析构函数、移动赋值运算符和 `reset` 成员函数中**。（相比之下，`std::shared_ptr `无法从不完全类型的裸指针构造，**但可以在 T 不完整时销毁**）。注意，**若 T 是类模板特化**，则使用 `unique_ptr `作为操作数（例如 !p）时，由于 `ADL` 的要求，**T 的参数必须完整**。



**如果T是基类B的派生类，则unique_ptr<T>可隐式转换为unique_ptr<B>**。所得`unique_ptr<B>`的默认删除器将使用B的`operator delete`，**除非B的析构函数是虚函数，否则会导致未定义行为**。需注意的是，**std::shared_ptr的行为有所不同：std::shared_ptr<B>将使用T类型的operator delete，即使B的析构函数不是虚函数，被管理对象也能正确删除**。



与`std::shared_ptr`不同，`unique_ptr`可以通过满足`NullablePointer`的任何自定义句柄类型来管理对象。例如，这允许通过提供定义`typedef boost::offset_ptr`指针的`Deleter`来管理位于共享内存中的对象；或者另一个花哨的指针。



### 嵌套类型

|     类型     |                             定义                             |
| :----------: | :----------------------------------------------------------: |
|   pointer    | `std::remove_reference<Deleter>::typepointer`（如果存在该类型），否则 T`*`。必须满足 [*NullablePointer*](https://www.cppreference.com/cpp/named_req/NullablePointer) |
| element_type |           `T`，即该 `unique_ptr` 所管理对象的类型            |
| deleter_type | `Deleter`，函数对象或 lvalue，指向函数或函数对象，需从解构器调用 |



### 成员函数

| [(constructor) ](https://www.cppreference.com/cpp/memory/unique_ptr/unique_ptr) |    构造一个新的unique_ptr    |
| :----------------------------------------------------------: | :--------------------------: |
| [(destructor)](https://www.cppreference.com/cpp/memory/unique_ptr/~unique_ptr) | **销毁管理对象（如果存在）** |
| [operator=](https://www.cppreference.com/cpp/memory/unique_ptr/operator%3D) |    **分配 `unique_ptr`**     |



#### 修饰符

| [release](https://www.cppreference.com/cpp/memory/unique_ptr/release) | 返回指向被管理对象的指针并释放所有权 |
| :----------------------------------------------------------: | :----------------------------------: |
| [reset](https://www.cppreference.com/cpp/memory/unique_ptr/reset) |         **替换了被管理对象**         |
| [swap](https://www.cppreference.com/cpp/memory/unique_ptr/swap) |          **交换被管理对象**          |



#### 观察者

| [get](https://www.cppreference.com/cpp/memory/unique_ptr/get) |       返回指向托管对象的指针       |
| :----------------------------------------------------------: | :--------------------------------: |
| [get_deleter](https://www.cppreference.com/cpp/memory/unique_ptr/get_deleter) | **返回用于销毁被管理对象的删除器** |
| [operator bool](https://www.cppreference.com/cpp/memory/unique_ptr/operator_bool) |  **检查是否存在关联的受管理对象**  |



#### 单对象版本，unique_ptr\<T>

| [operator*](https://www.cppreference.com/cpp/memory/unique_ptr/operator*)<br />[operator->](https://www.cppreference.com/cpp/memory/unique_ptr/operator*) | 解引用指向托管对象的指针 |
| :----------------------------------------------------------: | :----------------------: |



#### 数组版本，unique_ptr<T[]>

| [operator[\]](https://www.cppreference.com/cpp/memory/unique_ptr/operator_at) | 提供对受管理数组的索引访问 |
| :----------------------------------------------------------: | :------------------------: |



### 非成员函数

| [make_unique<br />make_unique_for_overwrite](https://www.cppreference.com/cpp/memory/unique_ptr/make_unique) |       创建一个管理新对象的唯一指针        |
| :----------------------------------------------------------: | :---------------------------------------: |
| [operator== <br />operator!= <br />operator< <br />operator<=<br />operator><br />operator>= <br />operator<=>](https://www.cppreference.com/cpp/memory/unique_ptr/operator_cmp) | 与另一个 `unique_ptr` 或与 `nullptr` 比较 |
| [operator<<(std::unique_ptr)](https://www.cppreference.com/cpp/memory/unique_ptr/operator_ltlt) |         输出管理指针到输出流的值          |
| [std::swap](https://www.cppreference.com/cpp/memory/unique_ptr/swap2) |           专注于 std::交换算法            |



#### 辅助类

| [std::hash](https://www.cppreference.com/cpp/memory/unique_ptr/hash) | std::unique_ptr的哈希支持 |
| :----------------------------------------------------------: | :-----------------------: |



### unique_ptr的例子

```C++
#include <iostream>
#include <memory>
#include <cassert>		//提供 断言宏 assert。
#include <cstdio>
#include <fstream>
#include <locale>		//提供 本地化（locale）支持。
#include <stdexcept>	//定义了一些标准异常类

//template< class T, class Deleter = std::default_delete<T> > class unique_ptr;
//template < class T, class Deleter > class unique_ptr<T[], Deleter>;
//如果 T* 不是有效类型（例如 T 是引用类型），那么实例化定义 std::unique_ptr<T， Deleter> 的程序是错误的。

//下面演示运行时多态性的辅助类
struct B {
	virtual ~B() = default;

	virtual void bar() { std::cout << "B::bar\n"; }
};

struct D :B {
	D() { std::cout << "D::D\n"; }
	~D() { std::cout << "D::~D\n"; }

	void bar() override { std::cout << "D::bar\n"; }
};

//使用unique_ptr的函数可以通过值或右值引用来获取它
std::unique_ptr<D> pass_through(std::unique_ptr<D> p) {
	p->bar();
	return p;
}

//下面自定义删除器演示的辅助函数
void close_file(std::FILE* fp) {
	std::fclose(fp);
}

//基于unique_ptr的链表演示
struct List {
	struct Node {
		int data;
		std::unique_ptr<Node> next;
	};

	std::unique_ptr<Node> head;

	~List() {
		//在循环中顺序销毁列表节点，默认析构函数将递归调用其“next”的析构函数，这将导致足够大的列表发生堆栈溢出。
		while (head) {
			auto next = std::move(head->next);
			head = std::move(next);
		}
	}

	void push(int data) {
		head = std::unique_ptr<Node>(new Node{ data,std::move(head) });
	}
};

int main() {
	std::cout << "1) Unique所有权语义演示\n";
	{
		//创建（唯一拥有的）资源
		std::unique_ptr<D> p = std::make_unique<D>();

		//将所有权转移到“pass_through”，这反过来又通过返回值将所有权转移回来
		std::unique_ptr<D> q = pass_through(std::move(p));

		//“p”现在处于从“空”状态移动的状态，等于nullptr
		assert(!p);
	}

	std::cout << "\n""2)运行时多态性演示\n";
	{
		//创建派生资源并通过基类型指向它
		std::unique_ptr<B> p = std::make_unique<D>();

		//动态调度按预期工作
		p->bar();
	}

	std::cout << "\n""3) 自定义删除器演示\n";
	std::ofstream("demo.txt") << 'x';	//准备要读取的文件
	{
		using unique_file_t = std::unique_ptr<std::FILE, decltype(&close_file)>;
		//unique_file_t 就是：一个 unique_ptr，它管理 std::FILE* 资源，并使用函数指针作为删除器。
		unique_file_t fp(std::fopen("demo.txt", "r"), &close_file);		//打开文件，返回 FILE* 指针
		if (fp)
			std::cout << char(std::fgetc(fp.get())) << "\n";			//从文件读取一个字符。
	}// 此处调用“close_file（）”（如果“fp”不为null）

	std::cout << "\n""4)自定义lambda表达式删除器和异常安全演示 \n";
	try {		
        //尖括号 < > 里的东西：模板参数
        //定义类型，第一个模板参数 D：表示这个 unique_ptr 管理的是 D 类型的对象。
        //第二个模板参数 void(*)(D*)：表示这个 unique_ptr 使用的删除器类型是“一个参数为 D*、返回 void 的函数指针”。
        
        //圆括号 ( ) 里的东西：构造函数的参数
        //第一个参数 new D：一个原始指针，交给 unique_ptr 管理。
		//第二个参数 [](D* ptr){...}：一个具体的删除器（这里是个 lambda，它正好能转成上面模板参数要求的函数指针类型）。
        
        //第二个模板参数 默认是 std::default_delete<T>（一个函数对象）。
        //构造函数中，删除器参数也 默认是 std::default_delete<T>()。
        //所以省略 = 使用标准库提供的默认删除器。可以省略，而且绝大多数情况下你都应该省略。
        
		std::unique_ptr<D, void(*)(D*)> p(new D, [](D* ptr) {
			std::cout << "从自定义删除器中删除...\n";
			delete ptr;
			});

		throw std::runtime_error(""); //如果“p”是一个普通的指针，它就会在这里泄漏
	}
	catch (const std::exception&) {
		std::cout << "Caught exception\n";
	}

	std::cout << "\n""5)unique_ptr演示的数组形式\n";
	{
		std::unique_ptr<D[]> p(new D[3]);
	}//“D::~D()”被调用3次

	std::cout << "\n""6)链接列表演示\n";
	{
		List wall;
		const int enough{ 1'000'000 };
		for (int beer = 0; beer != enough; ++beer)
			wall.push(beer);
		std::cout.imbue(std::locale("en_US.UTF-8"));		//imbue 是一个成员函数，用于将某个 locale 应用到流，此后该流将按照这个 locale 的规则进行输入/输出格式化。
		std::cout << "墙上挂着"<< enough <<"瓶啤酒...\n";
	}//毁掉所有的啤酒
}
```



#### 删除器省略与不省略的意义

##### 省略（使用默认删除器）的意义

- **简单、安全、无额外开销**。对于绝大多数堆上分配的对象，这就是你需要的。
- 不需要自己写任何删除逻辑，编译器生成的代码和手写 `delete` 一样高效。



##### 不省略（自定义删除器）的意义

你需要自定义删除器的典型场景：

| 场景                               | 例子                                                         |
| :--------------------------------- | :----------------------------------------------------------- |
| **管理 C 风格资源**                | `FILE*` 需要 `fclose`，而不是 `delete`                       |
| **管理操作系统句柄**               | `HANDLE` 需要 `CloseHandle`，`HWND` 需要 `DestroyWindow`     |
| **特殊释放逻辑**                   | 例如需要先记录日志再释放，或者需要从某个全局容器中移除指针   |
| **数组形式但需要单独处理每个元素** | 默认 `delete[]` 已经够用，但如果你需要调用特定的清理函数（如 `CoTaskMemFree`） |
| **不释放内存，只做其他清理**       | 比如你使用内存池，不需要 `delete`，只需将指针归还池子        |

**示例**：管理 `FILE*` 时，你**不能**省略删除器，因为默认删除器会调用 `delete fp`，而 `FILE*` 不是 `new` 出来的，行为未定义。所以必须提供自定义删除器（如 `close_file`）。



## shared_ptr

定义在头部 `<memory>`

| `template< class T > class shared_ptr; ` |
| ---------------------------------------- |

`STD::shared_ptr` 是一个智能指针，通过指针保留对对象的共享所有权。多个 `shared_ptr` 对象可能拥有同一个对象。当发生以下任一情况时，该对象被销毁并释放其内存：

- 拥有该对象的最后一个剩余`shared_ptr`被销毁
- 最后一个拥有该对象的 `shared_ptr` 通过 `operator=` 或 `reset（）` 被赋予另一个指针。

使用[delete表达式](https://www.cppreference.com/cpp/language/delete)或在构造过程中提供给`shared_ptr`的自定义删除器销毁对象。

`shared_ptr`**可以共享对象的所有权**，**同时存储指向另一个对象的指针**。此功能可用于指向成员对象，同时拥有它们所属的对象。存储的指针是由get（）、解引用和比较运算符访问的指针。**被管理指针是在使用计数为零时传递给删除器的指针**。

一个 `shared_ptr` 也可能没有任何对象，这时称为*空* （如果使用别名构造函数创建空的 `shared_ptr`，则可能有非空的存储指针）。



所有成员函数（包括复制构造函数和复制赋值）都可以被不同`shared_ptr`对象上的多个线程调用，而无需额外的同步，即使这些对象是同一对象的副本和共享所有权。如果多个执行线程在没有同步的情况下访问同一个`shared_ptr`对象，并且这些访问中的任何一个使用了`shared_ptr`的非const成员函数，那么将发生数据竞争；[ `std::atomic<shared_ptr>` ](https://www.cppreference.com/cpp/memory/shared_ptr/atomic2)可用于防止数据竞争。



### 成员类型

|   成员类型   |              定义               |
| :----------: | :-----------------------------: |
| element_type | T<br />std::remove_extent_t\<T> |
|  weak_type   |        std::weak_ptr\<T>        |



### 成员函数

| [(constructor)](https://www.cppreference.com/cpp/memory/shared_ptr/shared_ptr) |                     构造新 `shared_ptr`                     |
| :----------------------------------------------------------: | :---------------------------------------------------------: |
| [**(destructor)** ](https://www.cppreference.com/cpp/memory/shared_ptr/~shared_ptr) | **如果没有更多`shared_ptr`s链接到拥有的对象，则销毁该对象** |
| **[operator=](https://www.cppreference.com/cpp/memory/shared_ptr/operator%3D)** |                     **分配shared_ptr**                      |



#### 修饰符

|  reset   |  替换了被管理对象  |
| :------: | :----------------: |
| **swap** | **交换被管理对象** |



#### 观察者

|              get              |                   返回存储的指针                   |
| :---------------------------: | :------------------------------------------------: |
| **operator*<br />operator->** |                **去引用存储的指针**                |
|        **operator[]**         |            **提供对存储数组的索引访问**            |
|         **use_count**         |  **返回指向同一被管理对象的`shared_ptr`对象数量**  |
|          **unique**           | **检查被管理对象是否仅由当前`shared_ptr`对象管理** |
|       **operator bool**       |            **检查存储指针是否为空指针**            |
|       **owner_before**        |          **提供基于所有者的共享指针排序**          |
|        **owner_hash**         |          **提供基于所有者的共享指针哈希**          |
|        **owner_equal**        |       **提供基于所有者的共享指针的平等比较**       |



### 非成员函数

| **[make_shared<br />make_shared_for_overwrite](https://www.cppreference.com/cpp/memory/shared_ptr/make_shared)** |                 **创建管理新对象的共享指针**                 |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| **[allocate_shared<br />allocate_shared_for_overwrite](https://www.cppreference.com/cpp/memory/shared_ptr/allocate_shared)** |     **创建一个共享指针，用于管理使用分配器分配的新对象**     |
| **[static_pointer_cast<br />dynamic_pointer_cast<br />const_pointer_cast<br />reinterpret_pointer_cast](https://www.cppreference.com/cpp/memory/shared_ptr/pointer_cast)** | **将 [`static_cast`](https://www.cppreference.com/cpp/language/static_cast), [`dynamic_cast`](https://www.cppreference.com/cpp/language/dynamic_cast), [`const_cast`](https://www.cppreference.com/cpp/language/const_cast), or [`reinterpret_cast`](https://www.cppreference.com/cpp/language/reinterpret_cast)应用于存储的指针** |
| **[get_deleter](https://www.cppreference.com/cpp/memory/shared_ptr/get_deleter)** |             **如果拥有，则返回指定类型的删除器**             |
| **[operator==<br />operator!=<br />operator<<br />operator<=<br />operator><br />operator>=<br />operator<=>](https://www.cppreference.com/cpp/memory/shared_ptr/operator_cmp)** |          **与另一个shared_ptr或与nullptr进行比较**           |
| **[operator<<](https://www.cppreference.com/cpp/memory/shared_ptr/operator_ltlt)** |               **将存储的指针的值输出到输出流**               |
| **[std::swap](https://www.cppreference.com/cpp/memory/shared_ptr/swap2)** |                  **专门研究std::swap算法**                   |
| **[std::atomic_is_lock_free<br />std::atomic_load<br />std::atomic_load_explicit<br />std::atomic_store<br />std::atomic_store_explicit<br />std::atomic_exchange<br />std::atomic_exchange_explicit<br />std::atomic_compare_exchange_weak<br />std::atomic_compare_exchange_strong<br />std::atomic_compare_exchange_weak_explicit<br />std::atomic_compare_exchange_strong_explicit](https://www.cppreference.com/cpp/memory/shared_ptr/atomic)** |            **专门用于std::shared_ptr的原子操作**             |



#### 辅助类

| **[std::atomic](https://www.cppreference.com/cpp/memory/shared_ptr/atomic2)** |        **原子共享指针**        |
| :----------------------------------------------------------: | :----------------------------: |
| **[std::hash](https://www.cppreference.com/cpp/memory/shared_ptr/hash)** | **哈希支持 `std::shared_ptr`** |



### 注意

- 一个对象的所有权只能通过复制构造或复制其价值赋予另一个 `shared_ptr` 来与另一个 `shared_ptr` 共享。使用另一个 `shared_ptr` 拥有的原始底层指针构建新 `shared_ptr` 会导致未定义的行为。

- `std::shared_ptr`可以与不完整类型T一起使用。但是，来自原始指针（**template**<**class** **Y**> shared_ptr(Y)）的构造函数和**template**<**class** **Y**> void reset(Y*)成员函数只能用指向完整类型的指针调用（注意`std::unique_ptr`可以从指向不完整类型的原始指针构造）。

- `std::shared_ptr<T>`中的 T 可能是一个函数类型：在这种情况下，它管理一个指向函数的指针，而不是一个对象指针。**这有时用于在引用动态库或插件的任何函数时保持其加载状态**：

  - ```c++
    void del(void(*)()) {}
    
    void fun() {}
    
    int main()
    {
        std::shared_ptr<void()> ee(fun, del);
        (*ee)();
    }
    ```



### 实现说明

在典型实现中，`shared_ptr` 只包含两个指针：

- 存储的指针（get（）返回的指针）；
- 指向控制块的指针。

控制块是一个动态分配的对象，包含：

- 要么指向被管理对象的指针，要么指向被管理对象本身;
- 删除器（类型已擦除）；
- 分配器（类型已擦除）;
- 拥有托管对象的`shared_ptr`的数量；（托管就是被管理的意思）
- 引用托管对象的`weak_ptr`的数量。



当通过**调用`std::make_shared`或`std::allocate_shared`**创建`shared_ptr`时，**控制块和托管对象的内存都是通过单个分配创建的**。托管对象在控制块的数据成员中就地构造。当通过**`shared_ptr`构造函数之一**创建shared_ptr时，**托管对象和控制块必须分开分配**。在这种情况下，控制块存储指向托管对象的指针。



**`shared_ptr`直接持有的指针是`get（）`返回的指针**，而**控制块持有的指针/对象是当共享所有者数量达到零时将被删除的指针**。这些指针不一定相等。



`shared_ptr`的**析构函数会减少控制块的共享所有者数量**。**如果该计数器达到零，控制块将调用托管对象的析构函数**。在`std::weak_ptr`计数器也达到零之前，控制块不会自行释放。



在现有的实现中，如果存在指向同一控制块的共享指针，则弱指针的数量会递增.



为了满足线程安全要求，引用计数器通常使用等效的`std::atomic:：fetch_add`与`std::memory_order_relaxed`来递增（递减需要更强的排序来安全地销毁控制块）。



### 示例1

```C++
#include <chrono>		//<chrono> 是 C++ 标准库中专门处理时间和时钟的库
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;//本质上是个语法糖，让你不用写 std::chrono::milliseconds(987) 这种冗长的写法

struct Base {
	Base() { std::cout << "Base::Base()\n"; }

	//注意：此处非虚析构函数无妨
	~Base() { std::cout << "Base::~Base()\n"; }
};

struct Derived :public Base {
	Derived() { std::cout << "Derived::Derived()\n"; }

	~Derived() { std::cout << "Derived::~Derved()\n"; }
};

template <typename T>
void print(T rem, std::shared_ptr<Base> const& sp)  {
	std::cout << rem << "\n\tget() = " << sp.get()
		<< ",use_count() = " << sp.use_count() << '\n';
}

void thr(std::shared_ptr<Base> p) {
	std::this_thread::sleep_for(987ms);
	std::shared_ptr<Base> lp = p;	//线程安全，即使共享use_count递增
	{
		static std::mutex io_mutex;
		std::lock_guard<std::mutex> lk(io_mutex);
		print("线程中的本地指针：", lp);
	}
}

int main() {
	std::shared_ptr<Base> p = std::make_shared<Derived>();

	print("创建了一个共享的Derived（作为指向Base的指针）", p);

	std::thread t1{ thr,p }, t2{ thr,p }, t3{ thr,p };	//这行是在创建三个 std::thread 线程对象，每个线程都要执行 thr 函数。
	p.reset();	//从main中释放所有权，它会让 p 不再指向原来的对象；

	print("3个线程之间共享所有权，并从main释放所有权：", p);

    //join()是主线程等待子线程结束的阻塞操作
	t1.join();		//如果不用 join()，主线程走到 main 结尾就直接 return 0 了，整个程序就结束了。那么那些还在 sleep 或者正在打印的子线程会被操作系统强制杀死，可能导致资源没释放、析构函数没调用，甚至程序崩溃。
	t2.join();
	t3.join();

	std::cout << "所有线程均已完成，最后一个线程已删除。\n";
}
```



#### `lp = p` 下面的那个单独的中括号 `{}` 是什么？

这个单独的 `{}` 不是循环也不是函数，它是 C++ 里的**局部作用域（代码块）**，用来限制变量的生命周期。

看这段代码：

```c++
{
    static std::mutex io_mutex;
    std::lock_guard<std::mutex> lk(io_mutex);
    print("线程中的本地指针：", lp);
}
```

它的作用是：

- 让 `io_mutex` 和 `lk` 这两个变量，**只在这个 `{}` 里面有效**；
- 当程序执行到 `}` 时，`lk` 这个 `std::lock_guard` 会自动析构，**自动释放锁**，保证互斥锁一定会被解开，避免死锁。

这是 C++ 里很常用的写法，用来控制 RAII 对象（比如锁、智能指针）的生命周期。



#### `std::thread t1{ thr,p }, t2{ thr,p }, t3{ thr,p };` 里 `thr` 的用法为啥不像普通函数？

这行是在创建三个 `std::thread` 线程对象，每个线程都要执行 `thr` 函数。

你可以把 `std::thread` 的构造函数理解成：

```c++
std::thread 线程名{ 要执行的函数, 函数的参数1, 参数2, ... };
```

所以 `t1{ thr, p }` 就是：

- 创建线程 `t1`；
- 让线程 `t1` 去执行 `thr` 这个函数；
- 把 `p` 作为参数传给 `thr`。

它和普通函数调用 `thr(p)` 不一样，是因为 `std::thread` 是在**后台异步执行**这个函数，而不是主线程直接调用。主线程创建完线程就继续往下跑了，不会等 `thr` 执行完。



### 示例二

```c++
#include <iostream>
#include <memory>

struct MyObj {
	MyObj() { std::cout << "MyObj constructed\n"; }

	~MyObj() { std::cout << "MyObj destructed\n"; }
};

//Container 是一个外壳，里面可以放一个 MyObj 对象，并且它可以返回一个“绑定了外壳生命周期的 MyObj 指针”，确保只要别人在用那个指针，外壳就不会被提前销毁。
struct Container :std::enable_shared_from_this<Container>	//注意：public 继承
{
	std::shared_ptr<MyObj> memberObj;
	void CreateMember() { memberObj = std::make_shared<MyObj>(); }

	std::shared_ptr<MyObj> GetAsMyObj() {
		//为成员使用别名shared ptr
		return std::shared_ptr<MyObj>(shared_from_this(), memberObj.get());
	}
};

#define COUT(str) std::cout<<"\n"<<str<<"\n"
#define DEMO(...) std::cout<<#__VA_ARGS__<<" = "<<__VA_ARGS__<<"\n"

int main() {
	COUT("创建共享容器");
	std::shared_ptr<Container> cont = std::make_shared<Container>();
	DEMO(cont.use_count());
	DEMO(cont->memberObj.use_count());

	COUT("创建成员");
	cont->CreateMember();
	DEMO(cont.use_count());
	DEMO(cont->memberObj.use_count());

	COUT("创建另一个共享容器");
	std::shared_ptr<Container> cont2 = cont;
	DEMO(cont.use_count());
	DEMO(cont->memberObj.use_count());
	DEMO(cont2.use_count());
	DEMO(cont2->memberObj.use_count());

	COUT("GetAsMyObj");
	std::shared_ptr<MyObj> myobj1 = cont->GetAsMyObj();
	DEMO(myobj1.use_count());
	DEMO(cont.use_count());
	DEMO(cont->memberObj.use_count());
	DEMO(cont2.use_count());
	DEMO(cont2->memberObj.use_count());

	COUT("复制别名obj");
	std::shared_ptr<MyObj>myobj2 = myobj1;
	DEMO(myobj1.use_count());
	DEMO(myobj2.use_count());
	DEMO(cont.use_count());
	DEMO(cont->memberObj.use_count());
	DEMO(cont2.use_count());
	DEMO(cont2->memberObj.use_count());

	COUT("Restting cont2");
	cont2.reset();
	DEMO(myobj1.use_count());
	DEMO(myobj2.use_count());
	DEMO(cont.use_count());
	DEMO(cont->memberObj.use_count());

	COUT("Resetting myobj2");
	myobj2.reset();
	DEMO(myobj1.use_count());
	DEMO(cont.use_count());
	DEMO(cont->memberObj.use_count());

	COUT("Resetting cont");
	cont.reset();
	DEMO(myobj1.use_count());
	DEMO(cont.use_count());
}
```



#### `__VA_ARGS__` 是什么？

- 代表宏参数中 `...` 所匹配的**所有实际参数**（可以是一个、多个，用逗号分隔）。
- 简单说：`...` 是“占位符”，`__VA_ARGS__` 就是“实际传进来的那堆东西”。

#### `#__VA_ARGS__` 是什么？

- `#` 是**字符串化运算符**（stringification），把参数原样转换成字符串字面量。
- `#__VA_ARGS__` 就是把整个参数列表（包括逗号和空格）变成一个字符串。

##### 举例：

```c++
DEMO(cont.use_count())
```

展开后变成：

```c++
std::cout << "cont.use_count()" << " = " << cont.use_count() << "\n";
```

- `#__VA_ARGS__` 变成了 `"cont.use_count()"`（字符串）
- `__VA_ARGS__` 变成了 `cont.use_count()`（表达式，会被求值）



##  weak_ptr

Defined in header `<memory>`

```c++
template< class T > class weak_ptr;
```

`STD::weak_ptr` 是一个智能指针，**它保留一个非拥有（“弱”）的引用**，指向由 `std::shared_ptr `管理的对象。**必须将其转换为 `std::shared_ptr` 才能访问被引用对象。**

`std::weak_ptr`模型临时所有权：**当一个对象只有在存在的情况下才需要访问，并且可能随时被其他人删除时，`std::weak-ptr`用于跟踪该对象，并将其转换为`std::shared_ptr`以获得临时所有权**。如果此时原始的`std::shared_ptr`被销毁，则对象的生存期将延长，直到临时的`std::shared_ptr`也被销毁。

**`std::weak_ptr`的另一个用途是打破由`std::shared_ptr`管理的对象形成的引用循环。**如果这样的循环是孤立的（即没有外部共享指针进入循环），`shared_ptr`引用计数不能达到零，内存就会泄漏。为了防止这种情况，可以使循环中的一个指针[变弱 ](https://www.cppreference.com/cpp/memory/weak_ptr/~weak_ptr#Example)。



### 成员类型

| element_type | T<br />std::remove_extent_t\<T> |
| :----------: | :-----------------------------: |



### 成员函数

| [(constructor)](https://www.cppreference.com/cpp/memory/weak_ptr/weak_ptr) | 创造了新的 `weak_ptr` |
| :----------------------------------------------------------: | :-------------------: |
| **[(destructor) ](https://www.cppreference.com/cpp/memory/weak_ptr/~weak_ptr)** |  **摧毁 `weak_ptr`**  |
| **[operator=](https://www.cppreference.com/cpp/memory/weak_ptr/operator%3D)** |   **指定weak_ptr**    |



#### 修饰符

| [reset](https://www.cppreference.com/cpp/memory/weak_ptr/reset) | 释放对被管理对象的所有权 |
| :----------------------------------------------------------: | :----------------------: |
| **[swap](https://www.cppreference.com/cpp/memory/weak_ptr/swap)** |    **交换被管理对象**    |



#### 观察者

| **[use_count](https://www.cppreference.com/cpp/memory/weak_ptr/use_count)** | **返回管理该对象的 `shared_ptr` 对象数量** |
| :----------------------------------------------------------: | :----------------------------------------: |
| **[expired](https://www.cppreference.com/cpp/memory/weak_ptr/expired)** |       **检查被引用对象是否已被删除**       |
| **[lock](https://www.cppreference.com/cpp/memory/weak_ptr/lock)** |  **创建一个`shared_ptr`来管理引用的对象**  |
| **[owner_before](https://www.cppreference.com/cpp/memory/weak_ptr/owner_before)** |       **提供基于所有者的弱指针排序**       |
| **[owner_hash](https://www.cppreference.com/cpp/memory/weak_ptr/owner_hash)** |       **提供基于所有者的弱指针哈希**       |
| **[owner_equal](https://www.cppreference.com/cpp/memory/weak_ptr/owner_equal)** |     **提供基于所有者的弱指针相等比较**     |



### 非成员函数

| [std::swap](https://www.cppreference.com/cpp/memory/weak_ptr/swap2) | 专门研究`std::swap`算法 |
| :----------------------------------------------------------: | :---------------------: |



#### 辅助类

| [std::atomic](https://www.cppreference.com/cpp/memory/weak_ptr/atomic2) | 原子弱指针 |
| :----------------------------------------------------------: | :--------: |



### 注意

和 `std::shared_ptr` 类似，`weak_ptr` 的典型实现会存储两个指针：

- 指向控制块的指针；和
- 构造它的`shared_ptr`的存储指针。

**一个单独的存储指针是必要的，以确保将`shared_ptr`转换为`weak_ptr`并返回正确工作，即使是对于混叠的`shared_ptrs`也是如此**。如果不将其锁定到`shared_ptr`中，就不可能访问`weak_ptr`中存储的指针。





# **额外学到的一些相关内容**

## **Value categories 价值类别**

**每个 C++ [表达式 ](https://www.cppreference.com/cpp/language/expressions)（一个运算符及其操作数、文字、变量名等）具有两个独立的属性：*[ 类型](https://www.cppreference.com/cpp/language/type)*和*值类别* 。每个表达式都有某种非指称类型，且每个表达式恰好属于三个主要值类别中的一个：[*prvalue*](https://www.cppreference.com/cpp/language/value_category#prvalue)、[*xvalue*](https://www.cppreference.com/cpp/language/value_category#xvalue) 和 [*lvalue*](https://www.cppreference.com/cpp/language/value_category#lvalue)。**

- **[glvalue](https://www.cppreference.com/cpp/language/value_category#glvalue)（“广义”lvalue）是一种表达式，其求值用来确定对象或函数的身份;**
- **[prvalue ](https://www.cppreference.com/cpp/language/value_category#prvalue)（“纯”rvalue）是一种其值的表达式**
- - **计算内置算子操作数的值（该 PRvalue无*结果对象* ），或**
  - **初始化一个对象（该 prvalue 称为具有*结果对象*，结果对象可以是变量、由[新表达](https://www.cppreference.com/cpp/language/new)式创建的对象、由[临时物质化](https://www.cppreference.com/cpp/language/implicit_conversion#Temporary_materialization)创建的临时对象，或其成员。注意，`non-void` [discard](https://www.cppreference.com/cpp/language/expressions#Discarded-value_expressions)表达式有一个结果对象（即物质化的临时表达式）。此外，每个类和数组的 prvalue 都有结果对象，除非它是 [`decltype`](https://www.cppreference.com/cpp/language/decltype) 的操作数; ）。**
- **[xvalue ](https://www.cppreference.com/cpp/language/value_category#xvalue)（“eXpiring”值）是一个 glvalue，表示资源可重复利用的对象;**
- **[lvalue](https://www.cppreference.com/cpp/language/value_category#lvalue) 是指不是 xvalue的 glvalue;**
- **[rvalue](https://www.cppreference.com/cpp/language/value_category#rvalue) 是 prvalue 或 xvalue;**



``` c++
#include <type_traits>
#include <utility>

template <class T> struct is_prvalue : std::true_type {};
template <class T> struct is_prvalue<T&> : std::false_type {};
template <class T> struct is_prvalue<T&&> : std::false_type {};

template <class T> struct is_lvalue : std::false_type {};
template <class T> struct is_lvalue<T&> : std::true_type {};
template <class T> struct is_lvalue<T&&> : std::false_type {};

template <class T> struct is_xvalue : std::false_type {};
template <class T> struct is_xvalue<T&> : std::false_type {};
template <class T> struct is_xvalue<T&&> : std::true_type {};

int main()
{
    int a{42};
    int& b{a};
    int&& r{std::move(a)};

    // Expression `42` is prvalue
    static_assert(is_prvalue<decltype((42))>::value);

    // Expression `a` is lvalue
    static_assert(is_lvalue<decltype((a))>::value);

    // Expression `b` is lvalue
    static_assert(is_lvalue<decltype((b))>::value);

    // Expression `std::move(a)` is xvalue
    static_assert(is_xvalue<decltype((std::move(a)))>::value);

    // Type of variable `r` is rvalue reference
    static_assert(std::is_rvalue_reference<decltype(r)>::value);	//判断 T 是不是 右值引用类型

    // Type of variable `b` is lvalue reference
    static_assert(std::is_lvalue_reference<decltype(b)>::value);	//判断 T 是不是 左值引用类型

    // Expression `r` is lvalue
    static_assert(is_lvalue<decltype((r))>::value);
}
```

- **`<type_traits>`：C++11 引入的类型特性（type traits）库。**
  **它提供了一系列模板，可以在编译期检查和操作类型的信息。**
  **比如：`std::is_same<T, U>` 判断两个类型是否相同，`std::is_rvalue_reference<T>` 判断 `T` 是不是右值引用类型。**
  **什么时候用：当你需要在编译时根据类型做决策（比如模板编程、SFINAE、`static_assert` 检查）时就会用到。**

- **`<utility>`：实用工具库。**
  **里面包含了很多基础工具，比如 `std::move`、`std::forward`、`std::pair`、`std::integer_sequence` 等。**
  **什么时候用：当你需要移动语义、完美转发、或者使用 `std::pair` 等工具时。**

- **代码中自定义了三个模板：**

  **cpp**

  ```c++
  template <class T> struct is_prvalue : std::true_type {};
  template <class T> struct is_prvalue<T&> : std::false_type {};
  template <class T> struct is_prvalue<T&&> : std::false_type {};
  ```

  **这是什么意思？**

  - **对于大部分类型 `T`，`is_prvalue<T>` 继承自 `std::true_type`，表示“是 prvalue”。**
  - **但是对于 `T&`（左值引用）和 `T&&`（右值引用）这两个特化版本，它们继承自 `std::false_type`，表示“不是 prvalue”。**

  **为什么要这样做？**
  **因为 C++ 中，表达式的值类别可以通过它的decltype 获得的类型来区分：**

  - **prvalue 表达式（比如字面量 `42`）在 `decltype((...))` 下的类型是非引用类型（例如 `int`）。**
  - **lvalue 表达式（比如变量名 `a`）会得到 `int&` 类型。**
  - **xvalue 表达式（比如 `std::move(a)`）会得到 `int&&` 类型。**

  **所以代码利用这个规律，通过检查 `decltype((expr))` 得到的类型是 `T`、`T&` 还是 `T&&`，来判断表达式的值类别。**
  **`std::true_type` 和 `std::false_type` 就是用来在编译期“标记”判断结果的。最终可以用 `::value` 取出布尔值，比如 `is_prvalue<decltype((42))>::value` 就是 `true`。**

  > **小提示：`std::true_type` 和 `std::false_type` 是类型，而 `{}` 是创建它们的空对象。这里用作模板的默认继承，不实例化对象也可以，但是它们通常配合 `std::integral_constant` 使用。**

- **decltype是干什么的？**

  **`decltype(实体)` 的作用是：获取这个实体的声明类型。**
  **但是它的行为取决于你给的是 变量名 还是 带括号的表达式。**

  ### **规则 1：`decltype(变量名)` —— 得到变量声明时的类型**

  ```c++
  int a = 42;
  int& b = a;
  int&& r = std::move(a);
  
  decltype(a) t1;   // int
  decltype(b) t2;   // int&
  decltype(r) t3;   // int&&
  ```

  ### **规则 2：`decltype((表达式))` —— 得到表达式的类型（根据值类别决定）**

  | **表达式的值类别** | **`decltype((...))` 的结果** |
  | :----------------- | :--------------------------- |
  | **lvalue**         | **`T&`（左值引用）**         |
  | **xvalue**         | **`T&&`（右值引用）**        |
  | **prvalue**        | **`T`（非引用）**            |

  > **注意：这里的 `T` 是指该表达式的非引用类型（即去掉引用后的类型）。**

  **总结**

  - **`decltype(变量名)`：直接看变量定义的类型。**
  - **`decltype((变量名))`：看变量作为表达式时的值类别（左值 → 左值引用，右值 → 右值引用，纯右值 → 非引用）。**
  - **`is_lvalue_reference`：判断一个类型是不是 `T&`，这里的 `reference` 就是“引用”的意思。**

  

| **概念（英文）** | **中文译名** | **身份（identity）** | **可移动（Movable）** |                        **大概的理解**                        |
| :--------------: | :----------: | :------------------: | :-------------------: | :----------------------------------------------------------: |
|  **`glvalue`**   |  **泛左值**  |        **有**        |      **不一定**       | **泛指所有“有身份”的表达式，是 `lvalue` 和 `xvalue` 的统称** |
|   **`lvalue`**   |   **左值**   |        **有**        |       **不可**        | **传统的左值，就是我们通常说的“变量”。它有自己专属的“房间”（内存地址），住得稳稳的，一般不会被“请走”（移动）** |
|   **`xvalue`**   |  **将亡值**  |        **有**        |        **可**         | **C++11 引入的新概念。它虽然有自己的“房间”，但这个房间马上就要被拆了，里面的东西（资源）可以大方地送给别人，实现高效的“资源转移”** |
|   **`rvalue`**   |   **右值**   |      **不一定**      |        **可**         | **泛指所有“可被移动”的表达式，是 `prvalue` 和 `xvalue` 的统称** |
|  **`prvalue`**   |  **纯右值**  |        **无**        |        **可**         | **传统的右值。它没有自己的“房间”，是一个临时的、用完就扔的值，比如字面量 `42` 或函数返回的临时对象** |

**身份（identity）：简单来说，如果一个表达式你能通过 `&` 运算符对它取地址，那它大概率就拥有“身份”（是一个 `glvalue`）**



###  **主要类别**

#### **lvalue**

**以下表达式是 *lvalue 表达*式：**

- **变量、函数 、[ 模板参数对象 ](https://www.cppreference.com/cpp/language/template_parameters#Non-type_template_parameter)（自 C++20 起） 或数据成员的名称，无论类型如何，如 `std::cin` 或 `std::endl`。即使变量类型是 rvalue 引用，其名称组成的表达式也是 lvalue 表达式（但参见[可移动的表达式 ](https://www.cppreference.com/cpp/language/value_category#Move-eligible_expressions)）;**
- **函数调用或重载运算子表达式，其返回类型为 lvalue 引用，如 `std::getline（std::cin， str）`、`std::cout << 1`、`str1 = str2`，或 `++it`;**
- **`a = b`,`a+= b`,`a%= b`，以及所有其他内置[赋值和复合赋值](https://www.cppreference.com/cpp/language/operator_assignment)表达式;**
- **`++a` 和 `--a`，内置[的预递增和预递减](https://www.cppreference.com/cpp/language/operator_incdec#Built-in_prefix_operators)表达式;**
- **`*p`，内置[间接](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_indirection_operator)表达式;**
- **`a[n]` 和 `p[n]`，即内置[的下标](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_subscript_operator)表达式 ，其中 `a[n]` 中的一个操作数是数组值**
- **`a.m`，对象表达式[的成员 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_member_access_operators)，除非 `m` 是成员枚举器或非静态成员函数，或 `a` 是 rvalue 且 `m` 是对象类型的非静态数据成员;**
- **`p->m`，指[针表达式的内置成员 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_member_access_operators)，除非 `m` 是成员枚举器或非静态成员函数;**
- **`a.*mp`，对象表达[式指向成员的指针 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators)，`其中 a` 是 lvalue，`mp` 是指向数据成员的指针;**
- **`p->*mp`，指针表达式的内置[指向成员的指针 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators)，其中 `mp` 是指向数据成员的指针;**
- **`a，b`，内置[逗号](https://www.cppreference.com/cpp/language/operator_other#Built-in_comma_operator)表达式，其中 `b` 为 lvalue;**
- **`a ? b : c`，是某些 `b` 和 `c` 的[三元条件](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)表达式（例如，当两者都是同一类型的 lvalue 时，但详见[定义 ](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)）;**
- **字符串的[字面意思 ](https://www.cppreference.com/cpp/language/string_literal)，如 `“Hello， world！”`;**
- **将 lvalue 引用类型的 cast 表达式，如 `static_cast<int&>（x）` 或 `static_cast<void（&）（int）>（x）`;**
- **一个非类型[模板参数](https://www.cppreference.com/cpp/language/template_parameters)的 lvalue 引用类型;**
- **函数调用或超载运算子表达式，其返回类型为函数的 rvalue引用;**
- **一个将 rvalue引用到函数类型（例如 `：static_cast<void（&&）（int）>（x）` 的 cast 表达式。**





#### **cast 表达式是干啥用的？**

**cast 表达式就是“强制类型转换”。它的作用是把一个表达式从一种类型转换成另一种类型。**

**C++ 中有几种 cast：**

- **C 风格 cast：`(int)x`**
- **C++ 风格 cast：`static_cast`、`dynamic_cast`、`const_cast`、`reinterpret_cast`**





##### **Properties**

- **和 [glvalue](https://www.cppreference.com/cpp/language/value_category#glvalue)（下文）一样。**
- **lvalue 的地址可以用内置的地址操作符来取：`&++i` [[1\]](https://www.cppreference.com/cpp/language/value_category#cite_note-1) 和 `&std：：endl` 是有效的表达式。**
- **可修改的` lvalue` 可用作内置赋值和复合赋值算子的左操作数。**
- **lvalue 可用于[初始化 lvalue引用 ](https://www.cppreference.com/cpp/language/reference_initialization);这会为表达式所识别的对象关联一个新名称。**



#### **prvalue**

**以下表达式为 *prvalue 表达*式：**

- **字[面值 ](https://www.cppreference.com/cpp/language/expressions#Literals)（字符串[字面值](https://www.cppreference.com/cpp/language/string_literal)除外），如 `42`、`true` 或 `nullptr`;**
- **函数调用或重载运算子表达式，其返回类型为非引用，如 `STR.substr（1，2）`，`str1 + str2`，或 `it++`;**
- **`a++` 和 `a--`，内置[的后增和后递减](https://www.cppreference.com/cpp/language/operator_incdec#Built-in_postfix_operators)表达式;**
- **`a + b`，`a % b`，`a & b`，`a << b`，以及所有其他内置[算术](https://www.cppreference.com/cpp/language/operator_arithmetic)表达式;**
- **`a && b`， `a || B`，`！a`，内置[逻辑](https://www.cppreference.com/cpp/language/operator_logical)表达式;**
- **`a < b`，`a == b`，`a >= b`，以及所有其他内置[的比较](https://www.cppreference.com/cpp/language/operator_comparison)表达式;**
- **`&a`，表达式的内置[地址 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_address-of_operator);**
- **`a.m`，对象表达式[的成员 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_member_access_operators)，其中 `m` 是成员枚举器或非静态成员函数 [[2\]](https://www.cppreference.com/cpp/language/value_category#cite_note-pmfc-2) ;**
- **`p->m`，[ 指针](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_member_access_operators)表达式的内置成员，其中 `m` 是成员枚举器或非静态成员函数 [[2\]](https://www.cppreference.com/cpp/language/value_category#cite_note-pmfc-2) ;**
- **`a.*mp`，对象表达[式的指向成员的指针 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators)，其中 `MP` 是指向成员的指针函数 [[2\]](https://www.cppreference.com/cpp/language/value_category#cite_note-pmfc-2) ;**
- **`p->*mp`，指针表达式中内置[指向成员的指针 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators)，其中 `mp` 是指向成员的指针函数 [[2\]](https://www.cppreference.com/cpp/language/value_category#cite_note-pmfc-2) ;**
- **`a，b`，内置[逗号](https://www.cppreference.com/cpp/language/operator_other#Built-in_comma_operator)表达式，其中 `b` 是 prvalue;**
- **`a ? b : c`，是某些 `b` 和 `c` 的[三元条件](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)表达式（详见[定义 ](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)）;**
- **将表达式转换为非引用类型，如 `static_cast<double>（x）`、`std::string{}` 或 `（int）42`;**
- **`this`指针**
- **`an enumerator; `枚举器**
- **标量类型的非类型[模板参数 ](https://www.cppreference.com/cpp/language/template_parameters);**
- - **a [lambda expression](https://www.cppreference.com/cpp/language/lambda), such as `[](int x){ return x * x; }`;**
  - **a [requires-expression](https://www.cppreference.com/cpp/language/constraints), such as `requires (T i) { typename T::type; }`;**
  - **a specialization of a [concept](https://www.cppreference.com/cpp/language/constraints), such as `std::equality_comparable<int>`.**



##### **Properties**

- **与 [rvalue ](https://www.cppreference.com/cpp/language/value_category#rvalue)（下文）相同。**
- **prvalue 不能多[态 ](https://www.cppreference.com/cpp/language/object#Polymorphic_objects)：它所表示对象的[动态类型](https://www.cppreference.com/cpp/language/type#Dynamic_type)总是表达式的类型。**
- **非类非数组的判值不能被 [cv 限定 ](https://www.cppreference.com/cpp/language/cv)， 除非它被[实体化](https://www.cppreference.com/cpp/language/implicit_conversion#Temporary_materialization)以绑[定到 ](https://www.cppreference.com/cpp/language/reference_initialization)cv 限定类型的引用 （自 C++17 起）。（注：函数调用或 cast 表达式可能得到非类 cv 限定类型的 prvalue，但 cv 限定符通常会立即被剔除。）**
- **prvalue不能有[不完全类型 ](https://www.cppreference.com/cpp/language/type#Incomplete_type)（除了类型`空缺 `，见下文，或在 [`decltype`](https://www.cppreference.com/cpp/language/decltype) 指定符中使用时）。**
- **prvalue 不能有[抽象类类型](https://www.cppreference.com/cpp/language/abstract_class)或其数组。**



#### **xvalue**

**以下表达式是 *xvalue*表达式：**

- **`a.m`，对象表达式[的成员 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_member_access_operators)，其中 `a` 是 rvalue，`m` 是对象类型的非静态数据成员;**
- **`a.*mp`，对象表达[式的成员指针 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators)，其中 `A` 是 rvalue，`MP` 是指向数据成员的指针;**
- **`a，b`，内置[逗号](https://www.cppreference.com/cpp/language/operator_other#Built-in_comma_operator)表达式，其中 `b` 为 xvalue;**
- **`a ？ b ： c`，是某些 `b` 和 `c` 的[三元条件](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)表达式（详见[定义 ](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)）;**
- - **函数调用或重载运算子表达式，其返回类型为对象的 rvalue引用，如 `std：：move（x）`;**
  - **`a[n]`，内置[下标](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_subscript_operator)表达式，其中一个操作数是数组的 rvalue;**
  - **一个将 rvalue 引用对象类型的 cast 表达式，例如 `static_cast<char&&>（x）`;**
  - **任何在[临时物质化](https://www.cppreference.com/cpp/language/implicit_conversion#Temporary_materialization)后指代临时对象的表达式;**
  - **一个[可移动的表达 ](https://www.cppreference.com/cpp/language/value_category#Move-eligible_expressions)。**



##### **Properties**

- **与 rvalue（下文）相同。**
- **特别地，像所有 rvalue 一样，xvalue 绑定到 rvalue 引用，且像所有 glvalue一样，xvalue可能是[多态](https://www.cppreference.com/cpp/language/object#Polymorphic_objects)的，非类 xvalues 可能被 [cv 限定 ](https://www.cppreference.com/cpp/language/cv)。**





#### **为什么 lvalue 列表和 prvalue 列表有重叠？xvalue 也有重叠？**

**注意：这些列表是互斥的。一个表达式不可能同时是 lvalue 和 prvalue。你觉得重叠是因为你看到了类似的语法形式，但实际是不同的情况。**

##### **例：`a.m`（成员访问）**

- **在 lvalue 列表 中：`a.m`，其中 `a` 是 lvalue，且 `m` 是对象类型的非静态数据成员 → 结果是 lvalue。**
- **在 xvalue 列表 中：`a.m`，其中 `a` 是 rvalue，且 `m` 是对象类型的非静态数据成员 → 结果是 xvalue。**
- **在 prvalue 列表 中：`a.m`，其中 `m` 是成员枚举器或非静态成员函数 → 结果是 prvalue。**

**同样是 `a.m` 这个形式，结果的值类别取决于 `a` 的值类别和 `m` 的种类。 所以它们出现在不同列表里，并不矛盾。**

##### **例：`a, b`（逗号运算符）**

- **lvalue 列表：如果 `b` 是 lvalue，则整个逗号表达式是 lvalue。**
- **xvalue 列表：如果 `b` 是 xvalue，则整个是 xvalue。**
- **prvalue 列表：如果 `b` 是 prvalue，则整个是 prvalue。**

**规则：逗号表达式的值类别与最后一个表达式相同。**

##### **例：三元条件 `a ? b : c`**

- **如果 `b` 和 `c` 都是同一类型的 lvalue，则结果是 lvalue。**
- **如果一个是 xvalue，另一个是 xvalue，则结果是 xvalue。**
- **如果都是 prvalue，则结果是 prvalue。**

**所以同一语法形式出现在多个列表里，因为值类别依赖于操作数的值类别。**

**结论：这些列表不是互斥的“语法模式列表”，而是“在某种条件下，该表达式属于此类别的列表”。一个具体的表达式只能属于一个类别。cppreference 这么写是为了完整覆盖所有可能情况，不是让你以为同一个写法可以既是左值又是右值。**





#### **Mixed categories 混合类别**

##### **glvalue**

***glvalue 表达式*要么是 lvalue，要么是 xvalue。**

**Properties:** 

- **glvalue 可以通过 lvalue 到 rvalue、数组到指针或函数到指针的[隐式转换](https://www.cppreference.com/cpp/language/implicit_conversion)来隐式转换为 prvalue。**
- **glvalue可以是[多态](https://www.cppreference.com/cpp/language/object#Polymorphic_objects)的：它所识别对象的[动态类型](https://www.cppreference.com/cpp/language/type#Dynamic_type)不一定是表达式的静态类型。**
- **一个 glvalue 可以有[不完全类型 ](https://www.cppreference.com/cpp/language/type#Incomplete_type)，前提是表达式允许。**



##### **rvalue** 

***rvalue 表达式*可以是 prvalue 或 xvalue。**

**Properties:** 

- **rvalue的地址不能被内置的 address-of 操作符取出：`&int（）`、`&i++` [[3\]](https://www.cppreference.com/cpp/language/value_category#cite_note-3) 、`&42` 和 `&std：：move（x）` 均无效。**
- **rvalue不能作为内置赋值或复合赋值算子的左操作数。**
- **rvalue 可用于[初始化 const lvalue 引用 ](https://www.cppreference.com/cpp/language/reference_initialization)，此时 rvalue 所识别的临时对象的生命周期会[被延长 ](https://www.cppreference.com/cpp/language/reference_initialization#Lifetime_of_a_temporary)，直到引用的作用域结束。**
- - **rvalue可用于[初始化 rvalue 引用 ](https://www.cppreference.com/cpp/language/reference_initialization)，此时 rvalue 所识别的临时对象的生命周期会[被延长 ](https://www.cppreference.com/cpp/language/reference_initialization#Lifetime_of_a_temporary)，直到引用的作用域结束。**
  - **当用作函数参数时，且存在[两个重载 ](https://www.cppreference.com/cpp/language/overload_resolution)，一个取 rvalue 参考参数，另一个取  lvalue 引用 const 参数， rvalue 绑定 rvalue 引用重载（因此，如果复制和移动构造子都可用，rvalue参数调用 [move 构造子 ](https://www.cppreference.com/cpp/language/move_constructor)，复制和移动赋值运算符亦然）。**



#### **Special categories 特殊类别**

- ##### **Pending member function call 待处理成员函数调用**

**表达式 `a.mf` 以及 `p->mf`，其中 `mf` 是[非静态成员函数 ](https://www.cppreference.com/cpp/language/member_functions)，表达式 `a.*pmf` 和 `p->*pmf`（其中 `pmf` 是[指向成员函数的指针 ](https://www.cppreference.com/cpp/language/pointer#Pointers_to_member_functions)）被归类为 prvalue 表达式，但它们不能用于初始化引用、作为函数参数，或用于任何目的，除非作为函数调用操作符的左侧参数，例如 `（p->*pmf）（args）。`**

- ##### **Void expressions 虚空表达**

**返回 `void` 的函数调用表达式、将表达式投射为 `void` 以及[抛出表达](https://www.cppreference.com/cpp/language/throw)式被归类为 prvalue 表达式，但它们不能用于初始化引用或作为函数参数。它们可以在丢弃值的上下文中使用（例如在单独的行上，作为逗号运算符的左操作数等），也可以用于返回`void`值函数的`return`语句中。此外，抛出表达式也可以作为[条件运算符 ？：](https://www.cppreference.com/cpp/language/operator_other) 的第二和第三操作数使用。**

- ##### **Bit-fields 位域**

**指定[位域](https://www.cppreference.com/cpp/language/bit_field)的表达式（例如 `a.m`，其中 `a` 是类型为`struct A { int m: 3; }` 的 lvalue 形式）是 glvalue 表达式：它可以用作赋值操作符的左手操作数，但其地址不可取，且不能绑定非 const lvalue 引用。cont lvalue reference 或 rvalue reference 可以从位域 glvalue 初始化，但会临时复制该位域：它不会直接绑定到位域。**

- ##### **Move-eligible expressions 可移动的表达式**

**Although an expression consisting of the name of any variable is an lvalue expression, such expression may be move-eligible if it appears as the operand of**

- - **a [`return`](https://www.cppreference.com/cpp/language/return) statement**
  - **a [`co_return`](https://www.cppreference.com/cpp/language/coroutines) statement (since C++20)**
  - **a [`throw`](https://www.cppreference.com/cpp/language/throw) expression (since C++17)**

**如果表达式是移动可行的，则将其视为 rvalue 或 lvalue （直到 C++23）， 作为 rvalue （自 C++23 起）， 以实现[重载解析 ](https://www.cppreference.com/cpp/language/overload_resolution)（因此可能选择[移动构造子 ](https://www.cppreference.com/cpp/language/move_constructor)）。详情请参见 [“自动从局部变量和参数移动 ](https://www.cppreference.com/cpp/language/return#Automatic_move_from_local_variables_and_parameters)”。**



**在 C++11 中，表达式：**

- **have identity and cannot be moved from are called *lvalue* expressions;**
  **具有单位元且不可从中移动，称为 *l 值*表达式;**
- **have identity and can be moved from are called *xvalue* expressions;**
  **具有单位元且可以从中移动，称为 *x 值*表达式;**
- **do not have identity and can be moved from are called *prvalue* ("pure rvalue") expressions;**
  **没有单位元且可以从中移动的，称为 *prvalue*（“纯 r 值”）表达式;**
- **do not have identity and cannot be moved from are not used[[6\]](https://www.cppreference.com/cpp/language/value_category#cite_note-6).**
  **没有身份，且无法从中移动，不使用 [[6\]](https://www.cppreference.com/cpp/language/value_category#cite_note-6) 。**



**可以参考：**

**![image-20260419160049412](C:\Users\28251\AppData\Roaming\Typora\typora-user-images\image-20260419160049412.png)**





## `std::shared_ptr` 的四种强制类型转换

定义在头部 `<memory>`

### 四种转换函数

cppreference 页面主要介绍了这四个函数，它们的本质作用相同：**创建一个新的 `std::shared_ptr<T>`，它与传入的 `shared_ptr<U>` 共享同一个对象的所有权，但通过不同的 `cast` 来提供 `T*` 类型的访问视角**--[文档点击这里](https://www.cppreference.com/cpp/memory/shared_ptr/pointer_cast#Version_1)。

| 函数名                             | 等效的裸指针转换       | 主要用途 & 关键特性                                          |
| :--------------------------------- | :--------------------- | :----------------------------------------------------------- |
| `std::static_pointer_cast<T>`      | `static_cast<T*>`      | **用途**：在编译时已知继承关系的类之间进行转换（如向上转型 `Derived*` 到 `Base*`，或向下转型 `Base*` 到 `Derived*` 但不做运行时检查）。 <br />**安全性**：**不做运行时检查**。如果向下转型，程序员必须保证类型是正确的，否则会导致未定义行为[-1](https://www.cppreference.com/cpp/memory/shared_ptr/pointer_cast#Version_1)。 |
| `std::dynamic_pointer_cast<T>`     | `dynamic_cast<T*>`     | **用途**：安全地进行向下转型。在多态继承体系中，将一个基类指针转换为派生类指针。 <br />**安全性**：**会做运行时检查**。如果转换失败，它会返回一个**空的 `std::shared_ptr`**，而不是抛出异常或产生未定义行为。这是一个非常安全的设计[-1](https://www.cppreference.com/cpp/memory/shared_ptr/pointer_cast#Version_1)。 |
| `std::const_pointer_cast<T>`       | `const_cast<T*>`       | **用途**：为指针添加或移除 `const` 或 `volatile` 限定符。 <br />**注意**：主要用于与接受非 `const` 指针参数的旧式 C API 进行交互，**不建议**用其来修改原本声明为 `const` 的对象，否则同样是未定义行为[-1](https://www.cppreference.com/cpp/memory/shared_ptr/pointer_cast#Version_1)。 |
| `std::reinterpret_pointer_cast<T>` | `reinterpret_cast<T*>` | **用途**：进行低级的、与平台相关的“位重解释”转换。例如，将指针转换为 `uintptr_t` 或在一个不相关的类之间进行转换。 <br />**注意**：这是最危险的转换，**除非你明确知道自己在做什么，否则避免使用**。自 C++17 起可用[-1](https://www.cppreference.com/cpp/memory/shared_ptr/pointer_cast#Version_1)。 |

**C++20 的新变化 (Rvalue Overloads)**
该页面也提到了自 C++20 起增加的右值引用重载版本[（(2), (4), (6), (8)）](https://www.cppreference.com/cpp/memory/shared_ptr/pointer_cast#Version_1)。它允许你从将要销毁的 `shared_ptr` 中进行转换，可以避免不必要的引用计数原子操作，从而略微提升性能。例如：`auto p2 = std::static_pointer_cast<Derived>(std::move(p1));` 调用后，`p1` 会被置为空。

#### 一个必须避免的致命错误

cppreference 页面中[“Notes”](https://www.cppreference.com/cpp/memory/shared_ptr/pointer_cast#Version_1)部分强调的一点，是很多开发者容易踩的坑。它指出下面的代码会导致**未定义行为**（通常表现为**双重删除**并导致程序崩溃）:

cpp

```c++
// 错误示例！会导致程序崩溃！
std::shared_ptr<Base> basePtr = std::make_shared<Derived>();

// 错误原因：创建了一个新的、独立的控制块来管理对象
std::shared_ptr<Derived> derivedPtr(static_cast<Derived*>(basePtr.get()));

// 当 basePtr 和 derivedPtr 离开作用域时，它们的控制块都会尝试 delete 同一个对象，
// 导致双重删除 (Double Free)！
```



正是为了避免这个问题，C++11才引入了 `std::static_pointer_cast` 等函数。

#### 正确用法示例

以下是正确的使用方式：

cpp

```c++
#include <iostream>
#include <memory>

class Base {
public:
    virtual ~Base() = default; // 多态基类必须有虚析构函数
    virtual void print() const { std::cout << "I am Base" << std::endl; }
};

class Derived : public Base {
public:
    void print() const override { std::cout << "I am Derived" << std::endl; }
    void derivedFunc() const { std::cout << "Calling derivedFunc" << std::endl; }
};

int main() {
    // 1. 准备一个指向派生类对象的基类智能指针
    std::shared_ptr<Base> basePtr = std::make_shared<Derived>();

    // 2. 使用 static_pointer_cast 进行向上转型 (Derived -> Base)
    // 这是安全的，并且通常不需要显式转换，但可以展示用法
    std::shared_ptr<Base> anotherBasePtr = std::static_pointer_cast<Base>(basePtr);

    // 3. 使用 dynamic_pointer_cast 进行安全的向下转型 (Base -> Derived)
    std::shared_ptr<Derived> derivedPtr = std::dynamic_pointer_cast<Derived>(basePtr);

    // 4. 必须检查 dynamic_pointer_cast 是否成功！
    if (derivedPtr) {
        // 转换成功，可以安全使用
        derivedPtr->derivedFunc(); 
    } else {
        std::cout << "Dynamic cast failed!" << std::endl;
    }

    // 5. 使用 const_pointer_cast
    std::shared_ptr<const int> constIntPtr = std::make_shared<const int>(42);
    std::shared_ptr<int> intPtr = std::const_pointer_cast<int>(constIntPtr);
    // 注意：虽然能编译通过，但如果原始对象真的是 const，修改它仍是未定义行为

    // 6. 所有 shared_ptr 共享所有权，只有一个控制块
    std::cout << "Use count: " << basePtr.use_count() << std::endl; // 输出：3 (basePtr, anotherBasePtr, derivedPtr)
    
    return 0;
}
```



#### 总结与速查表

| 场景                                 | 推荐函数                        | 备注                                                        |
| :----------------------------------- | :------------------------------ | :---------------------------------------------------------- |
| 将 `Derived` 转换为 `Base`           | `std::static_pointer_cast`      | 通常是隐式转换，但也可显式使用。                            |
| 将 `Base` **安全地**转换为 `Derived` | `std::dynamic_pointer_cast`     | **必须**检查返回值是否为空。                                |
| 添加或移除 `const/volatile`          | `std::const_pointer_cast`       | 慎用，仅用于与旧 API 交互。                                 |
| 在完全不相关的类型间转换             | `std::reinterpret_pointer_cast` | 除非你非常清楚后果，否则避免使用。                          |
| 确保 `shared_ptr` 转换后仍共享所有权 | **上述四种函数**                | **切勿**使用 `std::shared_ptr<T>(cast(r.get()))` 这种模式。 |

### 示例

```c++
#include <iostream>
#include <memory>

class Base {
public:
	int a;
	virtual void f() const { std::cout << "I am base!\n"; }
	virtual ~Base(){}
};

class Derived :public Base {
public:
	void f() const override { std::cout << "I am derived!\n"; }
	~Derived(){}
};

int main() {
	auto basePtr = std::make_shared<Base>();
	std::cout << "Base pointer says: ";
	basePtr->f();

	auto derivedPtr = std::make_shared<Derived>();
	std::cout << "Derived pointer says";
	derivedPtr->f();

	//static_pointer_cast向上提升类层次结构
	basePtr = std::static_pointer_cast<Base>(derivedPtr);
	std::cout << "指向派生的基指针表示： ";
	basePtr->f();

	//使用 dynamic_pointer_cast 在类层次结构中向下或横向转换
	auto downcastedPtr = std::dynamic_pointer_cast<Derived>(basePtr);
	if (downcastedPtr) {
		std::cout << "向下转换的指针显示： ";
		downcastedPtr->f();
	}

	//所有指向派生类的指针共享所有权
	std::cout<<"指向底层派生的指针： "
		<<derivedPtr.use_count()
		<<'\n';
}
```

#### 分析*所有指向派生类的指针共享所有权*过程：

1. `auto derivedPtr = std::make_shared<Derived>();`
   → `derivedPtr` 指向新创建的 `Derived` 对象，引用计数 = 1。
2. `basePtr = std::static_pointer_cast<Base>(derivedPtr);`
   - `static_pointer_cast` 从 `derivedPtr` 生成一个新的 `shared_ptr<Base>`，与 `derivedPtr` **共享所有权**，因此引用计数变为 2。
   - 赋值给 `basePtr` 前，原 `basePtr`（指向之前的 `Base` 对象）被重置，原 `Base` 对象引用计数减 1（变为 0）并被销毁。
   - 赋值后，`basePtr` 指向 `Derived` 对象，引用计数仍为 2（`derivedPtr` 和 `basePtr` 共享）。
3. `auto downcastedPtr = std::dynamic_pointer_cast<Derived>(basePtr);`
   - 因为 `basePtr` 实际指向 `Derived`，转换成功，返回一个新的 `shared_ptr<Derived>`，**再次共享所有权**。
   - 引用计数变为 3。

此时共有三个 `shared_ptr` 指向同一个 `Derived` 对象：`derivedPtr`、`basePtr`、`downcastedPtr`。
因此 `derivedPtr.use_count()` 输出 **3**。



## `share_ptr`的原子操作函数

定义在头部 `<memory>`

