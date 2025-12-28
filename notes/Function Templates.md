# Function Templates

## 一、模板函数（Template Functions）
### 1. 核心动机

- 问题背景：传统函数重载支持多类型（如`int`、`double`、`string`）的相同逻辑时，需重复编写代码（如多个`min`函数），冗余且维护成本高。
- 核心目标：**复用函数逻辑，适配任意类型**，由编译器自动生成特定类型的函数实现，避免手动重载。

### 2. 基础语法与核心概念

#### （1）声明与定义
- 语法格式：`template <typename T> 返回类型 函数名(参数列表)`，其中`T`为类型参数（可替换为任意支持函数逻辑的类型）。
  ```cpp
  // 基础版本：返回两值中的较小者
  template <typename T>
  T min(const T& a, const T& b) { // 用const&避免拷贝，更高效
    return a < b ? a : b;
  }
  ```
- 关键说明：
  - 类型参数`T`需支持函数内的操作（如`min`函数要求`T`有`operator<`）；
  - `typename`可替换为`class`，含义完全一致。

#### （2）模板函数的调用方式
##### ① 显式实例化（Explicit Instantiation）
- 直接指定类型参数，与模板类调用方式一致，无歧义。
  ```cpp
  min<int>(106, 107);        // 显式指定T=int，返回106
  min<double>(1.2, 3.4);     // 显式指定T=double，返回1.2
  min<std::string>("Thomas", "Rachel"); // 显式指定T=string，返回"Rachel"
  ```
- 适用场景：参数类型模糊时（如`const char*`字符串比较，需显式指定为`string`避免指针比较）。

##### ② 隐式实例化（Implicit Instantiation）
- 编译器自动推导类型参数，无需手动指定，类似`auto`的类型推导。
  ```cpp
  min(106, 107);       // 推导T=int，等价于min<int>(106, 107)
  min(1.2, 3.4);       // 推导T=double，等价于min<double>(1.2, 3.4)
  ```
- 限制：
  - 参数类型需一致（如`min(106, 3.14)`编译报错，`int`与`double`无法推导统一`T`）；
  - 可通过多模板参数+`auto`返回值解决类型不匹配问题：
    ```cpp
    template <typename T, typename U>
    auto min(const T& a, const U& b) { // auto让编译器推导返回类型
      return a < b ? a : b;
    }
    min(106, 3.14); // 合法，返回double类型3.14
    ```

### 3. 实际应用：通用迭代器查找函数
- 问题：STL容器（`vector`、`set`、`map`等）的迭代器类型不同，但遍历、比较逻辑一致，需通用`find`函数。
- 模板函数实现（适配所有支持迭代器的容器）：
  ```cpp
  template <typename Iterator, typename TElem>
  Iterator find(Iterator begin, Iterator end, TElem value) {
    Iterator it = begin;
    while (it != end) {
      if (*it == value) break; // 迭代器解引用获取元素
      ++it;
    }
    return it; // 找到返回迭代器，未找到返回end
  }
  ```
- 调用示例（支持`vector`、`set`等任意容器）：
  ```cpp
  std::vector<int> v = {106, 111, 42, 112};
  auto it1 = find(v.begin(), v.end(), 42); // 找到42的迭代器
  
  std::set<std::string> s = {"house", "targaryen"};
  auto it2 = find(s.begin(), s.end(), "targaryen"); // 找到对应字符串迭代器
  ```
- 关联STL：`std::find`（定义于`<algorithm>`）本质就是该模板函数的标准实现。

## 二、概念（Concepts）—— 模板的类型约束
### 1. 核心问题：模板错误信息不友好
- 传统模板的缺陷：编译器仅在实例化后才检查类型是否支持函数逻辑，错误信息冗长且难以理解。
  示例：用`min`函数比较无`operator<`的自定义类型`StanfordID`：
  ```cpp
  struct StanfordID {};
  StanfordID a, b;
  min(a, b); // 编译报错，但错误信息涉及模板实例化细节，不易定位问题
  ```

### 2. 概念的定义与作用
- 定义：C++20引入的**类型约束机制**，明确模板对类型的要求（如“需支持`<`比较”“需是迭代器类型”），提前拦截不合法类型。
- 本质：命名的约束集合，让模板“声明”所需类型的能力，而非“隐含”要求。

#### （1）自定义概念示例：`Comparable`
```cpp
// 定义Concept：Comparable要求类型T支持a < b，且结果可转为bool
template <typename T>
concept Comparable = requires(const T a, const T b) {
  { a < b } -> std::convertible_to<bool>;
};
```
- 语法解析：
  - `requires`：指定约束条件（括号内为“假设的代码片段”，需能编译通过）；
  - `{ a < b } -> std::convertible_to<bool>`：约束`a < b`的结果可转为`bool`。

#### （2）使用概念约束模板
```cpp
// 方式1：用requires子句
template <typename T> requires Comparable<T>
T min(const T& a, const T& b) { return a < b ? a : b; }

// 方式2：简写（更简洁）
template <Comparable T>
T min(const T& a, const T& b) { return a < b ? a : b; }
```

#### （3）核心优势：清晰的错误提示
当传入不满足`Comparable`的类型时，编译器直接提示“类型不满足约束”，而非实例化后的复杂错误：
```
error: constraints not satisfied for class template 'min' [with T = StanfordID]
note: because 'StanfordID' does not satisfy 'Comparable'
note: because 'a < b' would be invalid: invalid operands to binary expression
```

### 3. 内置概念
C++20提供标准内置概念，覆盖常见场景：
- 迭代器相关：`std::input_iterator`（输入迭代器）、`std::forward_iterator`（前向迭代器）等；
- 类型属性相关：`std::integral`（整数类型）、`std::floating_point`（浮点类型）等；
- 示例：约束`find`函数的迭代器类型：
  ```cpp
  template <std::input_iterator It, typename T>
  It find(It begin, It end, const T& value); // 仅接受输入迭代器类型
  ```

## 三、可变参数模板（Variadic Templates）
### 1. 核心动机
- 问题：传统模板函数仅支持固定数量的参数（如`min`仅支持2个参数），无法适配任意数量的参数（如`min(2.4, 7.5, 5.3, 1.2)`）。
- 核心目标：**支持任意数量、任意类型的函数参数**，由编译器通过递归自动生成重载函数。

### 2. 基础语法与核心概念
#### （1）关键语法元素
- 参数包（Parameter Pack）：`typename... Args`（类型参数包）、`const Args&... args`（值参数包），表示“0个或多个类型/参数”；
- 包展开（Pack Expansion）：`args...`，将参数包展开为单个参数列表；
- 递归结构：需定义“基础案例”（终止递归）和“递归案例”（处理参数包）。

#### （2）示例：支持任意参数的`min`函数
```cpp
// 基础案例：1个参数（终止递归）
template <Comparable T>
T min(const T& v) { return v; }

// 递归案例：1个固定参数 + 参数包（处理任意数量额外参数）
template <Comparable T, Comparable... Args>
T min(const T& v, const Args&... args) {
  auto m = min(args...); // 展开参数包，递归调用min
  return v < m ? v : m;  // 比较当前参数与剩余参数的最小值
}
```

#### （3）参数包展开过程（以`min(2, 7, 5, 1)`为例）
编译器自动生成递归重载函数，过程如下：
1. `min(2, 7, 5, 1)` → 调用`min<int, int, int, int>(2, 7, 5, 1)`；
2. 展开参数包：`args...` = `7, 5, 1`，递归调用`min(7, 5, 1)`；
3. 继续展开：`min(7, 5, 1)` → 递归调用`min(5, 1)`；
4. 继续展开：`min(5, 1)` → 递归调用`min(1)`（基础案例）；
5. 回溯计算：`min(5,1)=1` → `min(7,1)=1` → `min(2,1)=1`，最终返回1。

### 3. 实际应用：通用格式化输出函数（类似Python f-string）
```cpp
// 基础案例：无额外参数（输出剩余格式字符串）
void format(const std::string& fmt) {
  std::cout << fmt << std::endl;
}

// 递归案例：替换一个{}，递归处理剩余参数
template <typename T, typename... Args>
void format(const std::string& fmt, T value, Args... args) {
  auto pos = fmt.find("{}"); // 找到第一个{}
  if (pos == std::string::npos) throw std::runtime_error("多余参数");
  std::cout << fmt.substr(0, pos) << value; // 输出{}前的内容+当前值
  format(fmt.substr(pos + 2), args...); // 递归处理剩余格式字符串和参数
}
```
- 调用示例：
  ```cpp
  format("Lecture {}: {} (Week {})", 9, "Templates", 5); 
  // 输出：Lecture 9: Templates (Week 5)
  format("Queen {}, Protector of the {} Kingdoms", "Rhaenyra", 7);
  // 输出：Queen Rhaenyra, Protector of the 7 Kingdoms
  ```

## 四、模板元编程（Template Metaprogramming, TMP）
### 1. 核心思想
- 定义：利用模板的编译期实例化特性，在**编译期执行代码、计算结果**，而非运行期。
- 本质：模板是“编译期的函数”，模板特化是“分支逻辑”，递归实例化是“循环逻辑”。

### 2. 基础示例：编译期计算阶乘
#### （1）传统TMP实现（基于模板特化+递归）
```cpp
// 递归案例：N >= 1，计算N * Factorial<N-1>::value
template <size_t N>
struct Factorial {
  enum { value = N * Factorial<N - 1>::value }; // enum存储编译期常量
};

// 基础案例（模板特化）：N=0，阶乘为1
template <>
struct Factorial<0> {
  enum { value = 1 };
};
```
- 调用与编译结果：
  ```cpp
  std::cout << Factorial<7>::value; // 输出5040，编译期已计算完成
  ```
- 底层：编译器实例化`Factorial<7>`到`Factorial<0>`的所有模板，直接将结果`5040`写入汇编代码，运行期无计算开销。

#### （2）现代简化方案：`constexpr`/`consteval`（C++20+）
传统TMP语法繁琐，C++20引入`constexpr`（尽量编译期执行）和`consteval`（必须编译期执行）简化开发：
```cpp
// constexpr：优先编译期执行，支持运行期调用
constexpr size_t factorial(size_t n) {
  return n == 0 ? 1 : n * factorial(n - 1);
}

// consteval：强制编译期执行，仅允许编译期常量调用
consteval size_t factorial_consteval(size_t n) {
  return n == 0 ? 1 : n * factorial_consteval(n - 1);
}
```
- 调用示例：
  ```cpp
  std::cout << factorial(7);          // 编译期计算，输出5040
  std::cout << factorial_consteval(7); // 编译期计算，输出5040
  ```

### 3. 核心特性
- 图灵完备：支持分支、循环、递归，可执行任意复杂的编译期计算；
- 应用场景：编译期常量计算、类型推导、静态断言（`static_assert`）等；
- 注意：传统TMP语法晦涩，优先使用`constexpr`/`consteval`（可读性更强）。

## 五、核心总结
### 1. 各特性核心价值与使用场景
| 特性               | 核心价值                                  | 典型场景                          |
|--------------------|-------------------------------------------|-----------------------------------|
| 模板函数           | 复用函数逻辑，适配多类型                  | 通用工具函数（`min`、`find`）、STL算法 |
| 概念（Concepts）   | 约束模板类型，优化错误提示                | 提升模板可读性、降低调试成本      |
| 可变参数模板       | 支持任意数量/类型的参数，自动生成重载      | 格式化输出（`format`）、多参数工具函数 |
| 模板元编程（TMP）  | 编译期执行代码，无运行期开销              | 编译期常量计算、静态类型检查      |

### 2. 关键注意事项
- 模板的核心是“编译器自动代码生成”，实例化发生在编译期；
- 概念是C++20新特性，STL尚未完全支持，需注意编译器版本；
- 可变参数模板依赖递归结构，需明确“基础案例”避免无限递归；
- TMP优先使用`constexpr`/`consteval`，传统模板特化仅用于兼容旧标准。
