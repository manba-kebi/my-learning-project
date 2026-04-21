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