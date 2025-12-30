# Operator Overloading

## 一、核心回顾与动机
### 1. 前置知识回顾
- 模板类：通过类型参数化复用类逻辑，适配不同数据类型（如`Vector<int>`、`Vector<string>`）。
- Const正确性：通过`const`成员方法承诺不修改对象，保障代码安全性。
- 仿函数（Functors）：重载`operator()`的类，实现“对象即函数”，为算法提供自定义逻辑。
- STL容器要求：如`std::map<K, V>`需`K`类型支持`operator<`，用于元素排序和查找。

### 2. 运算符重载的核心动机
- 直观性：自定义类型（如`Money`、`StanfordID`）使用运算符（`+`、`<`等）比普通函数更易理解，符合自然语义。
  - 反例：`add(Money(100), Money(50))` 不如 `Money(100) + Money(50)` 直观。
- 兼容性：满足STL算法/容器的要求（如`std::min`需类型支持`operator<`，`std::map`需键类型支持比较）。
- 语义传达：运算符能暗示类型的行为特性（如`+`表示“累加”，`<`表示“有序比较”），函数名无法直观体现。

## 二、运算符重载的基础规则
### 1. 可重载的运算符
大部分C++运算符支持重载，常用包括：
- 算术运算符：`+`、`-`、`*`、`/`、`%`、`++`、`--`
- 比较运算符：`==`、`!=`、`<`、`>`、`<=`、`>=`
- 逻辑运算符：`&&`、`||`、`!`
- 赋值运算符：`=`、`+=`、`-=`、`*=`等
- 其他：`[]`、`()`、`<<`（流输出）、`>>`（流输入）等

### 2. 不可重载的运算符（固定语义，禁止自定义）
- 作用域解析符：`::`
- 三元运算符：`? :`
- 成员访问符：`.`
- 指针到成员访问符：`.*`
- 类型相关运算符：`sizeof()`、`typeid()`、`cast()`（如`static_cast`）

### 3. 重载语法核心格式
```cpp
// 返回值类型 关键字operator + 运算符符号 (参数列表)
返回值类型 operator<符号>(参数列表);
```
- 示例：`bool operator<(const StanfordID& other) const;`（成员重载）
- 关键要求：运算符的“操作数个数”不能改变（如`+`必须是二元运算符，`!`必须是一元运算符）。

## 三、运算符重载的两种方式
### 1. 成员函数重载（Member Overloading）
#### （1）语法与特点
- 重载函数声明在类内部，左操作数为当前对象（`this`指针指向），右操作数通过参数传入。
- 示例：`StanfordID`类的`operator<`成员重载
  ```cpp
  // .h文件
  class StanfordID {
  private:
    int idNumber; // 私有成员：学号（用于比较）
  public:
    // 成员重载operator<：左操作数是this，右操作数是other
    bool operator<(const StanfordID& other) const;
  };
  
  // .cpp文件
  bool StanfordID::operator<(const StanfordID& other) const {
    return this->idNumber < other.idNumber; // 按学号比较
  }
  ```

#### （2）优缺点
- 优点：直接访问类的私有成员，无需额外授权。
- 缺点：
  1. 左操作数必须是当前类的对象（无法支持左操作数为非类类型，如`int < StanfordID`）。
  2. 无法用于“不拥有源代码的类”（如为`std::string`和自定义类重载比较运算符）。

### 2. 非成员函数重载（Non-member Overloading）
#### （1）语法与特点
- 重载函数声明在类外部，两个操作数均通过参数传入（左操作数`lhs`，右操作数`rhs`）。
- 示例：`StanfordID`与`std::string`的非成员`operator<`
  ```cpp
  // .h文件
  class StanfordID {
  private:
    std::string sunet;
  public:
    std::string getSunet() const { return sunet; } // 公有访问器
  };
  
  // 非成员重载：StanfordID < string
  bool operator<(const StanfordID& lhs, const std::string& rhs);
  // 非成员重载：string < StanfordID（支持对称性）
  bool operator<(const std::string& lhs, const StanfordID& rhs);
  
  // .cpp文件
  bool operator<(const StanfordID& lhs, const std::string& rhs) {
    return lhs.getSunet() < rhs;
  }
  bool operator<(const std::string& lhs, const StanfordID& rhs) {
    return lhs < rhs.getSunet();
  }
  ```

#### （2）核心优势（STL推荐，更符合C++ idiom）
- 支持左操作数为非类类型（如`operator<(int lhs, const StanfordID& rhs)`）。
- 支持对称性比较（如`string < StanfordID`和`StanfordID < string`均可）。
- 可用于“不拥有源代码的类”（无需修改类定义，直接在外部重载）。

#### （3）访问私有成员的解决方案：`friend`关键字
- 问题：非成员函数默认无法访问类的私有成员。
- 解决方案：在类内部声明该非成员函数为`friend`，授权其访问私有成员。
  ```cpp
  // .h文件
  class StanfordID {
  private:
    int idNumber;
  public:
    // 声明非成员operator<为友元，允许访问私有成员
    friend bool operator<(const StanfordID& lhs, const StanfordID& rhs);
  };
  
  // .cpp文件（无需类作用域，直接定义）
  bool operator<(const StanfordID& lhs, const StanfordID& rhs) {
    return lhs.idNumber < rhs.idNumber; // 直接访问私有成员idNumber
  }
  ```

### 3. 关键注意事项：避免歧义
- 同一运算符不能同时声明“成员重载”和“非成员重载”（针对相同参数类型），否则编译器无法判断调用哪个，导致编译错误。

## 四、运算符重载的最佳实践与规则
### 1. 最小惊讶原则（Principle of Least Astonishment, PoLA）
- 运算符的功能必须符合直觉，不能滥用。
  - 正确：`operator+`用于累加（如`Money`类的金额相加）。
  - 错误：`operator+`用于集合减法（语义混淆，应定义`subtract`函数）。
- 若语义不明确，优先定义普通函数而非重载运算符。

### 2. 互补运算符的实现（规则 of Contrariety）
- 互补运算符（如`!=`与`==`、`>`与`<`）应基于已实现的运算符推导，避免代码冗余。
  ```cpp
  // 先实现operator==
  bool StanfordID::operator==(const StanfordID& other) const {
    return (name == other.name) && (sunet == other.sunet) && (idNumber == other.idNumber);
  }
  // operator!= 基于operator==实现
  bool StanfordID::operator!=(const StanfordID& other) const {
    return !(*this == other); // 直接复用==的逻辑
  }
  ```

### 3. 常用运算符重载示例
#### （1）流输出运算符`<<`（非成员重载，常用）
用于自定义类型的`cout`输出，必须非成员重载（左操作数是`std::ostream`，无法作为成员）。
```cpp
// .h文件
friend std::ostream& operator<<(std::ostream& out, const StanfordID& sid);

// .cpp文件
std::ostream& operator<<(std::ostream& out, const StanfordID& sid) {
  out << "Name: " << sid.name << ", SUNet: " << sid.sunet << ", ID: " << sid.idNumber;
  return out; // 支持链式调用（如cout << sid1 << sid2）
}

// 使用：cout << sid; // 输出格式化信息
```

#### （2）算术运算符`+`（成员/非成员均可）
```cpp
// Money类的operator+（非成员重载，支持对称性）
Money operator+(const Money& lhs, const Money& rhs) {
  return Money(lhs.cents + rhs.cents);
}

// 使用：Money total = Money(100) + Money(50); // total.cents = 150
```

## 五、核心总结
### 1. 运算符重载的核心价值
- 让自定义类型拥有“原生类型般的直观操作”，提升代码可读性。
- 满足STL容器/算法的要求，实现类型兼容。
- 传达类型的语义特性（如`+`暗示可累加，`<`暗示可排序）。

### 2. 两种重载方式的选择
| 方式               | 适用场景                          | 优点                                  | 缺点                                  |
|--------------------|-----------------------------------|---------------------------------------|---------------------------------------|
| 成员函数重载       | 仅需当前类对象作为左操作数        | 直接访问私有成员，无需friend          | 不支持非类类型左操作数，无对称性      |
| 非成员函数重载     | 需支持非类类型左操作数/对称性     | 灵活，STL推荐，支持外部类重载          | 访问私有成员需friend，代码稍繁琐      |

### 3. 关键禁忌
- 不重载无直观语义的运算符（如`operator&`用于字符串拼接）。
- 不改变运算符的操作数个数和优先级（如不能把`+`改成一元运算符）。
- 不同时定义成员和非成员的同一运算符（避免歧义）。

### 4. 后续内容
下一节将聚焦“特殊成员函数”，深入探讨构造函数、析构函数、赋值运算符等编译器默认生成的函数，以及如何自定义这些函数以满足复杂需求。