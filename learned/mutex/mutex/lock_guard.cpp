//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <string_view>
//#include <syncstream>
//
//using namespace std;
//
//volatile int g_i = 0;
//mutex g_i_mutex;	//保护 g_i
//
//void safe_increment(int iterations) {
//	const lock_guard<mutex> lock(g_i_mutex);
//	while (iterations-- > 0)
//		g_i = g_i + 1;
//	cout << "thread #" << this_thread::get_id() << ", g_i: " << g_i << '\n';
//	//当lock超出作用域时，g_i_mutex会自动释放
//}
//
//void unsafe_increment(int iterations) {
//	while (iterations-- > 0)
//		g_i = g_i + 1;
//	osyncstream(cout) << "thread #" << this_thread::get_id() << " ,g_i: " << g_i << '\n';
//}
//
//int main() {
//	auto test = [](string_view fun_name, auto fun) {
//		g_i = 0;
//		cout << fun_name << ":\nbefore,g_i: " << g_i << '\n';
//		{
//			std::jthread t1(fun, 1'000'000);
//			std::jthread t2(fun, 1'000'000);
//		}
//		cout << "after, g_i: " << g_i << "\n\n";
//	};
//	test("safe_increment", safe_increment);
//	test("unsafe_increment", unsafe_increment);
//}
