//#include <iostream>
//#include <thread>
//#include <windows.h>
//using namespace std;
//
//void worker(int id) {
//	cout << "工人" << id << "开始干活了" << endl;
//	this_thread::sleep_for(chrono::seconds(1));
//	cout << "工人" << id << "干完活了" << endl;
//}
//
//void increment(int& x) {
//	x++;
//}
//
//int main() {
//	thread t1(worker, 1), t2(worker, 2), t3(worker, 3);	//线程创建时就立刻开始执行、并且是并行执行的谁都不会阻拦到谁
//
//	t1.join();
//	t2.join();
//	t3.join();
//	cout << "主线程：知道工人们都干完活了，好，收工！" << endl;
//	Sleep(1200);
//
//	cout << "===============================================================================" << endl;
//	int Count = 0;
//	cout << "调用线程前Count = " << Count << endl;
//	//thread t4(increment, Count);		编译错误！因为无法用右值（拷贝的临时对象）绑定到 int&
//	thread t5(increment, ref(Count));
//	t5.join();
//	cout << "调用用 std::ref 包装的拷贝线程后Count = " << Count << endl;
//
//}