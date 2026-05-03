//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <chrono>
//#include <string>
//#include <syncstream>
//#include <functional>
//#include <vector>
//
//using namespace std;
//using namespace chrono_literals;
//
//struct Employee {
//	vector<string> lunch_partners;
//	string id;
//	mutex m;
//	Employee(string id):id(id){}
//	string partners()const {
//		string ret = "Employee " + id + " has lunch partners: ";
//		for (int count{}; const auto& partner:lunch_partners)
//			ret += (count++ ? ", " : " ") + partner;
//		return ret;
//	}
//};
//
//void send_mail(Employee&, Employee&) {
//	//模拟耗时的消息传递操作
//	this_thread::sleep_for(1s);		//或者：chrono::seconds(1)
//}
//
//void assign_lunch_partner(Employee& e1, Employee& e2) {
//	osyncstream synced_out(cout);
//	synced_out << e1.id << " and " << e2.id << " are waiting for locks" << endl;
//
//	{
//		//使用std::scoped_lock获取两个锁，而不必担心其他对assign_lunch_partner的调用会使我们死锁，它还提供了一种方便的RAII风格机制
//		scoped_lock lock(e1.m, e2.m);
//
//		//等效代码1（使用std::lock和std::lock_guard）
//		//std::lock(e1.m, e2.m);
//		//std::lock_guard<std::mutex> lk1(e1.m, std::adopt_lock);
//		//std::lock_guard<std::mutex> lk2(e2.m, std::adopt_lock);
//
//		//等效代码2（如果需要unique_locks，例如用于条件变量）
//		//std::unique_lock<std::mutex> lk1(e1.m, std::defer_lock);
//		//std::unique_lock<std::mutex> lk2(e2.m, std::defer_lock);
//		//std::lock(lk1, lk2);
//
//		synced_out << e1.id << " and " << e2.id << " got locks " << endl;
//		e1.lunch_partners.push_back(e2.id);
//		e2.lunch_partners.push_back(e1.id);
//	}
//
//	send_mail(e1, e2);
//	send_mail(e2, e1);
//}
//
//int main() {
//	Employee alice("Alice"), bob("Bob"), christina("Christina"), dave("Dave");
//
//	//以并行线程进行分配，因为向用户发送午餐分配的邮件需要很长时间
//	vector<thread> threads;
//	threads.emplace_back(assign_lunch_partner, ref(alice), ref(bob));
//	threads.emplace_back(assign_lunch_partner, ref(christina), ref(bob));
//	threads.emplace_back(assign_lunch_partner, ref(christina), ref(alice));
//	threads.emplace_back(assign_lunch_partner, ref(dave), ref(bob));
//
//	for (auto& thread : threads)
//		thread.join();
//	osyncstream(cout) << alice.partners() << '\n'
//		<< bob.partners() << '\n'
//		<< christina.partners() << '\n'
//		<< dave.partners() << '\n';
//}