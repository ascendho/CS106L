# Classes

## 一、类的核心定位与基础结构
类（Class）是C++面向对象编程的核心，本质是「数据与操作的封装体」，用于将相关的变量（成员变量）和函数（成员函数）捆绑成一个独立实体，同时实现数据隐藏与接口抽象。

### 1. 类的定义语法
```cpp
class ClassName {
    // 访问控制修饰符（默认private）
public:
    // 公有成员（对外接口：成员函数/变量）
    void memberFunc();  // 成员函数声明
private:
    // 私有成员（内部数据：仅类内可访问）
    int memberVar;     // 成员变量
protected:
    // 保护成员（仅类内及派生类可访问）
    double proVar;
};

// 成员函数类外定义（需加作用域解析符::）
void ClassName::memberFunc() {
    memberVar = 10;  // 直接访问私有成员
}
```

### 2. 访问控制的核心规则
- **public**：对外暴露的接口，允许类外部通过对象访问（如 `obj.memberFunc()`）。
- **private**：仅类内部成员可访问，外部无法直接操作（封装的核心，隐藏实现细节）。
- **protected**：类内部及派生类可访问，外部不可见（用于继承场景）。
- 注：struct与class的唯一区别——struct默认访问控制为public，class默认private。

---

## 二、特殊成员函数（类的核心操作）
类的特殊成员函数负责对象的创建、销毁、拷贝等核心操作，分为编译器默认生成和用户自定义两种。

### 1. 构造函数（对象初始化）
- 核心作用：创建对象时自动调用，初始化成员变量。
- 关键特性：
  1. 函数名与类名完全一致，无返回值（无需写void）。
  2. 支持重载（参数列表不同即可），满足多种初始化需求。
  3. 无自定义构造函数时，编译器生成默认构造函数（无参、不初始化内置类型成员）。
- 初始化列表（推荐用法，效率更高）：
  ```cpp
  class Point {
  private:
      int x, y;
  public:
      // 带参数的构造函数（初始化列表初始化成员）
      Point(int xVal, int yVal) : x(xVal), y(yVal) {}
      // 默认构造函数（无参）
      Point() : x(0), y(0) {}
      // 单参数构造函数（支持隐式类型转换，加explicit可禁用）
      explicit Point(int val) : x(val), y(val) {}
  };
  ```

### 2. 析构函数（对象销毁与资源释放）
- 核心作用：对象生命周期结束时自动调用，释放动态内存、文件句柄等资源。
- 关键特性：
  1. 函数名前加`~`，与类名一致，无参数、无返回值，不可重载。
  2. 无自定义析构函数时，编译器生成默认析构函数（不释放动态内存）。
- 示例（释放动态内存）：
  ```cpp
  class String {
  private:
      char* data;
  public:
      String(const char* str) {
          data = new char[strlen(str) + 1];
          strcpy(data, str);
      }
      // 自定义析构函数释放内存，避免内存泄漏
      ~String() { delete[] data; }
  };
  ```

### 3. 拷贝语义（拷贝构造+赋值运算符重载）
用于对象的“复制”操作，默认情况下编译器生成「浅拷贝」（仅复制成员变量的值），存在资源共享风险。

#### （1）拷贝构造函数
- 调用时机：用已有对象初始化新对象（如`String s2 = s1`、函数值传递参数）。
- 语法要求：参数必须是「同类对象的const引用」（避免无限递归拷贝）。
- 示例（深拷贝实现）：
  ```cpp
  String::String(const String& other) {
      // 深拷贝：为新对象分配独立内存
      data = new char[strlen(other.data) + 1];
      strcpy(data, other.data);
  }
  ```

#### （2）赋值运算符重载
- 调用时机：已有对象之间赋值（如`s2 = s1`，s2已初始化）。
- 核心规则：
  1. 返回值为「同类对象的引用」（支持链式赋值`a = b = c`）。
  2. 先检查自赋值（`if (this == &other)`），避免重复释放资源。
  3. 先释放当前对象资源，再深拷贝目标对象资源。
- 示例：
  ```cpp
  String& String::operator=(const String& other) {
      if (this == &other) return *this;  // 处理自赋值
      delete[] data;  // 释放当前资源
      // 深拷贝目标对象资源
      data = new char[strlen(other.data) + 1];
      strcpy(data, other.data);
      return *this;
  }
  ```

#### （3）浅拷贝 vs 深拷贝
- 浅拷贝：仅复制指针地址，多个对象共享同一块内存，析构时会重复释放导致崩溃。
- 深拷贝：为新对象分配独立内存，复制数据内容，对象间资源独立，无共享风险。
- 适用场景：类含动态内存（指针、new分配的资源）时，必须自定义深拷贝；无动态资源时，默认浅拷贝即可。

---

## 三、this指针与const正确性
### 1. this指针的核心特性
- 本质：隐藏的const指针（`ClassName* const this`），自动传递给非static成员函数。
- 作用：指向当前调用成员函数的对象，用于区分成员变量与局部变量（如`this->memberVar`）。
- 示例：
  ```cpp
  void Point::setX(int x) {
      this->x = x;  // this->x 指成员变量，x 指局部参数
  }
  ```

### 2. const成员函数（只读操作）
- 语法：函数参数列表后加`const`（修饰this指针为`const ClassName* const this`）。
- 核心规则：
  1. 承诺不修改对象的非static成员变量（mutable修饰的变量除外）。
  2. const对象只能调用const成员函数，非const对象可调用所有成员函数。
  3. const成员函数内部不能调用非const成员函数（避免间接修改对象）。
- 示例：
  ```cpp
  class Point {
  public:
      int getX() const {  // const成员函数（只读）
          return x;  // 允许访问，不允许修改
      }
      void setX(int x) {  // 非const成员函数（可修改）
          this->x = x;
      }
  private:
      int x, y;
  };
  
  const Point p1(1,2);
  p1.getX();  // 允许：const对象调用const成员函数
  p1.setX(3); // 报错：const对象不能调用非const成员函数
  ```

---

## 四、类的扩展：嵌套类与友元
### 1. 嵌套类（内部类）
- 定义：一个类定义在另一个类的内部，称为嵌套类（外层类为包围类）。
- 核心特性：
  1. 嵌套类是独立类，不共享外层类的成员，需通过外层类对象访问外层成员。
  2. 常用于封装与外层类强相关的功能（如容器的迭代器），避免暴露内部细节。
- 示例（容器的嵌套迭代器类）：
  ```cpp
  class MyVector {
  private:
      int* data;
      size_t size;
  public:
      // 嵌套迭代器类（仅MyVector内部逻辑相关）
      class Iterator {
      private:
          int* ptr;
      public:
          Iterator(int* p) : ptr(p) {}
          int& operator*() { return *ptr; }
          Iterator& operator++() { ptr++; return *this; }
          bool operator!=(const Iterator& other) { return ptr != other.ptr; }
      };
      // 容器的迭代器接口
      Iterator begin() { return Iterator(data); }
      Iterator end() { return Iterator(data + size); }
  };
  ```

### 2. 友元（突破封装的特殊访问）
- 定义：通过`friend`关键字，允许外部函数/类访问当前类的private/protected成员。
- 类型与语法：
  ```cpp
  class Point {
      // 友元函数：允许该函数访问私有成员
      friend void printPoint(const Point& p);
      // 友元类：允许该类的所有成员函数访问私有成员
      friend class PointPrinter;
  private:
      int x, y;
  };
  
  void printPoint(const Point& p) {
      // 允许访问private成员x、y
      std::cout << p.x << "," << p.y << std::endl;
  }
  ```
- 注意：友元破坏封装性，仅在必要时使用（如运算符重载、工具类协作）。

---

## 五、实战：用类实现自定义容器与迭代器
结合前一讲迭代器知识，用类设计自定义容器，并实现配套迭代器，支持范围for循环。

### 完整示例代码
```cpp
#include <iostream>
#include <cstring>
using namespace std;

class MyString {
private:
    char* data;
    size_t length;
public:
    // 1. 构造函数（重载）
    MyString() : data(nullptr), length(0) {}
    MyString(const char* str) {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }

    // 2. 拷贝构造（深拷贝）
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
    }

    // 3. 赋值运算符重载（深拷贝）
    MyString& operator=(const MyString& other) {
        if (this == &other) return *this;
        delete[] data;
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        return *this;
    }

    // 4. 析构函数
    ~MyString() { delete[] data; }

    // 5. 嵌套迭代器类
    class Iterator {
    private:
        char* ptr;
    public:
        Iterator(char* p) : ptr(p) {}
        // 迭代器核心操作
        char& operator*() { return *ptr; }
        Iterator& operator++() { ptr++; return *this; }
        bool operator!=(const Iterator& other) { return ptr != other.ptr; }
    };

    // 6. 容器迭代器接口
    Iterator begin() { return Iterator(data); }
    Iterator end() { return Iterator(data + length); }

    // 7. 其他成员函数
    size_t size() const { return length; }
};

// 测试：用范围for循环遍历自定义容器
int main() {
    MyString str("Hello CS106L!");
    for (char c : str) {  // 支持范围for，本质调用迭代器
        cout << c;
    }
    return 0;
}
```

### 核心要点
- 容器类负责管理数据（动态内存），通过构造/析构/拷贝函数保证资源安全。
- 迭代器类嵌套在容器内部，封装指针操作，提供`++`、`*`、`!=`核心接口。
- 容器提供`begin()`（返回首元素迭代器）和`end()`（返回尾后迭代器），适配范围for循环。

---

## 六、核心总结
1. 类的核心是「封装」：通过private隐藏数据，public暴露接口，降低耦合。
2. 特殊成员函数（构造、析构、拷贝、赋值）决定对象的生命周期与资源管理，含动态资源时必须自定义深拷贝。
3. const正确性是C++类设计的关键：用const成员函数标记只读操作，避免意外修改。
4. 嵌套类适合封装配套功能（如迭代器），友元用于必要的跨类访问，需谨慎使用。
5. 类与迭代器的结合是STL容器的设计核心：容器管理数据，迭代器提供统一遍历接口。
