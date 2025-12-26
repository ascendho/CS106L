# Class Templates & Const Correctness

## 一、模板类（Template Classes）
### 1. 模板类的核心动机
- 问题背景：传统方式中，存储不同类型（int、double、string等）的序列需要编写重复逻辑的类（如`IntVector`、`DoubleVector`），代码冗余且扩展性差（无法应对自定义类型）。
- 核心目标：**复用逻辑，适配不同类型**，自动生成特定类型的类代码，无需手动编写重复实现。

### 2. 模板类的历史演进
#### （1）早期解决方案：预处理器宏（Preprocessor Macro）
- 实现方式：通过宏定义生成特定类型的类，例如：
  ```cpp
  #define GENERATE_VECTOR(MY_TYPE) 
  class MY_TYPE##Vector { 
  public:
    MY_TYPE& at(size_t index); 
    void push_back(const MY_TYPE& elem); 
  private:
    MY_TYPE* elems; 
    size_t logical_size; 
    size_t array_size; 
  };
  ```
- 使用方式：调用宏生成对应类型的类（如`GENERATE_VECTOR(int)`生成`intVector`），本质是**预处理阶段的代码生成**。
- 致命缺陷：语法繁琐、无法类型检查、易漏调用或重复调用，维护成本高。

#### （2）现代解决方案：模板类（Template Class）
- 核心改进：编译器级别的自动代码生成，解决宏的缺陷，支持任意类型（包括自定义类型）。
- 设计思想：由Alexander Stepanov（STL创始人）提出，Bjarne Stroustrup（C++之父）支持，核心是“逻辑通用，类型参数化”。

### 3. 模板类的基础语法与核心概念
#### （1）声明与定义
- 声明格式：`template <typename T> class 类名`，其中`T`是**类型参数**（可替换为任意类型）。
  ```cpp
  template <typename T>
  class Vector {
  public:
    T& at(size_t index);          // 成员方法，返回T类型引用
    void push_back(const T& elem); // 接收T类型常量引用
  private:
    T* elems;                     // 存储T类型元素的指针
    size_t logical_size;          // 实际元素个数
    size_t array_size;            // 底层数组容量
  };
  ```
- 关键说明：`typename`与`class`等价（如`template <class T>`），可互换使用。

#### （2）模板实例化（Template Instantiation）
- 定义：使用模板时，指定具体类型（如`int`、`string`），编译器**按需生成**该类型的类代码（编译阶段完成）。
- 示例：
  ```cpp
  Vector<int> intVec;          // 生成int类型的Vector类
  Vector<std::string> strVec;  // 生成string类型的Vector类
  Vector<Vector<int>> vecVec;  // 支持嵌套实例化
  struct MyType {};
  Vector<MyType> customVec;    // 支持自定义类型
  ```
- 本质：不同实例化结果是**完全独立的类型**（如`Vector<int>`与`Vector<double>`无继承/兼容关系，编译期和运行期均不同）。

#### （3）非类型模板参数（Fun Fact）
- 模板参数不仅可以是类型（`typename T`），还可以是常量值（如`size_t`、`bool`）。
- 示例：
  ```cpp
  template <size_t N> class SizeTemplate {};  // N是整数常量参数
  SizeTemplate<5> s;                          // 实例化时指定N=5
  
  template <bool B> class BoolTemplate {};
  BoolTemplate<true> b;                       // 实例化时指定B=true
  
  std::array<std::string, 5> arr;             // 标准库示例：固定大小的数组（栈分配，无堆内存开销）
  ```

### 4. 模板类的实现注意事项
#### （1）语法要求
- 实现成员方法时，必须重复模板声明，并明确指定类名是`类名<T>`（而非原始类名）：
  ```cpp
  // Vector.cpp
  template <typename T>
  T& Vector<T>::at(size_t index) {  // 必须写Vector<T>::，而非Vector::
    return elems[index];
  }
  ```

#### （2）头文件与实现文件的关系
- 特殊规则：模板类的`.h`文件末尾必须包含`.cpp`文件（或直接将实现写在`.h`中）。
  ```cpp
  // Vector.h
  template <typename T> class Vector { /* 声明 */ };
  #include "Vector.cpp"  // 必须包含实现文件
  ```
- 原因：模板代码生成依赖“实例化时可见的实现”，编译器需在使用模板的文件中看到完整实现才能生成代码。

#### （3）模板与类型的区别
- 模板：`template <typename T> class Vector`是“类工厂”，**不是类型**，无法直接定义对象。
- 类型：`Vector<int>`是模板实例化后的结果，**是具体类型**，可定义对象、作为函数参数等。
- 常见错误：将不同实例化类型视为兼容（如`std::vector<double>`无法传递给接收`std::vector<int>`的函数）。

## 二、const正确性（Const Correctness）
### 1. 核心问题：const对象的方法调用限制
- 场景：当对象被声明为`const`时，编译器禁止其调用可能修改对象的方法（避免违背“只读”承诺）。
- 示例（错误）：
  ```cpp
  void printVec(const Vector<int>& v) {
    for (size_t i = 0; i < v.size(); i++) {  // 编译错误：const对象无法调用非const方法size()
      std::cout << v.at(i) << " ";
    }
  }
  ```
- 原因：编译器无法确定`size()`、`at()`是否修改对象（成员方法默认可访问/修改成员变量）。

### 2. 解决方案1：声明const成员方法
#### （1）语法与含义
- 声明格式：在成员方法括号后加`const`，表示“该方法不修改对象的成员变量”。
  ```cpp
  template <typename T>
  class Vector {
  public:
    size_t size() const;  // const方法：承诺不修改对象
    bool empty() const;   // const方法
    T& at(size_t index);  // 非const方法：可能修改对象
  };
  ```
- 实现要求：`.cpp`文件中实现时必须保留`const`关键字，否则编译器报错：
  ```cpp
  template <typename T>
  size_t Vector<T>::size() const {  // 必须加const，与声明一致
    return logical_size;
  }
  ```

#### （2）const方法的底层逻辑
- const方法中，`this`指针的类型为`const Vector<T>*`（而非`Vector<T>*`），因此无法修改成员变量（编译期检查）。
- 错误示例：const方法中修改成员变量会直接编译报错：
  ```cpp
  template <typename T>
  size_t Vector<T>::size() const {
    this->logical_size = 10;  // 错误：const方法无法修改成员变量
    return logical_size;
  }
  ```

### 3. 解决方案2：const重载（Const Overloading）
#### （1）问题升级：返回值的const属性
- 仅声明const方法仍有缺陷：
  1. 若const方法返回`T&`（非const引用），可能导致const对象被修改（违背只读承诺）。
  2. 若返回`const T&`，非const对象调用时无法修改元素（功能受限）。

#### （2）解决思路：重载const与非const版本的方法
- 规则：为同一方法定义两个版本，编译器根据对象是否为const自动选择：
  - 非const对象调用非const版本（返回`T&`，支持修改）。
  - const对象调用const版本（返回`const T&`，禁止修改）。
- 示例：
  ```cpp
  template <typename T>
  class Vector {
  public:
    T& at(size_t index);          // 非const版本：非const对象调用
    const T& at(size_t index) const;  // const版本：const对象调用
  };
  
  // 实现
  template <typename T>
  T& Vector<T>::at(size_t index) {
    return elems[index];
  }
  
  template <typename T>
  const T& Vector<T>::at(size_t index) const {
    return elems[index];
  }
  ```

#### （3）避免代码冗余：const_cast的妙用
- 问题：const与非const版本的方法实现可能完全相同（如`at()`），重复编写冗余。
- 解决方案：用`const_cast`“移除`this`的const属性”，让const版本调用非const版本（仅在逻辑完全一致时使用）：
  ```cpp
  template <typename T>
  const T& Vector<T>::findElement(const T& value) const {
    // const_cast<Vector<T>&>(*this)：将const Vector<T>&转为Vector<T>&
    return const_cast<Vector<T>&>(*this).findElement(value);
  }
  
  template <typename T>
  T& Vector<T>::findElement(const T& value) {
    for (size_t i = 0; i < logical_size; i++) {
      if (elems[i] == value) return elems[i];
    }
    throw std::out_of_range("Element not found");
  }
  ```
- 注意：`const_cast`是“危险操作”，仅在确认非const版本不会修改对象时使用（否则违背const承诺），日常开发中应尽量避免。

### 4. 精细控制：mutable关键字
- 场景：需让const对象的**特定成员变量**可修改（如调试信息、缓存数据），但不影响其他成员。
- 语法：用`mutable`修饰成员变量，该变量不受const对象的“只读”限制。
- 示例：
  ```cpp
  struct CameraRay {
    Point origin;          // const对象中不可修改
    Direction direction;   // const对象中不可修改
    mutable Color debugColor;  // const对象中可修改
  };
  
  const CameraRay ray;
  ray.debugColor = Color::Yellow;  // 合法：debugColor是mutable
  ```
- 注意：`mutable`会绕过const保护，需谨慎使用（仅用于“不影响对象核心状态”的变量）。

## 三、核心总结
### 1. 模板类关键要点
- 核心价值：**逻辑复用，类型通用**，自动生成特定类型的类代码，支持任意类型（包括自定义类型和嵌套类型）。
- 关键区别：模板是“类工厂”，实例化后的结果才是具体类型（不同实例化类型完全独立）。
- 实现规则：成员方法需重复模板声明，`.h`文件需包含`.cpp`文件（或实现写在`.h`中）。

### 2. const正确性关键要点
- 核心目标：保证const对象“只读”，避免意外修改，提升代码安全性和可读性。
- 核心规则：
  1. 不修改对象的方法必须声明为`const`。
  2. 为需返回引用的方法（如`at()`、`findElement()`）提供const与非const重载。
  3. 谨慎使用`const_cast`（避免冗余）和`mutable`（精细控制），避免滥用破坏const语义。
