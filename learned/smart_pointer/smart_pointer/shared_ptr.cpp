//#include <chrono>
//#include <iostream>
//#include <memory>
//#include <mutex>
//#include <thread>
//
//using namespace std::chrono_literals;
//
//struct Base {
//	Base() { std::cout << "Base::Base()\n"; }
//
//	//注意：此处非虚析构函数无妨
//	~Base() { std::cout << "Base::~Base()\n"; }
//};
//
//struct Derived :public Base {
//	Derived() { std::cout << "Derived::Derived()\n"; }
//
//	~Derived() { std::cout << "Derived::~Derved()\n"; }
//};
//
//template <typename T>
//void print(T rem, std::shared_ptr<Base> const& sp)  {
//	std::cout << rem << "\n\tget() = " << sp.get()
//		<< ",use_count() = " << sp.use_count() << '\n';
//}
//
//void thr(std::shared_ptr<Base> p) {
//	std::this_thread::sleep_for(987ms);
//	std::shared_ptr<Base> lp = p;	//线程安全，即使共享use_count递增
//	{
//		static std::mutex io_mutex;
//		std::lock_guard<std::mutex> lk(io_mutex);
//		print("线程中的本地指针：", lp);
//	}
//}
//
//int main() {
//	std::shared_ptr<Base> p = std::make_shared<Derived>();
//
//	print("创建了一个共享的Derived（作为指向Base的指针）", p);
//
//	std::thread t1{ thr,p }, t2{ thr,p }, t3{ thr,p };
//	p.reset();	//从main中释放所有权
//
//	print("3个线程之间共享所有权，并从main释放所有权：", p);
//
//	t1.join();
//	t2.join();
//	t3.join();
//
//	std::cout << "所有线程均已完成，最后一个线程已删除。\n";
//}