# GDB

## gdb 的命令：

- run — r                  运行一下程序
- quit                        退出GDB模式
- break — b             打断点，可以在函数打断点（函数名字），也可以在某一行打断点
- list                         查看源代码
- info b                     查看在哪里打了断点
- n                            一步一步往下调试
- print — p               打印变量，也可以去打印地址
- step                       进入某一个具体的函数进行调试

## gdb 小技巧

1. shell  去调用我们终端的命令
2. 日志功能
   1. set logging on
3. watchpoint       观察变量是否变化
   1. info watchpoints           可以用info来看我们的watchpoint

## core文件

当程序运行出错时、程序挂掉时，可以生成core文件去调试，例如`gdb ./a.out core_pattern`

core文件不会默认生成，资源设置在ulimit里，可以通过`ulimit -a`去查看（养成用man的习惯，输入`/要查找内容`，可以查到要查找的内容），`ulimit -c unlimited`解除core文件的限制。

设置core文件路径：

- echo "/corefile/core-%e-%p-%t">core_pattern         (这个我用过了，是好用的)
- 或者 echo "./core-%e-%p-%s">/proc/sys/kernel/core_pattern 体

## 调一个正在运行的程序

test_for.c:

```jsx
#include <stdio.h>

//test test1
//i i++
//call test test1
void test(){

}

void test1(){
	int i = 0;
	i++;
}
int main(){
	for(;;){
		test();
		test1();
	}
	return 0;
}
```

gcc -g test_for.c

./a.out &             后台运行

在调试正在运行的程序前，需要执行该命令临时修改ptrace限制：

```jsx
sudo sysctl kernel.yama.ptrace_scope=0;
```

gdb -p pid          其中pid是进程号

ps aux | grep a.out       这是用来看有关a.out的进程都有哪些的

kill pid                关闭该进程





## ulimit 命令（来自于[菜鸟教程](https://www.runoob.com/linux/linux-comm-ulimit.html)）

Linux ulimit命令用于控制shell程序的资源。

ulimit为shell内建指令，可用来控制shell执行程序的资源。

### 语法

```
ulimit [-aHS][-c <core文件上限>][-d <数据节区大小>][-f <文件大小>][-m <内存大小>][-n <文件数目>][-p <缓冲区大小>][-s <堆叠大小>][-t <CPU时间>][-u <程序数目>][-v <虚拟内存大小>]
```

**参数**：

- -a 　显示目前资源限制的设定。
- -c <core文件上限> 　设定core文件的最大值，单位为区块。
- -d <数据节区大小> 　程序数据节区的最大值，单位为KB。
- -f <文件大小> 　shell所能建立的最大文件，单位为区块。
- -H 　设定资源的硬性限制，也就是管理员所设下的限制。
- -m <内存大小> 　指定可使用内存的上限，单位为KB。
- -n <文件数目> 　指定同一时间最多可开启的文件数。
- -p <缓冲区大小> 　指定管道缓冲区的大小，单位512字节。
- -s <堆叠大小> 　指定堆叠的上限，单位为KB。
- -S 　设定资源的弹性限制。
- -t <CPU时间> 　指定CPU使用时间的上限，单位为秒。
- -u <程序数目> 　用户最多可开启的程序数目。
- -v <虚拟内存大小> 　指定可使用的虚拟内存上限，单位为KB。

### 实例

显示系统资源的设置

```
[root@runoob ~]# ulimit -a
core file size     (blocks, -c) 0
data seg size      (kbytes, -d) unlimited
file size        (blocks, -f) unlimited
pending signals         (-i) 1024
max locked memory    (kbytes, -l) 32
max memory size     (kbytes, -m) unlimited
open files           (-n) 1024
pipe size      (512 bytes, -p) 8
POSIX message queues   (bytes, -q) 819200
stack size       (kbytes, -s) 10240
cpu time        (seconds, -t) unlimited
max user processes       (-u) 4096
virtual memory     (kbytes, -v) unlimited
file locks           (-x) unlimited
[root@runoob ~]# 
```

设置单一用户程序数目上限

```
[root@runoob ~]# ulimit -u 500 //设置单一用户程序上限
[root@runoob ~]# ulimit -a
core file size     (blocks, -c) 0
data seg size      (kbytes, -d) unlimited
file size        (blocks, -f) unlimited
pending signals         (-i) 1024
max locked memory    (kbytes, -l) 32
max memory size     (kbytes, -m) unlimited
open files           (-n) 1024
pipe size      (512 bytes, -p) 8
POSIX message queues   (bytes, -q) 819200
stack size       (kbytes, -s) 10240
cpu time        (seconds, -t) unlimited
max user processes       (-u) 500
virtual memory     (kbytes, -v) unlimited
file locks           (-x) unlimited
[root@runoob ~]# 
```