//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <chrono>
//#include <map>
//#include <string>
//
//using namespace std;
//map<string, string> g_pages;
//mutex g_pages_mutex;
//
//void save_page(const string& url) {
//	//模拟长页面提取
//	this_thread::sleep_for(chrono::seconds(2));
//	string result = "fake content";
//
//	lock_guard<mutex> guard(g_pages_mutex);
//	g_pages[url] = result;
//}
//
//
//int main() {
//	thread t1(save_page, "http://foo");
//	thread t2(save_page, "http://bar");
//	t1.join();
//	t2.join();
//
//	//现在无需锁即可安全访问g_pages，因为线程已连接
//	for (const auto& [url, page] : g_pages)
//		cout << url << "=>" << page << "\n";
//}