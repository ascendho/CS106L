# Functions & Lambdas

## 一、谓词（Predicate）与通用查找函数（find_if）
### 1. 谓词的定义与分类
- 核心定义：返回布尔值（`bool`）的函数，用于描述“判断条件”，是算法与自定义逻辑结合的桥梁。
- 分类：
  - 一元谓词：接收1个参数（如`bool isVowel(char c)`判断字符是否为元音）；
  - 二元谓词：接收2个参数（如`bool isLessThan(int x, int y)`判断x是否小于y）。
- 示例：
  ```cpp
  // 一元谓词：判断是否为质数
  bool isPrime(size_t n) {
    if (n < 2) return false;
    for (size_t i = 3; i <= sqrt(n); i++) if (n % i == 0) return false;
    return true;
  }
  ```

### 2. 从find到find_if：支持通用条件查找
- 传统`find`的局限：仅能查找“特定值”（如`find(v.begin(), v.end(), 42)`），无法支持自定义条件（如“查找第一个质数”）。
- 通用化改造：将固定值判断替换为“谓词参数”，形成`find_if`函数，适配任意判断逻辑。
- 模板实现：
  ```cpp
  template <typename It, typename Pred> // Pred：谓词类型（编译器隐式推导）
  It find_if(It first, It last, Pred pred) {
    for (auto it = first; it != last; ++it) {
      if (pred(*it)) return it; // 调用谓词判断当前元素是否符合条件
    }
    return last; // 未找到返回end迭代器
  }
  ```

### 3. 函数指针的局限
- 函数指针是谓词的早期实现方式（如`find_if`接收`bool(*)(char)`类型的函数指针），但存在明显缺陷：
  1. 无法携带额外状态（如“查找小于N的元素”，N是运行时输入，函数指针无法直接使用）；
  2. 需为不同参数编写重复函数（如`lessThan5`、`lessThan6`），冗余且不灵活。


## 二、Lambda表达式：带状态的匿名谓词
### 1. 核心作用
解决函数指针的局限，允许在代码中“即时定义匿名函数”，并可捕获外部作用域的变量（携带状态），是C++中“行为即变量”的核心实现。

### 2. 语法结构
```cpp
auto 变量名 = [捕获子句](参数列表) { 函数体 };
```
- 捕获子句：指定要从外部作用域捕获的变量（核心区别于普通函数）；
- 参数列表：与普通函数一致，支持`auto`（隐含模板参数，编译器自动推导类型）；
- 函数体：逻辑实现，可使用捕获的变量和参数。

### 3. 关键特性：捕获子句（Capture Clause）
| 捕获方式       | 语法          | 含义                                  |
|----------------|---------------|---------------------------------------|
| 值捕获         | `[x]`         | 捕获变量x，拷贝到lambda内部（只读，需修改加`mutable`） |
| 引用捕获       | `[x&]`        | 捕获变量x的引用，可修改外部变量        |
| 多个捕获       | `[x, y&]`     | 捕获x（值）、y（引用）                |
| 默认值捕获     | `[=]`         | 捕获所有外部变量（值拷贝）            |
| 默认引用捕获   | `[&]`         | 捕获所有外部变量（引用）              |
| 混合默认捕获   | `[&, x]`      | 默认引用捕获，x单独值捕获             |

- 示例：解决“查找小于运行时输入N的元素”问题
  ```cpp
  int n;
  std::cin >> n;
  // 捕获n（值拷贝），参数x为auto（适配任意可比较类型）
  auto lessThanN = [n](auto x) { return x < n; };
  // 调用find_if，传递lambda作为谓词
  auto it = find_if(v.begin(), v.end(), lessThanN);
  ```

### 4. 底层实现：Lambda本质是Functor（函数对象）
- 编译器会自动将lambda表达式转换为一个匿名的`functor`（重载`operator()`的类）：
  - 捕获的变量成为该类的成员变量；
  - `operator()`实现lambda的函数体逻辑。
- 等价转换示例：
  ```cpp
  // Lambda代码
  int n = 10;
  auto lessThanN = [n](int x) { return x < n; };
  
  // 编译器生成的等价Functor
  class __lambda_anonymous {
  private:
    int n; // 捕获的变量作为成员
  public:
    __lambda_anonymous(int _n) : n(_n) {} // 构造函数初始化捕获变量
    bool operator()(int x) const { return x < n; } // 重载operator()
  };
  auto lessThanN = __lambda_anonymous(n);
  ```

### 5. 统一类型：std::function
- 作用：作为函数、lambda、functor的“统一容器类型”，可存储任意可调用对象（函数指针、lambda、functor）。
- 语法：`std::function<返回类型(参数类型...)>`
- 示例：
  ```cpp
  #include <functional>
  std::function<bool(int)> pred1 = isPrime; // 存储函数指针
  std::function<bool(int)> pred2 = [](int x) { return x < 5; }; // 存储lambda
  std::function<bool(int, int)> pred3 = std::less<int>{}; // 存储STL functor
  ```
- 局限：存在轻微性能开销，日常使用优先用`auto`或模板推导类型（更高效）。


## 三、STL算法：基于模板与谓词的通用工具
### 1. STL算法的核心思想
- STL四大组件：容器（存储数据）、迭代器（遍历数据）、functor/lambda（描述行为）、算法（处理数据）；
- 算法特性：
  1. 模板实现，通用适配所有容器（依赖迭代器接口）；
  2. 接收谓词/操作函数，支持自定义逻辑；
  3. 定义于`<algorithm>`头文件，多数函数操作迭代器范围`[first, last)`。

### 2. 常用STL算法示例
| 算法名         | 功能                                  | 示例场景                          |
|----------------|---------------------------------------|-----------------------------------|
| `std::find_if` | 查找第一个满足谓词的元素              | 查找第一个元音、第一个质数        |
| `std::count_if`| 统计满足谓词的元素个数                | 统计字符串中元音的数量            |
| `std::sort`    | 排序，支持自定义比较谓词              | 降序排序（`std::greater<int>{}`） |
| `std::transform`| 对元素应用操作函数，生成新序列        | 所有字符转为大写                  |
| `std::copy_if` | 仅拷贝满足谓词的元素                  | 提取字符串中的所有字母            |
| `std::unique_copy` | 拷贝时去除连续重复元素              | 合并相邻重复编码（如Soundex）     |

### 3. 实践：用STL算法实现Soundex编码
Soundex是姓名的语音编码算法，核心步骤6步，仅需3个STL算法即可实现：
```cpp
#include <algorithm>
#include <vector>
#include <string>

// 步骤1：提取字符串中的所有字母
std::string letters;
std::copy_if(s.begin(), s.end(), std::back_inserter(letters), ::isalpha);

// 步骤2：将字母转换为Soundex编码（如B→1、C→2）
std::transform(letters.begin(), letters.end(), letters.begin(), soundexEncode);

// 步骤3：去除相邻重复编码（如222→2）
std::string unique;
std::unique_copy(letters.begin(), letters.end(), std::back_inserter(unique));

// 步骤4-6：替换首字符、去除0、补全/截断为4位（略）
```
- 核心优势：算法复用性强，无需手动编写循环，代码简洁且通用。


## 四、Ranges与Views：C++20+的STL升级
### 1. Ranges：简化迭代器传递
- 传统STL算法的痛点：需手动传递`begin()`和`end()`迭代器，代码冗余（如`std::find(v.begin(), v.end(), 42)`）。
- Ranges的改进：
  1. 直接接收容器（或范围）作为参数，无需显式迭代器；
  2. 基于Concepts约束，错误提示更清晰；
  3. 兼容原有迭代器（需操作子范围时仍可使用）。
- 示例：
  ```cpp
  #include <ranges>
  std::vector<char> v = {'a', 'b', 'c', 'd', 'e'};
  auto it1 = std::ranges::find(v, 'c'); // 直接传容器，简洁
  auto it2 = std::ranges::find(v.begin()+1, v.end()-1, 'c'); // 兼容迭代器，支持子范围
  ```

### 2. Views：惰性求值的可组合范围
- 核心定义：Views是“适配现有范围的轻量级视图”，具有两大特性：
  1. 惰性求值：仅在需要时才处理元素（避免中间容器拷贝，高效）；
  2. 可组合性：通过`|`运算符链式调用，形成数据处理流水线。
- 常用Views：
  - `views::filter(Pred)`：过滤满足谓词的元素；
  - `views::transform(Op)`：对元素应用操作函数；
  - `views::unique`：去除连续重复元素；
  - `views::take(N)`：取前N个元素。

### 3. Views的优势：简洁高效的链式编程
- 传统STL实现“过滤+转换”（如元音转大写）：
  ```cpp
  std::vector<char> f, t;
  std::copy_if(v.begin(), v.end(), std::back_inserter(f), isVowel); // 过滤
  std::transform(f.begin(), f.end(), std::back_inserter(t), toupper); // 转换
  ```
- Views链式实现（无中间容器，惰性求值）：
  ```cpp
  auto upperVowel = v
    | std::ranges::views::filter(isVowel) // 过滤元音
    | std::ranges::views::transform(toupper) // 转为大写
    | std::ranges::to<std::vector<char>>(); //  materialize为容器（触发实际计算）
  ```
- 类比：类似Python的生成器表达式，仅在最终转换为容器时才执行计算，性能更优。

### 4. Ranges/Views的局限
- 新特性：依赖C++20及以上标准，部分编译器支持不完全；
- 性能：复杂场景下可能略逊于手写优化代码；
- 兼容性：旧项目迁移成本较高，需升级编译器和标准库。


## 五、核心总结
### 1. 关键技术链路
模板函数（通用逻辑）→ 谓词（自定义判断）→ Lambda（带状态谓词）→ STL算法（通用数据处理）→ Ranges/Views（简化语法+高效组合）。

### 2. 各组件核心价值
| 组件         | 核心价值                                  | 适用场景                          |
|--------------|-------------------------------------------|-----------------------------------|
| 谓词         | 定义判断逻辑，衔接算法与自定义需求        | 所有需要“条件筛选”的算法（find_if、count_if） |
| Lambda       | 匿名带状态函数，简化谓词定义              | 临时使用的短逻辑（无需单独定义函数）        |
| STL算法      | 复用循环逻辑，避免重复编码                | 排序、查找、转换、拷贝等通用数据处理        |
| Ranges       | 简化迭代器传递，提升代码可读性            | 日常容器操作（无需子范围时）                |
| Views        | 惰性组合处理，减少中间拷贝                | 多步骤数据转换（过滤+转换+去重等）          |

### 3. 实践建议
- 优先使用Lambda替代独立函数（短逻辑场景），用`auto`推导类型（高效）；
- 数据处理优先选用STL算法（`<algorithm>`），避免手动循环；
- 新项目（C++20+）可尝试Ranges/Views，简化代码并提升效率；
- 复杂场景需兼容旧标准时，仍可使用传统STL+Lambda组合。

### 4. 后续展望
Ranges/Views是C++未来的发展方向，后续标准（如C++23/26）将持续完善其功能，进一步提升C++的函数式编程能力和代码简洁性。