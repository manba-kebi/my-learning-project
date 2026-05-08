//#include <iostream>
//#include <thread>
//#include <chrono>
//using namespace std;
//
//void worker() {
//	cout << "工人 jthread 要开始干活了" << endl;
//	this_thread::sleep_for(chrono::seconds(1));
//	cout << "工人 jthread 干完活了" << endl;
//}
//
//void stoppable_worker(stop_token st) {
//	int progress = 0;
//	while (!st.stop_requested()) {		//只要没被请求停止，就继续循环
//		cout << "进度：" << progress++ << endl;
//		this_thread::sleep_for(chrono::milliseconds(500));
//		if (progress > 10) {
//			cout << "工作自己完成了" << endl;
//			return;
//		}
//	}
//	cout << "收到停止请求，清理并退出" << endl;
//}
//
//int main() {
//	//jthread j(worker);
//	// 注意，这里没有写 j.join()
//	//cout << "主线程：jthread 会在销毁前自动 join，我不需要手动等。" << endl;
//
//	jthread j(stoppable_worker);
//
//	this_thread::sleep_for(chrono::seconds(2));
//	cout << "主线程：让他停下来吧" << endl;
//	j.request_stop();	//发出停止请求
//
//} // j 离开作用域，析构函数自动 join，安全