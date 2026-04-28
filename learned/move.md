# 移动构造器

移动构造函数是一种能够以相同类类型的参数调用并复制参数内容的[构造函数](https://cppreference.com/cpp/language/constructor)，可能会修改参数。



## 语法

- *class-name* ﻿`(`*parameter-list* ﻿`);`

- *class-name* ﻿`(`*parameter-list*` )` *function-body*

- *class-name* ﻿`(`*single-parameter-list* ﻿`) = default;`

- *class-name* ﻿`(parameter-list ﻿`**) = delete;**

- *class-name* ﻿`::`*class-name* ﻿`(`*parameter-list* ﻿`)` *function-body*

- *class-name* ﻿`::`*class-name* ﻿`(`*single-parameter-list* ﻿`) = default;`

|      **class-name**       |                  **声明其移动构造函数的类**                  |
| :-----------------------: | :----------------------------------------------------------: |
|    **parameter-list**     | **满足以下所有条件的非空参数列表：<br />给定类类型为T，第一个参数的类型为`T&&`、`const T&&`，`volatile T&&`或`const volatile T&&`，以及<br />要么没有其他参数，要么所有其他参数都有默认参数** |
| **single-parameter-list** | **仅包含一个参数的参数列表，该参数的类型为`T&&`、`const T&&`、`volatile T&&`或`const volatile T&&`，并且没有默认参数** |
|     **function-body**     |                  **`move`构造函数的函数体**                  |



## 解释

- （1）在类定义中声明移动构造函数。
- 类定义中移动构造函数的定义。
  - 移动构造函数是显式默认的。
  - 移动构造函数已删除。
- 类定义之外的移动构造函数的定义（类必须包含声明（1））。
  - 移动构造函数是显式默认的。



```c++
struct X
{
    X(X&& other); // move constructor
//  X(X other);   // Error: incorrect parameter type
};

union Y
{
    Y(Y&& other, int num = 1); // move constructor with multiple parameters
//  Y(Y&& other, int num);     // Error: `num` has no default argument
};
```



移动构造函数通常在对象从相同类型的 [rvalue](https://cppreference.com/cpp/language/value_category#rvalue)（xvalue或prvalue）初始化（通过直接初始化或复制初始化）时调用，包括：

- 初始化：`T a = std：move（b）;` 或 `T a（std::move（b））;` 其中 `b` 属于`类型 T`;
- 函数参数传递：`f(std::move(a));`，其中`a` 类型为 `T`，f 为空`void f(T t)`;
- 函数返回：`return a;`，在`T f()`这样的函数中，`a`的类型为`T`，具有`move`构造函数。

当初始化器是一个`prvalue`时，`move`构造函数调用通常会被优化掉，而从未被调用，请参阅[复制省略](https://cppreference.com/cpp/language/copy_elision)。

**移动构造函数**通常会**转移参数所持有的资源（例如指向动态分配对象的指针、文件描述符、TCP套接字、线程句柄等），而不是复制它们**，并将参数保持在某种有效但不确定的状态。**由于移动构造函数不会改变参数的生存期，因此通常会在稍后对参数调用析构函数**。例如，从`std::string`或`std::vector`移动可能会导致参数为空。对于某些类型，例如`std::unique_ptr`，完全指定了移出状态。



## 隐式声明的移动构造函数

如果没有为类类型提供用户定义的移动构造函数，并且以下所有条件都为真：

- 没有用户声明的[复制构造函数 ](https://cppreference.com/cpp/language/copy_constructor);
- 没有用户声明的[复制赋值操作符 ](https://cppreference.com/cpp/language/copy_assignment);
- 没有用户声明的[移动分配算符 ](https://cppreference.com/cpp/language/move_assignment);
- 没有用户声明的[析构函数](https://cppreference.com/cpp/language/destructor)。

然后，编译器将声明一个移动构造函数为其类的非[显式](https://cppreference.com/cpp/language/explicit)**内联公共**成员，并带有签名：`T::T（T&&）`。

一个类可以有多个移动构造函数，例如 `T::T(const T&&)` 和 `T::T(T&&)` 。如果存在一些用户定义的移动构造函数，用户仍然可以使用关键字default强制生成隐式声明的移动构造函数。



隐式声明（或默认在第一次声明时被默认）的移动构造器有一个异常规范，如[动态异常规范 ](https://cppreference.com/cpp/language/except_spec)（直到 C++17）和[noexcept](https://cppreference.com/cpp/language/noexcept_spec)规范（从C++17开始）中所述。



## 隐式定义的移动构造函数

如果隐式声明的移动构造函数既没有被删除也没有被忽略，那么如果需要 [ODR](https://cppreference.com/cpp/language/definition#ODR-use) 或[需要进行常量计算 ](https://cppreference.com/cpp/language/constant_expression#Functions_and_variables_needed_for_constant_evaluation)，它将由编译器定义（即生成并编译函数体）。对于联合类型，隐式定义的移动构造函数复制对象表示（如`std::memmove`）。对于非联合类类型，移动构造函数使用`xvalue`参数的直接初始化，按初始化顺序执行对象的直接基子对象和成员子对象的完全成员移动。对于引用类型的每个非静态数据成员，移动构造函数将引用绑定到与源引用绑定的相同对象或函数。



如果这满足constexpr构造函数（直到C++23）constexpr函数（从C++23开始）的要求，则生成的移动构造函数是constexpr。



## 已删除移动构造函数

如果`T`具有类类型`M`（或其多维数组）的[潜在构造](https://cppreference.com/cpp/language/object#Potentially_constructed_subobject)子对象，则类`T`的隐式声明或显式默认移动构造函数被定义为删除，从而：

- M有一个析构函数，该析构函数已被删除或无法从复制构造函数中访问，或者
- 用于查找M的移动构造函数的重载解析
  - 没有产生可用的候选人，或
  - 如果子对象是[变体成员](https://cppreference.com/cpp/language/union#Union-like_classes)，则选择一个非平凡函数。

[重载解析](https://cppreference.com/cpp/language/overload_resolution)会忽略这样的构造函数（否则将阻止从右值进行复制初始化）。



## 平凡移动构造函数

如果满足以下所有条件，则类`T`的移动构造函数是平凡的：

- 它不是用户提供的（即，它是隐式定义的或默认的）；
- `T`没有虚拟成员函数；
- `T`没有虚拟基类；
- 每个 `T` 的直接基选择的移动构造器是平凡的;
- 为 `T` 中每个非静态类类型（或类类型数组）成员选择的移动构造函数是平凡的。

**平凡移动构造函数**是一种执行与平凡**复制**构造函数相同操作的构造函数，即复制对象表示，就像通过`std::memmove`一样。所有与C语言兼容的数据类型都可以轻松移动。



## 合格的移动构造函数

- 如果未删除移动构造函数，则该构造函数符合条件。
- 如果满足以下所有条件，则移动构造函数有资格：
  - 它不会被删除。
  - 其[相关的约束](https://cppreference.com/cpp/language/constraints)条件（如果有的话）都满足了。
  - 满足相关约束的移动构造函数没有受到[更多约束](https://cppreference.com/cpp/language/constraints#Partial_ordering_of_constraints)。

符合条件的**移动构造函数的平凡性**决定了**类是否是隐式生存期类型**，以及**类是否是可复制的平凡类型**。



## 注意

为了使[强异常保证](https://cppreference.com/cpp/language/exceptions#Exception_safety)成为可能，用户定义的移动构造函数不应抛出异常。例如，当需要重新定位元素时，`std::vector`依赖于`std::move_if_noexcept`在移动和复制之间进行选择。

如果同时**提供了复制和移动构造函数**，**并且没有其他可行的构造函数**，则**重载**解析**会在参数是相同类型的[rvalue](https://cppreference.com/cpp/language/value_category#rvalue)**（ [xvalue](https://cppreference.com/cpp/language/value_category#xvalue)，如`std::move`的结果，或 [prvalue](https://cppreference.com/cpp/language/value_category#prvalue)，如无名临时值（直到C++17））的情况下**选择移动构造函数**，并在**参数是 [lvalue](https://cppreference.com/cpp/language/value_category#lvalue)**（命名对象或返回左值引用的函数/运算符）的情况下**选择复制构造函数**。如果**只**提供了**复制构造函数**，**则所有参数类别都会选择它**（只要它需要引用`const`，因为`rvalues`可以绑定到`const引用`），这使得在移动不可用时，**复制是移动的回退**。



```C++
#include <iomanip>		//quoted 就来自 <iomanip>。quoted功能是给字符串自动加双引号，方便看清空字符串。
#include <iostream>
#include <string>
#include <utility>		//C++中专门使用移动语义的库		std::move → 把对象标记为可移动		std::exchange → 安全移动基础类型

struct A {
	std::string s;
	int k;

	A() :s("test"), k(-1) {};
	A(const A& o) :s(o.s), k(o.k) { std::cout << "move failed!\n"; }		//拷贝构造
	A(A&& o) noexcept:			//noexcept 告诉编译器：这个函数绝对不会抛异常
		s(std::move(o.s)),		//类类型成员的显示移动
		k(std::exchange(o.k,0))	//非类类型成员的显示移动
		//exchage 给变量赋新值，并返回旧值
	{ }
};

A f(A a) {
	return a;
}

struct B :A {
	std::string s2;
	int n;
	//隐式移动构造函数：B::(B&&)
	//调用 A 的移动构造函数
	//调用 s2 的移动构造函数
	//并按位复制n
};


struct C :B
{
	~C(){}	//析构函数防止隐式移动构造函数 C::(C&&)
};

struct D :B {
	D(){}
	~D(){}				//析构函数将会阻止隐式移动构造函数 D::(D&&)
	D(D&&) = default;	//无论如何都会强制使用移动构造函数
};

int main() {
	std::cout << "Trying to move A\n";
	A a1 = f(A());		//按值返回移动构造目标
						//从函数参数
	std::cout << "Before move,a1.s = " << std::quoted(a1.s)
		<< "a1.k = " << a1.k << '\n';

	A a2 = std::move(a1);	//从将亡值中移动构造
	std::cout << "After move, a1.s = " << std::quoted(a1.s)
		<< "a1.k = " << a1.k 
		<<"\tand a2.s = "<<std::quoted(a2.s)
		<<"a2.k = "<<a2.k << '\n';

	std::cout << "\nTrying to move B\n";
	B b1;

	std::cout << "Before move,b1.s = " << std::quoted(b1.s) << "\n";

	B b2 = std::move(b1);	//调用隐式移动构造函数
	std::cout << "After move,b1.s = " << std::quoted(b1.s) << "\n";

	std::cout << "\nTrying to move C\n";						//这里，等下课回来试试添加的输出，看看到底是怎么回事
	C c1;
	std::cout << "创建C类对象c1后，c1.s = " << std::quoted(c1.s)
		<< "c1.k = " << c1.k << std::endl;
	C c2 = std::move(c1);	//调用复制构造函数
	std::cout << "用c1移动构造创建C类对象c2后(这里调用的应该是拷贝构造)，c1.s = " << std::quoted(c1.s)
		<< "c1.k = " << c1.k << "\tc2.s = " << std::quoted(c2.s) << "c2.k = " << c2.k << std::endl;

	std::cout << "\nTrying to move D\n";
	D d1;
	std::cout << "创建D类对象d1后，d1.s = " << std::quoted(d1.s)
		<< "d1.k = " << d1.k << std::endl;
	D d2 = std::move(d1);
	std::cout << "用d1移动构造创建D类对象d2后，d1.s = " << std::quoted(d1.s)
		<< "d1.k = " << d1.k << "\td2.s = " << std::quoted(d2.s) << "d2.k = " << d2.k << std::endl;
}
```













