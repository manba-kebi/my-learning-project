//#include <iostream>
//#include <thread>
//#include <mutex>
//
//using namespace std;
//
//struct Box {
//	explicit Box(int num):num_things{num}{}
//
//	int num_things;
//	mutex m;
//};
//
//void transfer(Box& from, Box& to, int num) {
//	//还没把锁拿走
//	unique_lock lock1{ from.m,defer_lock };
//	unique_lock lock2{ to.m,defer_lock };
//
//	//锁定两个unique_locks而不死锁
//	lock(lock1, lock2);
//	from.num_things -= num;
//	to.num_things += num;
//
//	//“from.m”和“to.m”互斥体在unique_lock dtors中解锁
//}
//
//int main() {
//	Box acc1{ 100 };
//	Box acc2{ 50 };
//	
//	thread t1{ transfer,ref(acc1),ref(acc2),10 };
//	thread t2{ transfer,ref(acc2),ref(acc1),5 };
//
//	t1.join();
//	t2.join();
//
//	cout << "acc1: " << acc1.num_things << "\n"
//		"acc2: " << acc2.num_things << "\n";
//}