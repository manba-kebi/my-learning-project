#include <iostream>
#include <mutex>
using namespace std;

std::mutex m;

void f() {
	cout << "f() called" << endl;
}

bool everything_ok() {
	return true;
}

void bad() {
	m.lock();
	f();
	if (!everything_ok()) {
		return;
	}
	m.unlock();
}

void good() {
	std::lock_guard<std::mutex> lk(m);
	f();
	if (!everything_ok()) {
		return;
	}
}

int main() {
	bad();
	good();
	return 0;
}
