# std::optional & Type Safety

## 一、回顾（Recap）
### 1. 移动语义（Move Semantics）
- **核心目的**：当原对象的资源不再被需要时，直接转移资源而非拷贝，提升性能。
- **关键操作**：用 `std::move(x)` 将左值（l-value）转为右值（r-value），触发移动构造/赋值，原对象会被置为“有效但未指定”状态（如 `vector` 移动后大小为 0）。
- **示例**：
  ```cpp
  std::vector<int> a = {1,2,3};
  std::vector<int> b = std::move(a); // 转移a的资源，a.size()变为0
  ```

### 2. 三大核心规则（零/三/五规则）
#### （1）零规则（Rule of Zero）
- **适用场景**：类成员为“自管理类型”（如 `std::string`、`std::vector`），无需自定义构造、析构、拷贝/移动相关函数。
- **原理**：编译器自动生成的析构、拷贝构造、拷贝赋值、移动构造、移动赋值函数可正确管理资源，无需手动实现。
- **示例**：
  ```cpp
  class Student {
  public:
    Student(std::string name, std::vector<int> scores) 
      : name_(std::move(name)), scores_(std::move(scores)) {}
  private:
    std::string name_;    // 自管理成员
    std::vector<int> scores_; // 自管理成员
  };
  ```

#### （2）三规则（Rule of Three）
- **适用场景**：类包含“原始指针”等非自管理资源（需手动释放），自定义了析构函数时。
- **要求**：必须同时自定义 **拷贝构造函数** 和 **拷贝赋值运算符**，避免浅拷贝导致的内存泄漏/重复释放。
- **核心操作**：拷贝时需“深拷贝”资源（如为新对象分配独立内存并复制数据），赋值时需先释放自身旧资源，再深拷贝，且避免自赋值。

#### （3）五规则（Rule of Five）
- **适用场景**：已自定义拷贝构造和拷贝赋值运算符时。
- **要求**：应额外定义 **移动构造函数** 和 **移动赋值运算符**，利用移动语义提升性能（避免不必要的深拷贝）。
- **核心操作**：移动时直接接管源对象的资源指针，将源对象指针置空（避免释放冲突）。

## 二、类型安全（Type Safety）
### 1. 定义
指语言“防止类型错误”并“保证程序行为可预测”的程度——类型安全的代码在编译/运行时能规避无效类型操作或不可预期行为。

### 2. 语言对比（Python vs C++）
| 特性                | Python（动态类型）                | C++（静态类型）                  |
|---------------------|-----------------------------------|---------------------------------|
| 类型检查时机        | 运行时                            | 编译时                          |
| 错误表现            | 调用 `div_3("hello")` 运行时崩溃  | 调用 `div_3("hello")` 编译报错  |
| 核心优势            | 灵活                              | 安全、可预测                    |

### 3. 典型问题：未定义行为（UB）的陷阱
以 `std::vector::back()` 为例：
- **函数签名**：`valueType& vector<valueType>::back()`，承诺返回 `valueType&`。
- **风险**：当 vector 为空时，调用 `back()` 会触发未定义行为（崩溃、返回垃圾值等）——签名的“虚假承诺”导致类型不安全。
- **本质原因**：函数无法通过签名告知“可能无返回值”，需程序员手动保证前置条件（vector 非空）。

### 4. 早期解决方案的缺陷（std::pair）
为解决“可能无返回值”问题，曾用 `std::pair<bool, valueType&>` 封装结果（`bool` 标识是否有值），但存在三大问题：
1. `valueType` 可能无默认构造函数（空值时无法创建默认对象）；
2. 默认构造函数可能开销巨大；
3. 默认值可能导致逻辑错误（如 `int` 默认值为 0，若业务需区分“无值”和“值为 0”则失效）。

## 三、std::optional：类型安全的“可选值”解决方案
### 1. 核心定义
- **本质**：C++17 引入的模板类（头文件 `<optional>`），用于表示“可能包含 T 类型值，或不包含任何值”的场景。
- **空值标识**：用 `std::nullopt` 表示空值（≠ `nullptr`：`nullptr` 用于指针，`std::nullopt` 仅用于 `std::optional`）。
- **核心优势**：
  - 明确表达“可选值”意图，比 `std::pair` 更易读；
  - 避免默认构造的开销（空 `optional` 不构造 `T` 对象）；
  - 支持昂贵构造的 `T` 类型（仅在有值时构造）。

### 2. 基础用法与接口
#### （1）创建与初始化
```cpp
std::optional<int> a = 5;          // 有值：5
std::optional<int> b = std::nullopt; // 空值
std::optional<int> c = {};         // 空值（与 std::nullopt 等价）
```

#### （2）核心接口
| 接口                | 功能描述                                  | 注意事项                              |
|---------------------|-------------------------------------------|---------------------------------------|
| `has_value()`       | 返回 `bool`，标识是否包含有效值            | 无值时返回 `false`                    |
| `value()`           | 返回包含的有效值                          | 无值时抛出 `bad_optional_access` 异常  |
| `value_or(default)` | 有值返回有效值，无值返回默认值 `default`   | 不抛出异常，默认值需为 `T` 类型        |
| 直接解引用 `*opt`   | 同 `value()`，但无值时触发未定义行为       | 慎用，需先通过 `has_value()` 检查     |

#### （3）示例代码
```cpp
#include <optional>
#include <iostream>

int main() {
  std::optional<int> a = 5;
  std::optional<int> b = std::nullopt;

  std::cout << a.has_value(); // 1（true）
  std::cout << b.has_value(); // 0（false）

  std::cout << a.value();     // 5
  // std::cout << b.value();  // 抛异常 bad_optional_access

  std::cout << a.value_or(999); // 5
  std::cout << b.value_or(999); // 999（无值时用默认值）
  return 0;
}
```

### 3. 高级操作（Monadic 接口）
`std::optional` 提供三个链式操作接口，简化“可选值的连续处理”，避免嵌套判断：
#### （1）`and_then(function f)`
- **功能**：若 `optional` 有值，调用函数 `f`（`f` 需返回 `optional` 类型），返回 `f` 的结果；无值则返回 `std::nullopt`。
- **示例**：
  ```cpp
  std::optional<int> half(int x) {
    return x % 2 == 0 ? std::optional(x/2) : std::nullopt;
  }
  std::optional<int> a = 8;
  auto result = a.and_then(half).and_then(half).and_then(half); // 8→4→2→1
  std::cout << *result; // 1
  ```

#### （2）`transform(function f)`
- **功能**：若 `optional` 有值，调用函数 `f`（`f` 返回普通类型），返回包含 `f` 结果的 `optional`；无值则返回 `std::nullopt`。
- **示例**：
  ```cpp
  int square(int x) { return x*x; }
  std::optional<int> x = 5;
  auto y = x.transform(square); // optional(25)
  ```

#### （3）`or_else(function f)`
- **功能**：若 `optional` 有值，返回自身；无值则调用函数 `f`（`f` 需返回 `optional` 类型），返回 `f` 的结果（兜底逻辑）。
- **示例**：
  ```cpp
  std::optional<int> fallback() { return 42; }
  std::optional<int> bad = std::nullopt;
  auto r2 = bad.or_else(fallback); // optional(42)
  ```

### 4. 关键限制
- **不支持 `std::optional<T&>`**：引用必须指向有效对象，而 `std::optional` 可能为空，矛盾。因此 `vector::operator[]` 无法返回 `optional`，只能用 `at()` 抛 `out_of_range` 异常保证安全。
- **仍可能触发未定义行为**：直接解引用空 `optional`（`*opt`）会导致 UB，需先通过 `has_value()` 或布尔判断（`if (opt)`）检查。

## 四、std::optional 的优缺点与适用场景
### 1. 优点
- **类型安全**：函数签名明确告知“可能无返回值”，避免“虚假承诺”，减少未定义行为。
- **行为可预测**：无值时通过 `value_or` 或 `or_else` 提供兜底逻辑，替代手动检查前置条件。
- **性能友好**：空 `optional` 不构造 `T` 对象，避免默认构造的开销。

### 2. 缺点
- **易用性成本**：需频繁通过 `value()` 或 `has_value()` 访问值，代码略显繁琐。
- **异常风险**：`value()` 访问空 `optional` 会抛异常，需手动处理。
- **无引用支持**：无法用 `std::optional<T&>` 封装“可选引用”，限制部分场景使用。

### 3. 语言设计权衡
- C++ 标准库（如 `vector::back()`）未采用 `optional` 返回，因需兼顾性能（无额外封装开销）和向后兼容。
- Rust、Swift 等语言将 `optional` 作为核心特性，因更注重安全优先；C++ 则保留灵活性，允许程序员在“安全”和“性能”间选择。

## 五、总结
1. **类型安全的核心**：通过严格的类型系统和明确的函数签名，保证程序行为可预测，规避未定义行为。
2. **`std::optional` 的价值**：优雅解决“可能有值或无值”的场景，比 `std::pair` 更安全、易用，支持链式操作简化逻辑。
3. **实践建议**：在业务代码中优先使用 `std::optional` 表示可选返回值（如数据库查询结果、解析结果），提升代码可读性和安全性；避免在性能敏感场景过度使用（如高频调用的底层函数）。