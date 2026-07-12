# 算法



## 辗转相除法

```c++
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}
```

用于快速求出最大公约数

### 核心思想：用余数替换大数

这个算法基于一个很巧的规律：

> **如果 `a ÷ b` 的余数是 `r`，那么 `a` 和 `b` 的最大公约数，就等于 `b` 和 `r` 的最大公约数。**

写成公式：`gcd(a, b) = gcd(b, a % b)`

为什么？你可以这样理解：

- 假设有一个数 `d` 能同时整除 `a` 和 `b`。
- 因为 `a = b × 商 + r`，所以 `r = a - b × 商`。
- 那 `d` 一定能整除 `r`（因为 `d` 能整除 `a` 和 `b`，那它们的线性组合也能整除）。
- 反过来，如果 `d` 能整除 `b` 和 `r`，它也能整除 `a`。
- 所以 `(a,b)` 的公约数和 `(b,r)` 的公约数完全一样，最大公约数自然也一样。

这样，我们就用一个**更小的数** `r` 去替换原来较大的数 `a`，问题规模越来越小。







# 错题



## 模板：主模板与特化模板

以下程序的输出是:

```c++
#include <iostream>
using namespace std;
template <typename T>
void print(T t){
    cout<<"The value is "<<t<<endl;
}
template <>
void print<char *>(char* c){
    cout<<"The string is " << c <<endl;
}
int main(){
    char str[] = "TrendMicro[char]";
    unsigned char ustr[] = "TrendMicro[unsigned char]";
    print(str);
    print(ustr);
    return 0;
}
```

答案：The string is TrendMicro[char] The value is TrendMicro[unsigned char]

对 `char[]` 衰减为 `char*`，命中函数模板的显式特化 `print<char*>(char*)`，输出“The string is …”。
对 `unsigned char[]` 衰减为 `unsigned char*`，无对应特化，调用主模板，输出“The value is …”。

注意：*特化版本**必须**依附于主模板存在。编译器在调用时，会优先查看有没有匹配的特化版本；如果没有，才去实例化主模板。*



## 隐式转换：int类型向 类类型对象的隐式转换

有如下程序段：

```c++
#include <stdio.h>
class A {
  public:
    A() { printf("0"); }
    A(int a) { printf("1"); }
    A &operator=(const A &a) {
        printf("2");
        return (*this);
    }
};
int main() {
    A al;
    al = 10;
}
```

则程序输出是：

答案：**012**

程序执行过程分析：

1. A al; 调用默认构造函数，输出"0"
2. al=10; 这条语句会有两个步骤：

- 首先会调用A(int a)构造函数创建一个临时对象，输出"1"
-  然后调用赋值运算符operator=将临时对象赋值给al，输出"2"

补充说明：
*这里的al=10;语句实际上包含了隐式类型转换，编译器会先将整数10转换为A类型的临时对象，再进行赋值操作。这是C++中常见的对象赋值过程，涉及到临时对象的创建和赋值运算符的调用。理解这个过程对于掌握C++的对象操作非常重要。*



## strcpy

strcpy执行时，会自动在复制后的字符串上添加 '\0'

下面代码的执行结果是什么：

```c++
char ccString1[]="Is Page Fault??";
char ccString2[]="No Page Fault??";
strcpy(ccString1,"No");
if(strcmp(ccString1,ccString2)==0)
   cout<<ccString2;
else
   cout<<ccString1;
```

答案：**No**

strcpy执行时，会自动在复制后的字符串上添加‘\0’，所以复制完成之后ccString1变为 "No\0Page Fault??"  

strcmp 比较两个字符串，设这两个字符串为str1，str2

- 若str1==str2，则返回零； 

- 若str1>str2，则返回正数； 

- 若str1<str2，则返回负数

由于比较结果不相等，执行else分支，所以答案是**"No"**
