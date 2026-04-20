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

`STD：：unique_ptr` 是一个智能指针，它拥有（负责）并通过指针管理另一个对象，当 `unique_ptr` 超出作用域时，它会处理该对象。

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



























# 额外学到的一些相关内容

## Value categories 价值类别

每个 C++ [表达式 ](https://www.cppreference.com/cpp/language/expressions)（一个运算符及其操作数、文字、变量名等）具有两个独立的属性：*[ 类型](https://www.cppreference.com/cpp/language/type)*和*值类别* 。每个表达式都有某种非指称类型，且每个表达式恰好属于三个主要值类别中的一个：[*prvalue*](https://www.cppreference.com/cpp/language/value_category#prvalue)、[*xvalue*](https://www.cppreference.com/cpp/language/value_category#xvalue) 和 [*lvalue*](https://www.cppreference.com/cpp/language/value_category#lvalue)。

- [glvalue](https://www.cppreference.com/cpp/language/value_category#glvalue)（“广义”lvalue）是一种表达式，其求值用来确定对象或函数的身份;
- [prvalue ](https://www.cppreference.com/cpp/language/value_category#prvalue)（“纯”rvalue）是一种其值的表达式
- - 计算内置算子操作数的值（该 PRvalue无*结果对象* ），或
  - 初始化一个对象（该 prvalue 称为具有*结果对象*，结果对象可以是变量、由[新表达](https://www.cppreference.com/cpp/language/new)式创建的对象、由[临时物质化](https://www.cppreference.com/cpp/language/implicit_conversion#Temporary_materialization)创建的临时对象，或其成员。注意，`non-void` [discard](https://www.cppreference.com/cpp/language/expressions#Discarded-value_expressions)表达式有一个结果对象（即物质化的临时表达式）。此外，每个类和数组的 prvalue 都有结果对象，除非它是 [`decltype`](https://www.cppreference.com/cpp/language/decltype) 的操作数; ）。
- [xvalue ](https://www.cppreference.com/cpp/language/value_category#xvalue)（“eXpiring”值）是一个 glvalue，表示资源可重复利用的对象;
- [lvalue](https://www.cppreference.com/cpp/language/value_category#lvalue) 是指不是 xvalue的 glvalue;
- [rvalue](https://www.cppreference.com/cpp/language/value_category#rvalue) 是 prvalue 或 xvalue;



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

- **`<type_traits>`**：C++11 引入的**类型特性**（type traits）库。
  它提供了一系列模板，可以在**编译期**检查和操作类型的信息。
  比如：`std::is_same<T, U>` 判断两个类型是否相同，`std::is_rvalue_reference<T>` 判断 `T` 是不是右值引用类型。
  **什么时候用**：当你需要在编译时根据类型做决策（比如模板编程、SFINAE、`static_assert` 检查）时就会用到。

- **`<utility>`**：实用工具库。
  里面包含了很多基础工具，比如 `std::move`、`std::forward`、`std::pair`、`std::integer_sequence` 等。
  **什么时候用**：当你需要移动语义、完美转发、或者使用 `std::pair` 等工具时。

- 代码中自定义了三个模板：

  cpp

  ```c++
  template <class T> struct is_prvalue : std::true_type {};
  template <class T> struct is_prvalue<T&> : std::false_type {};
  template <class T> struct is_prvalue<T&&> : std::false_type {};
  ```

  这是什么意思？

  - 对于大部分类型 `T`，`is_prvalue<T>` 继承自 `std::true_type`，表示“是 prvalue”。
  - 但是对于 `T&`（左值引用）和 `T&&`（右值引用）这两个特化版本，它们继承自 `std::false_type`，表示“不是 prvalue”。

  **为什么要这样做？**
  因为 C++ 中，表达式的值类别可以通过它的**decltype 获得的类型**来区分：

  - **prvalue** 表达式（比如字面量 `42`）在 `decltype((...))` 下的类型是**非引用**类型（例如 `int`）。
  - **lvalue** 表达式（比如变量名 `a`）会得到 `int&` 类型。
  - **xvalue** 表达式（比如 `std::move(a)`）会得到 `int&&` 类型。

  所以代码利用这个规律，通过检查 `decltype((expr))` 得到的类型是 `T`、`T&` 还是 `T&&`，来判断表达式的值类别。
  `std::true_type` 和 `std::false_type` 就是用来在编译期“标记”判断结果的。最终可以用 `::value` 取出布尔值，比如 `is_prvalue<decltype((42))>::value` 就是 `true`。

  > 小提示：`std::true_type` 和 `std::false_type` 是类型，而 `{}` 是创建它们的空对象。这里用作模板的默认继承，不实例化对象也可以，但是它们通常配合 `std::integral_constant` 使用。

- decltype是干什么的？

  `decltype(实体)` 的作用是：**获取这个实体的声明类型**。
  但是它的行为取决于你给的是 **变量名** 还是 **带括号的表达式**。

  ### 规则 1：`decltype(变量名)` —— 得到变量**声明时的类型**

  ```c++
  int a = 42;
  int& b = a;
  int&& r = std::move(a);
  
  decltype(a) t1;   // int
  decltype(b) t2;   // int&
  decltype(r) t3;   // int&&
  ```

  ### 规则 2：`decltype((表达式))` —— 得到**表达式**的类型（根据值类别决定）

  | 表达式的值类别 | `decltype((...))` 的结果 |
  | :------------- | :----------------------- |
  | **lvalue**     | `T&`（左值引用）         |
  | **xvalue**     | `T&&`（右值引用）        |
  | **prvalue**    | `T`（非引用）            |

  > 注意：这里的 `T` 是指该表达式的**非引用类型**（即去掉引用后的类型）。

  **总结**

  - **`decltype(变量名)`**：直接看变量定义的类型。
  - **`decltype((变量名))`**：看变量作为表达式时的值类别（左值 → 左值引用，右值 → 右值引用，纯右值 → 非引用）。
  - **`is_lvalue_reference`**：判断一个类型是不是 `T&`，这里的 `reference` 就是“引用”的意思。

  

| 概念（英文） | 中文译名 | 身份（identity） | 可移动（Movable） |                          大概的理解                          |
| :----------: | :------: | :--------------: | :---------------: | :----------------------------------------------------------: |
|  `glvalue`   |  泛左值  |        有        |      不一定       |   泛指所有“有身份”的表达式，是 `lvalue` 和 `xvalue` 的统称   |
|   `lvalue`   |   左值   |        有        |       不可        | 传统的左值，就是我们通常说的“变量”。它有自己专属的“房间”（内存地址），住得稳稳的，一般不会被“请走”（移动） |
|   `xvalue`   |  将亡值  |        有        |        可         | C++11 引入的新概念。它虽然有自己的“房间”，但这个房间马上就要被拆了，里面的东西（资源）可以大方地送给别人，实现高效的“资源转移” |
|   `rvalue`   |   右值   |      不一定      |        可         | 泛指所有“可被移动”的表达式，是 `prvalue` 和 `xvalue` 的统称  |
|  `prvalue`   |  纯右值  |        无        |        可         | 传统的右值。它没有自己的“房间”，是一个临时的、用完就扔的值，比如字面量 `42` 或函数返回的临时对象 |

**身份**（identity）：简单来说，如果一个表达式你能通过 `&` 运算符对它取地址，那它大概率就拥有“身份”（是一个 `glvalue`）



###  主要类别

#### lvalue

以下表达式是 *lvalue 表达*式：

- 变量、函数 、[ 模板参数对象 ](https://www.cppreference.com/cpp/language/template_parameters#Non-type_template_parameter)（自 C++20 起） 或数据成员的名称，无论类型如何，如 `std::cin` 或 `std::endl`。即使变量类型是 rvalue 引用，其名称组成的表达式也是 lvalue 表达式（但参见[可移动的表达式 ](https://www.cppreference.com/cpp/language/value_category#Move-eligible_expressions)）;
- 函数调用或重载运算子表达式，其返回类型为 lvalue 引用，如 `std::getline（std::cin， str）`、`std::cout << 1`、`str1 = str2`，或 `++it`;
- `a = b`,`a+= b`,`a%= b`，以及所有其他内置[赋值和复合赋值](https://www.cppreference.com/cpp/language/operator_assignment)表达式;
- `++a` 和 `--a`，内置[的预递增和预递减](https://www.cppreference.com/cpp/language/operator_incdec#Built-in_prefix_operators)表达式;
- `*p`，内置[间接](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_indirection_operator)表达式;
- `a[n]` 和 `p[n]`，即内置[的下标](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_subscript_operator)表达式 ，其中 `a[n]` 中的一个操作数是数组值
- `a.m`，对象表达式[的成员 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_member_access_operators)，除非 `m` 是成员枚举器或非静态成员函数，或 `a` 是 rvalue 且 `m` 是对象类型的非静态数据成员;
- `p->m`，指[针表达式的内置成员 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_member_access_operators)，除非 `m` 是成员枚举器或非静态成员函数;
- `a.*mp`，对象表达[式指向成员的指针 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators)，`其中 a` 是 lvalue，`mp` 是指向数据成员的指针;
- `p->*mp`，指针表达式的内置[指向成员的指针 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators)，其中 `mp` 是指向数据成员的指针;
- `a，b`，内置[逗号](https://www.cppreference.com/cpp/language/operator_other#Built-in_comma_operator)表达式，其中 `b` 为 lvalue;
- `a ? b : c`，是某些 `b` 和 `c` 的[三元条件](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)表达式（例如，当两者都是同一类型的 lvalue 时，但详见[定义 ](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)）;
- 字符串的[字面意思 ](https://www.cppreference.com/cpp/language/string_literal)，如 `“Hello， world！”`;
- 将 lvalue 引用类型的 cast 表达式，如 `static_cast<int&>（x）` 或 `static_cast<void（&）（int）>（x）`;
- 一个非类型[模板参数](https://www.cppreference.com/cpp/language/template_parameters)的 lvalue 引用类型;
- 函数调用或超载运算子表达式，其返回类型为函数的 rvalue引用;
- 一个将 rvalue引用到函数类型（例如 `：static_cast<void（&&）（int）>（x）` 的 cast 表达式。





#### cast 表达式是干啥用的？

**cast 表达式**就是“强制类型转换”。它的作用是把一个表达式从一种类型转换成另一种类型。

C++ 中有几种 cast：

- **C 风格 cast**：`(int)x`
- **C++ 风格 cast**：`static_cast`、`dynamic_cast`、`const_cast`、`reinterpret_cast`





##### Properties

- 和 [glvalue](https://www.cppreference.com/cpp/language/value_category#glvalue)（下文）一样。
- lvalue 的地址可以用内置的地址操作符来取：`&++i` [[1\]](https://www.cppreference.com/cpp/language/value_category#cite_note-1) 和 `&std：：endl` 是有效的表达式。
- 可修改的` lvalue` 可用作内置赋值和复合赋值算子的左操作数。
- lvalue 可用于[初始化 lvalue引用 ](https://www.cppreference.com/cpp/language/reference_initialization);这会为表达式所识别的对象关联一个新名称。



#### prvalue

以下表达式为 *prvalue 表达*式：

- 字[面值 ](https://www.cppreference.com/cpp/language/expressions#Literals)（字符串[字面值](https://www.cppreference.com/cpp/language/string_literal)除外），如 `42`、`true` 或 `nullptr`;
- 函数调用或重载运算子表达式，其返回类型为非引用，如 `STR.substr（1，2）`，`str1 + str2`，或 `it++`;
- `a++` 和 `a--`，内置[的后增和后递减](https://www.cppreference.com/cpp/language/operator_incdec#Built-in_postfix_operators)表达式;
- `a + b`，`a % b`，`a & b`，`a << b`，以及所有其他内置[算术](https://www.cppreference.com/cpp/language/operator_arithmetic)表达式;
- `a && b`， `a || B`，`！a`，内置[逻辑](https://www.cppreference.com/cpp/language/operator_logical)表达式;
- `a < b`，`a == b`，`a >= b`，以及所有其他内置[的比较](https://www.cppreference.com/cpp/language/operator_comparison)表达式;
- `&a`，表达式的内置[地址 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_address-of_operator);
- `a.m`，对象表达式[的成员 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_member_access_operators)，其中 `m` 是成员枚举器或非静态成员函数 [[2\]](https://www.cppreference.com/cpp/language/value_category#cite_note-pmfc-2) ;
- `p->m`，[ 指针](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_member_access_operators)表达式的内置成员，其中 `m` 是成员枚举器或非静态成员函数 [[2\]](https://www.cppreference.com/cpp/language/value_category#cite_note-pmfc-2) ;
- `a.*mp`，对象表达[式的指向成员的指针 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators)，其中 `MP` 是指向成员的指针函数 [[2\]](https://www.cppreference.com/cpp/language/value_category#cite_note-pmfc-2) ;
- `p->*mp`，指针表达式中内置[指向成员的指针 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators)，其中 `mp` 是指向成员的指针函数 [[2\]](https://www.cppreference.com/cpp/language/value_category#cite_note-pmfc-2) ;
- `a，b`，内置[逗号](https://www.cppreference.com/cpp/language/operator_other#Built-in_comma_operator)表达式，其中 `b` 是 prvalue;
- `a ? b : c`，是某些 `b` 和 `c` 的[三元条件](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)表达式（详见[定义 ](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)）;
- 将表达式转换为非引用类型，如 `static_cast<double>（x）`、`std::string{}` 或 `（int）42`;
- `this`指针
- `an enumerator; `枚举器
- 标量类型的非类型[模板参数 ](https://www.cppreference.com/cpp/language/template_parameters);
- - a [lambda expression](https://www.cppreference.com/cpp/language/lambda), such as `[](int x){ return x * x; }`;
  - a [requires-expression](https://www.cppreference.com/cpp/language/constraints), such as `requires (T i) { typename T::type; }`;
  - a specialization of a [concept](https://www.cppreference.com/cpp/language/constraints), such as `std::equality_comparable<int>`.



##### Properties

- 与 [rvalue ](https://www.cppreference.com/cpp/language/value_category#rvalue)（下文）相同。
- prvalue 不能多[态 ](https://www.cppreference.com/cpp/language/object#Polymorphic_objects)：它所表示对象的[动态类型](https://www.cppreference.com/cpp/language/type#Dynamic_type)总是表达式的类型。
- 非类非数组的判值不能被 [cv 限定 ](https://www.cppreference.com/cpp/language/cv)， 除非它被[实体化](https://www.cppreference.com/cpp/language/implicit_conversion#Temporary_materialization)以绑[定到 ](https://www.cppreference.com/cpp/language/reference_initialization)cv 限定类型的引用 （自 C++17 起）。（注：函数调用或 cast 表达式可能得到非类 cv 限定类型的 prvalue，但 cv 限定符通常会立即被剔除。）
- prvalue不能有[不完全类型 ](https://www.cppreference.com/cpp/language/type#Incomplete_type)（除了类型`空缺 `，见下文，或在 [`decltype`](https://www.cppreference.com/cpp/language/decltype) 指定符中使用时）。
- prvalue 不能有[抽象类类型](https://www.cppreference.com/cpp/language/abstract_class)或其数组。



#### xvalue

以下表达式是 *xvalue*表达式：

- `a.m`，对象表达式[的成员 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_member_access_operators)，其中 `a` 是 rvalue，`m` 是对象类型的非静态数据成员;
- `a.*mp`，对象表达[式的成员指针 ](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators)，其中 `A` 是 rvalue，`MP` 是指向数据成员的指针;
- `a，b`，内置[逗号](https://www.cppreference.com/cpp/language/operator_other#Built-in_comma_operator)表达式，其中 `b` 为 xvalue;
- `a ？ b ： c`，是某些 `b` 和 `c` 的[三元条件](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)表达式（详见[定义 ](https://www.cppreference.com/cpp/language/operator_other#Conditional_operator)）;
- - 函数调用或重载运算子表达式，其返回类型为对象的 rvalue引用，如 `std：：move（x）`;
  - `a[n]`，内置[下标](https://www.cppreference.com/cpp/language/operator_member_access#Built-in_subscript_operator)表达式，其中一个操作数是数组的 rvalue;
  - 一个将 rvalue 引用对象类型的 cast 表达式，例如 `static_cast<char&&>（x）`;
  - 任何在[临时物质化](https://www.cppreference.com/cpp/language/implicit_conversion#Temporary_materialization)后指代临时对象的表达式;
  - 一个[可移动的表达 ](https://www.cppreference.com/cpp/language/value_category#Move-eligible_expressions)。



##### Properties

- 与 rvalue（下文）相同。
- 特别地，像所有 rvalue 一样，xvalue 绑定到 rvalue 引用，且像所有 glvalue一样，xvalue可能是[多态](https://www.cppreference.com/cpp/language/object#Polymorphic_objects)的，非类 xvalues 可能被 [cv 限定 ](https://www.cppreference.com/cpp/language/cv)。





#### 为什么 lvalue 列表和 prvalue 列表有重叠？xvalue 也有重叠？

注意：**这些列表是互斥的**。一个表达式不可能同时是 lvalue 和 prvalue。你觉得重叠是因为你看到了类似的语法形式，但实际是不同的情况。

##### 例：`a.m`（成员访问）

- 在 **lvalue 列表** 中：`a.m`，其中 `a` 是 lvalue，且 `m` 是对象类型的非静态数据成员 → 结果是 lvalue。
- 在 **xvalue 列表** 中：`a.m`，其中 `a` 是 rvalue，且 `m` 是对象类型的非静态数据成员 → 结果是 xvalue。
- 在 **prvalue 列表** 中：`a.m`，其中 `m` 是成员枚举器或非静态成员函数 → 结果是 prvalue。

**同样是 `a.m` 这个形式，结果的值类别取决于 `a` 的值类别和 `m` 的种类。** 所以它们出现在不同列表里，并不矛盾。

##### 例：`a, b`（逗号运算符）

- lvalue 列表：如果 `b` 是 lvalue，则整个逗号表达式是 lvalue。
- xvalue 列表：如果 `b` 是 xvalue，则整个是 xvalue。
- prvalue 列表：如果 `b` 是 prvalue，则整个是 prvalue。

**规则**：逗号表达式的值类别与最后一个表达式相同。

##### 例：三元条件 `a ? b : c`

- 如果 `b` 和 `c` 都是同一类型的 lvalue，则结果是 lvalue。
- 如果一个是 xvalue，另一个是 xvalue，则结果是 xvalue。
- 如果都是 prvalue，则结果是 prvalue。

所以同一语法形式出现在多个列表里，因为**值类别依赖于操作数的值类别**。

**结论**：这些列表不是互斥的“语法模式列表”，而是“在某种条件下，该表达式属于此类别的列表”。一个具体的表达式只能属于一个类别。cppreference 这么写是为了完整覆盖所有可能情况，不是让你以为同一个写法可以既是左值又是右值。





#### Mixed categories 混合类别

##### glvalue

*glvalue 表达式*要么是 lvalue，要么是 xvalue。

Properties: 

- glvalue 可以通过 lvalue 到 rvalue、数组到指针或函数到指针的[隐式转换](https://www.cppreference.com/cpp/language/implicit_conversion)来隐式转换为 prvalue。
- glvalue可以是[多态](https://www.cppreference.com/cpp/language/object#Polymorphic_objects)的：它所识别对象的[动态类型](https://www.cppreference.com/cpp/language/type#Dynamic_type)不一定是表达式的静态类型。
- 一个 glvalue 可以有[不完全类型 ](https://www.cppreference.com/cpp/language/type#Incomplete_type)，前提是表达式允许。



##### rvalue 

*rvalue 表达式*可以是 prvalue 或 xvalue。

Properties: 

- rvalue的地址不能被内置的 address-of 操作符取出：`&int（）`、`&i++` [[3\]](https://www.cppreference.com/cpp/language/value_category#cite_note-3) 、`&42` 和 `&std：：move（x）` 均无效。
- rvalue不能作为内置赋值或复合赋值算子的左操作数。
- rvalue 可用于[初始化 const lvalue 引用 ](https://www.cppreference.com/cpp/language/reference_initialization)，此时 rvalue 所识别的临时对象的生命周期会[被延长 ](https://www.cppreference.com/cpp/language/reference_initialization#Lifetime_of_a_temporary)，直到引用的作用域结束。
- - rvalue可用于[初始化 rvalue 引用 ](https://www.cppreference.com/cpp/language/reference_initialization)，此时 rvalue 所识别的临时对象的生命周期会[被延长 ](https://www.cppreference.com/cpp/language/reference_initialization#Lifetime_of_a_temporary)，直到引用的作用域结束。
  - 当用作函数参数时，且存在[两个重载 ](https://www.cppreference.com/cpp/language/overload_resolution)，一个取 rvalue 参考参数，另一个取  lvalue 引用 const 参数， rvalue 绑定 rvalue 引用重载（因此，如果复制和移动构造子都可用，rvalue参数调用 [move 构造子 ](https://www.cppreference.com/cpp/language/move_constructor)，复制和移动赋值运算符亦然）。



#### Special categories 特殊类别

- ##### Pending member function call 待处理成员函数调用

表达式 `a.mf` 以及 `p->mf`，其中 `mf` 是[非静态成员函数 ](https://www.cppreference.com/cpp/language/member_functions)，表达式 `a.*pmf` 和 `p->*pmf`（其中 `pmf` 是[指向成员函数的指针 ](https://www.cppreference.com/cpp/language/pointer#Pointers_to_member_functions)）被归类为 prvalue 表达式，但它们不能用于初始化引用、作为函数参数，或用于任何目的，除非作为函数调用操作符的左侧参数，例如 `（p->*pmf）（args）。`

- ##### Void expressions 虚空表达

返回 `void` 的函数调用表达式、将表达式投射为 `void` 以及[抛出表达](https://www.cppreference.com/cpp/language/throw)式被归类为 prvalue 表达式，但它们不能用于初始化引用或作为函数参数。它们可以在丢弃值的上下文中使用（例如在单独的行上，作为逗号运算符的左操作数等），也可以用于返回`void`值函数的`return`语句中。此外，抛出表达式也可以作为[条件运算符 ？：](https://www.cppreference.com/cpp/language/operator_other) 的第二和第三操作数使用。

- ##### Bit-fields 位域

指定[位域](https://www.cppreference.com/cpp/language/bit_field)的表达式（例如 `a.m`，其中 `a` 是类型为`struct A { int m: 3; }` 的 lvalue 形式）是 glvalue 表达式：它可以用作赋值操作符的左手操作数，但其地址不可取，且不能绑定非 const lvalue 引用。cont lvalue reference 或 rvalue reference 可以从位域 glvalue 初始化，但会临时复制该位域：它不会直接绑定到位域。

- ##### **Move-eligible expressions 可移动的表达式**

Although an expression consisting of the name of any variable is an lvalue expression, such expression may be move-eligible if it appears as the operand of

- - a [`return`](https://www.cppreference.com/cpp/language/return) statement
  - a [`co_return`](https://www.cppreference.com/cpp/language/coroutines) statement (since C++20)
  - a [`throw`](https://www.cppreference.com/cpp/language/throw) expression (since C++17)

如果表达式是移动可行的，则将其视为 rvalue 或 lvalue （直到 C++23）， 作为 rvalue （自 C++23 起）， 以实现[重载解析 ](https://www.cppreference.com/cpp/language/overload_resolution)（因此可能选择[移动构造子 ](https://www.cppreference.com/cpp/language/move_constructor)）。详情请参见 [“自动从局部变量和参数移动 ](https://www.cppreference.com/cpp/language/return#Automatic_move_from_local_variables_and_parameters)”。



在 C++11 中，表达式：

- have identity and cannot be moved from are called *lvalue* expressions;
  具有单位元且不可从中移动，称为 *l 值*表达式;
- have identity and can be moved from are called *xvalue* expressions;
  具有单位元且可以从中移动，称为 *x 值*表达式;
- do not have identity and can be moved from are called *prvalue* ("pure rvalue") expressions;
  没有单位元且可以从中移动的，称为 *prvalue*（“纯 r 值”）表达式;
- do not have identity and cannot be moved from are not used[[6\]](https://www.cppreference.com/cpp/language/value_category#cite_note-6).
  没有身份，且无法从中移动，不使用 [[6\]](https://www.cppreference.com/cpp/language/value_category#cite_note-6) 。



可以参考：

![image-20260419160049412](C:\Users\28251\AppData\Roaming\Typora\typora-user-images\image-20260419160049412.png)





## C++ 命名需求：*MoveConstructible*
