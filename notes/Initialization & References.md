1. 初始化：在构造（对象）时提供初始值
1. 直接初始化，如 `int numOne =  12.0;`，这条语句不会报错，因为以这种方式初始化变量时，编译器不会做类型检查
1. 统一初始化，如 `int numOne{12.0};`（注意是花括号），这条语句会编译错误，因为编译器做了类型检查！

>  **安全性高**！它不允许发生窄化转换 —— 而窄化转换可能导致意外行为（甚至引发严重的系统故障 ）
>  **通用性极强**：它适用于所有类型，例如 vector 容器、map 容器、自定义类等诸如此类的类型！

4. 结构化绑定是一种从**编译期固定大小**的数据结构中初始化部分变量的实用方法，能够访问函数返回的多个值，可用于**大小在编译期已知**的对象。如下：

```c++
#include <iostream>
#include <tuple>
#include <string>

std::tuple<std::string, std::string, std::string> getClassInfo() {
    std::string className = "CS106L";
    std::string buildingName = "260-113";
    std::string language = "C++";
    return {className, buildingName, language};
 }

int main() {
    // 结构化绑定
    auto [className, buildingName, language] = getClassInfo();
    
    // 而不必像下面这样
    // auto classInfo = getClassInfo();
    // std::string className = std::get<0>(classInfo);
    // std::string buildingName = std::get<1>(classInfo);
    // std::string language = std::get<2>(classInfo);
    
    std::cout << "Come to " << buildingName << " and join us for " << className
              << " to learn " << language << "!" << std::endl;
    return 0;
 }
```

5. 引用是已存在实体的别名，例如：

```c++
int num = 5;
int& ref = num;
ref = 10;  // Assigning a new value through the reference
std::cout << num << std::endl;  // Output: 10

// `num` 是一个 `int` 类型的变量，而 `ref` 是一个 `int&` 类型的变量
```

来看一个常见的错误：

```c++
void shift(std::vector<std::pair<int, int>>& nums) {
    for (auto [num1, num2] : nums) {
        num1++;
        num2++;
    }
}
```

虽然 `nums` 以引用的身份传入函数，但在 `for` 循环遍历中，`num1` 和 `num2` 是用结构化绑定 **按值拷贝** 的副本，对 `num1++` 和 `num2++` 的操作只是修改了这些**局部副本**，原始的 `nums` 向量中的元素保持不变。如果想要修改原始值，应该这样：

```c++
void shift(std::vector<std::pair<int, int>>& nums) {
    for (auto& [num1, num2] : nums) {  // 注意这里的 & 
        num1++;
        num2++;
    }
}

// 或者这样
void shift(std::vector<std::pair<int, int>>& nums) {
    for (auto& pair : nums) {
        pair.first++;
        pair.second++;
    }
}
```

6. 在下面这段程序中，`int& n` 是一个左值，因为右值是临时的，我们无法通过引用传递右值

```c++
// note the ampersand!
void squareN(int& n) {
    // calculates n to the power of 2
    n = std::pow(n, 2);
}

int main() {
   int num = 5;
   squareN(num);
   
   // 下面会报错
   // squareN(5); 
    
   std::cout << num << std::endl;
   return 0;
}
```

7. `const`：一种用于对象的限定符，声明它们不能被修改。你不能声明指向 `const` 变量的 `非const` 引用（同时，*const 对象也不能调用非 const 成员函数*），例如：

```c++
#include <iostream>
#include <vector>

int main()
{
	// a const vector
	const std::vector<int> const_vec{1, 2, 3};
	std::vector<int>& bad_ref{ const_vec }; // BAD
	
    // 会编译报错
    // 引用本身在 C++ 中天生就不能重新绑定，这与 const 无关
	return 0;
}
```

注意跟 `const 指针`区分，例如：

```c++
// 指向常量的指针
const int* ptr1;        // 指向常量的指针
int const* ptr2;        // 同上，写法不同
// *ptr1 = 10;          // 编译错误！不能通过指针修改值
ptr1 = &some_int;       // 可以，指针本身可以重新指向

// 常量指针
int* const ptr3 = &a;   // 常量指针
*ptr3 = 10;             // 可以，能修改指向的值
// ptr3 = &b;           // 编译错误！指针本身不能重新指向

// 指向常量的常量指针
const int* const ptr4 = &a;  // 既不能修改值，也不能重新指向
// *ptr4 = 10;               // 编译错误！
// ptr4 = &b;                // 编译错误！
```

8. C++ 是一门编译型语言，存在一类名为编译器的计算机程序，几款常用的编译器包括 clang 和 g++
9. 扼要重述：

> 使用统一初始化 —— 它适用于所有类型和对象！
>
> 引用是为变量创建别名的一种方式！
>
> 你只能引用左值！
>
> const 是确保你无法修改变量的一种方式