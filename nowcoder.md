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



## 以下程序的输出是:

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



