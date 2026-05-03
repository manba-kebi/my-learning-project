#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <string>

using namespace std;

mutex m;
condition_variable cv;
string a_data;
bool ready = false;
bool processed = false;

void worker_thread() {
	//等待main（）发送数据
	unique_lock lk(m);
	cv.wait(lk, [] {return ready; });

	//等待之后，我们拥有了锁
	cout << "Worker thread is processing data\n";
	a_data += " after processing";

	//将数据发送回main（）
	processed = true;
	cout << "Worker thread signals data processing completed\n";

	//手动解锁在通知之前完成，以避免唤醒等待的线程只是再次阻塞（有关详细信息，请参阅notify_one）
	lk.unlock();
	cv.notify_one();
}

int main() {
	thread worker(worker_thread);
	
	a_data = "Example data";
	//将数据发送到工作线程
	{
		lock_guard lk(m);
		ready = true;
		cout << "main() signals data ready for processing\n";
	}
	cv.notify_one();

	//等待工人
	{
		unique_lock lk(m);
		cv.wait(lk, [] {return processed; });
	}
	cout << "Back in main(), data = " << a_data << '\n';

	worker.join();
}
