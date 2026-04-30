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