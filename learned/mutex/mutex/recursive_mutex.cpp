//#include <iostream>
//#include <thread>
//#include <mutex>
//
//using namespace std;
//
//class X {
//	recursive_mutex m;
//	string shared;
//public:
//	void fun1() {
//		lock_guard<recursive_mutex> lk(m);
//		shared = "fun1";
//		cout << "in fun1, shared varible is now " << shared << '\n';
//	}
//	void fun2() {
//		lock_guard<recursive_mutex> lk(m);
//		shared = "fun2";
//		cout << "in fun2, shared variable is now " << shared << '\n';
//		fun1();		//recursive lock 在这里变得有用
//		cout << "back in fun2,shared variable is " << shared << '\n';	//这里shared变成fun1了
//	}
//};
//
//int main() {
//	X x;
//	thread t1(&X::fun1, &x);
//	thread t2(&X::fun2, &x);
//	t1.join();
//	t2.join();
//}