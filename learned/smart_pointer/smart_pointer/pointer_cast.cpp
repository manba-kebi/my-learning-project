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

	//static_pointer_cast to go up class hierarchy
	basePtr = std::static_pointer_cast<Base>(derivedPtr);
	std::cout << "指向派生的基指针表示： ";
	basePtr->f();

	//dynamic_pointer_cast to go down/across class hierarchy
	auto downcastedPtr = std::dynamic_pointer_cast<Derived>(basePtr);
	if (downcastedPtr) {
		std::cout << "向下转换的指针显示： ";
		downcastedPtr->f();
	}

	//All pointers to derived share ownership
	std::cout<<"指向底层派生的指针： "
		<<derivedPtr.use_count()
		<<'\n';
}