#include<iostream>
#include<mutex>
#include<cstdio>

class Open_error :public std::runtime_error {
public:
	explicit Open_error(int errno_)		//explicit 是 C++ 里一个用来限制隐式类型转换的关键字，主要用在单参数的构造函数上，
		: std::runtime_error("File open failed"),errno_(errno_){}		//这个 : 后面的东西叫「成员初始化列表」，是 C++ 构造函数里专门用来初始化成员 / 父类的语法！

	int error_code() const { return errno_; }

private:
	int errno_;
};

class File_handle {
	FILE* p;
public:
	File_handle(const char* n, const char* a) {
		p = fopen(n, a);
		if (!p) throw Open_error(errno);		//似乎像是笔误了
	}
	File_handle(FILE* pp) {
		p = pp;
		if (!p) throw Open_error(errno);		//这里好像也是笔误了
	}
	~File_handle() {
		fclose(p);
	}
	
	operator FILE* () { return p; }
	//...
};

void f(const char* fn) {
	File_handle f(fn, "rw");		//open fn for reading and writing
	//use file throught f
}

int main() {
	try {
		File_handle fh("test.txt", "w");
		fprintf(fh, "Hello RAII!\n");
	}
	catch (const Open_error& e) {
		printf("Error open file:%d\n", e.error_code());
	}
	return 0;
}