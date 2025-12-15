# Inheritance

## 一、继承的核心定位与基础语法

继承是C++面向对象编程的核心机制，核心目的是**代码复用**——让子类（派生类）继承父类（基类）的公共成员（属性+行为），同时可扩展自身特有功能，形成“is-a”的层级关系（如“学生是一种人”）。

### 1. 继承的基础语法

```cpp
// 基类（父类）：封装公共属性与行为
class Base {
public:
    void publicFunc() {}  // 公共成员
protected:
    int proVar;           // 保护成员（为继承设计）
private:
    double priVar;        // 私有成员（仅基类内部访问）
};

// 派生类（子类）：继承基类 + 扩展功能
// 语法：class 子类名 : 继承方式 父类名
class Derived : public Base {  // 显式指定public继承（推荐）
public:
    void derivedFunc() {
        publicFunc();  // 可访问基类public成员
        proVar = 10;   // 可访问基类protected成员
        // priVar = 2.0;  // 错误：基类private成员不可直接访问
    }
private:
    int derivedVar;    // 子类特有成员
};
```

### 2. 继承方式与访问权限规则
继承方式（public/protected/private）决定基类成员在子类中的访问权限，核心遵循“**最小限制原则**”——子类中成员的最终权限 = 基类成员权限与继承方式权限的更严格者（权限等级：public > protected > private）。

| 基类成员类型 | public继承（常用） | protected继承 | private继承 |
|--------------|--------------------|---------------|-------------|
| public成员    | 子类public（外部可访问） | 子类protected（外部不可访问） | 子类private（外部不可访问） |
| protected成员 | 子类protected（外部不可访问） | 子类protected（外部不可访问） | 子类private（外部不可访问） |
| private成员   | 不可直接访问（需基类接口间接访问） | 不可直接访问 | 不可直接访问 |

### 3. 默认继承方式
- 用`class`定义子类时，默认继承方式为`private`（如`class Derived : Base {}`等价于`private Base`）。
- 用`struct`定义子类时，默认继承方式为`public`（如`struct Derived : Base {}`等价于`public Base`）。
- 建议：显式指定继承方式（优先`public`），避免歧义。

---

## 二、构造与析构的执行机制
继承体系中，基类与子类的构造、析构函数调用遵循固定顺序，直接影响对象的初始化与资源安全。

### 1. 构造函数调用顺序
遵循“**先基类，后子类；成员变量按声明顺序**”原则：
1. 调用基类的构造函数（默认调用无参构造，可通过初始化列表指定有参构造）。
2. 调用子类成员变量的构造函数（按变量在子类中的声明顺序，与初始化列表顺序无关）。
3. 执行子类构造函数体的逻辑。

示例（显式调用基类有参构造）：
```cpp
class Base {
public:
    Base(int val) : baseVar(val) {}  // 基类有参构造
private:
    int baseVar;
};

class Derived : public Base {
public:
    // 初始化列表：先调用基类有参构造，再初始化子类成员
    Derived(int baseVal, int derivedVal) : Base(baseVal), derivedVar(derivedVal) {}
private:
    int derivedVar;
};
```

### 2. 析构函数调用顺序
与构造顺序**完全相反**（“先子类，后基类”）：
1. 执行子类析构函数体的逻辑。
2. 调用子类成员变量的析构函数（按声明逆序）。
3. 调用基类的析构函数。

### 3. 虚析构函数（关键！）
当用“基类指针指向子类对象”时，若基类析构函数非虚函数，`delete`指针只会调用基类析构，导致子类资源泄漏。

解决方案：将基类析构函数声明为`virtual`（虚析构）：
```cpp
class Base {
public:
    virtual ~Base() {  // 虚析构：确保子类析构被调用
        // 基类资源释放逻辑
    }
};

class Derived : public Base {
public:
    ~Derived() override {  // override标记重写（可选但推荐）
        // 子类资源释放逻辑（如动态内存）
    }
};

// 正确用法：基类指针指向子类对象，delete时调用析构链
Base* ptr = new Derived();
delete ptr;  // 先调用Derived::~Derived()，再调用Base::~Base()
```

---

## 三、多态的核心：虚函数与动态绑定
多态是继承的核心价值，允许“基类指针/引用调用子类的重写函数”，实现“运行时确定调用逻辑”（动态绑定）。

### 1. 动态多态的三个必要条件
1. 基类中声明虚函数（加`virtual`关键字）。
2. 子类重写虚函数（函数名、参数列表、返回值完全一致）。
3. 通过基类的指针或引用指向子类对象，调用虚函数。

### 2. 虚函数与重写示例（动物叫场景）
```cpp
class Animal {
public:
    virtual void speak() {  // 基类虚函数
        cout << "动物发出声音" << endl;
    }
    virtual ~Animal() {}  // 虚析构（必加）
};

class Cat : public Animal {
public:
    void speak() override {  // 子类重写虚函数（override可选，用于语法检查）
        cout << "小猫：喵喵喵~" << endl;
    }
};

class Dog : public Animal {
public:
    void speak() override {
        cout << "小狗：汪汪汪~" << endl;
    }
};

// 测试：基类指针指向不同子类对象，实现多态
int main() {
    Animal* animal1 = new Cat();
    Animal* animal2 = new Dog();
    animal1->speak();  // 输出“小猫：喵喵喵~”（运行时绑定）
    animal2->speak();  // 输出“小狗：汪汪汪~”（运行时绑定）
    delete animal1;
    delete animal2;
    return 0;
}
```

### 3. 虚函数的底层原理：虚函数表（vtable）与虚指针（vptr）
- 虚函数表（vtable）：每个包含虚函数的类，会生成一个全局唯一的vtable，存储该类所有虚函数的地址。
- 虚指针（vptr）：每个包含虚函数的对象，会隐含一个vptr成员，指向所属类的vtable。
- 动态绑定过程：通过基类指针找到对象的vptr → 从vtable中获取子类重写后的虚函数地址 → 调用对应函数。

### 4. 纯虚函数与抽象类
纯虚函数是“没有实现体的虚函数”，用于定义接口规范；包含纯虚函数的类称为抽象类，**不能实例化对象**，仅用于被继承。

语法与示例：
```cpp
class Animal {  // 抽象类（含纯虚函数）
public:
    // 纯虚函数：=0表示无实现体，强制子类重写
    virtual void speak() = 0;
    virtual void eat() = 0;
    virtual ~Animal() {}
};

class Bird : public Animal {
public:
    void speak() override { cout << "小鸟：叽叽叽~" << endl; }
    void eat() override { cout << "小鸟：吃虫子~" << endl; }
};

// Animal animal;  // 错误：抽象类不能实例化
Animal* bird = new Bird();  // 正确：抽象类指针指向子类对象
```

---

## 四、继承的进阶特性：final与override
C++11提供`final`和`override`关键字，用于增强继承的安全性和可读性，避免潜在错误。

### 1. override关键字
- 作用：显式声明子类函数“重写基类虚函数”，编译器会检查重写的正确性（如函数名、参数写错时编译报错）。
- 用法：在子类重写函数的参数列表后加`override`。

示例（错误检查）：
```cpp
class Animal {
public:
    virtual void speak(int volume) {}  // 基类虚函数带int参数
};

class Cat : public Animal {
public:
    // 错误：基类speak带int参数，此处无参数，override触发编译报错
    void speak() override {}
};
```

### 2. final关键字
- 修饰类：禁止该类被继承。
- 修饰虚函数：禁止该虚函数被子类重写。

示例：
```cpp
class FinalClass final {};  // 被final修饰，不能被继承
// class Derived : public FinalClass {};  // 错误：禁止继承

class Base {
public:
    virtual void func() final {}  // 被final修饰，不能被重写
};

class Derived : public Base {
public:
    // void func() override {}  // 错误：禁止重写final虚函数
};
```

---

## 五、多重继承与菱形继承问题
多重继承指子类同时继承多个基类（如`class Derived : public Base1, public Base2`），核心问题是“菱形继承”导致的二义性和数据冗余。

### 1. 菱形继承的问题
当两个基类继承自同一个公共基类，且子类同时继承这两个基类时，会形成“菱形”结构：
- 二义性：子类访问公共基类的成员时，编译器无法确定访问哪个路径的基类成员。
- 数据冗余：子类对象中会存储两份公共基类的成员，浪费内存。

示例（菱形继承结构）：
```cpp
class Animal { public: int age; };  // 公共基类
class Sheep : public Animal {};     // 基类1
class Tuo : public Animal {};       // 基类2
class SheepTuo : public Sheep, public Tuo {};  // 子类（菱形结构）

SheepTuo st;
// st.age = 3;  // 错误：二义性（Sheep::age 或 Tuo::age？）
```

### 2. 解决方案：虚继承（virtual inheritance）
在基类继承公共基类时，加`virtual`关键字，确保子类中仅保留一份公共基类的实例，消除二义性和数据冗余。

修改示例（虚继承解决菱形问题）：
```cpp
class Animal { public: int age; };
// 虚继承：Sheep和Tuo共享Animal实例
class Sheep : virtual public Animal {};
class Tuo : virtual public Animal {};
class SheepTuo : public Sheep, public Tuo {};

SheepTuo st;
st.age = 3;  // 正确：仅一份age成员，无歧义
```

- 底层原理：虚继承的基类会生成“虚基类指针（vbptr）”，指向“虚基类表（vbtable）”，通过偏移量找到公共基类的成员。

---

## 六、继承与组合的选择（设计原则）
继承（is-a）和组合（has-a）是两种代码复用方式，核心遵循“**组合优于继承**”的设计原则（降低耦合度）。

### 1. 核心区别
| 特性         | 继承（is-a 关系）                | 组合（has-a 关系）                |
|--------------|----------------------------------|----------------------------------|
| 关系本质     | 子类是基类的一种特殊类型（如“狗是动物”） | 类包含其他类的对象作为成员（如“汽车有发动机”） |
| 耦合度       | 高（基类修改会直接影响子类）      | 低（通过接口交互，组件可替换）    |
| 灵活性       | 低（继承层次固定，难扩展）        | 高（组件可动态替换，易扩展）      |
| 多态支持     | 天然支持（虚函数）                | 需结合接口抽象实现                |

### 2. 适用场景
- 用继承：明确的is-a关系、需要多态特性、定义类族的公共接口（如抽象类）。
- 用组合：功能模块独立、需要动态扩展或替换组件（如游戏角色的装备系统、汽车的发动机模块）。

---

## 七、实战：基于继承与多态的图形绘制系统
设计一个图形类层级，实现多态绘制功能，展示继承、虚函数、抽象类的综合应用。

### 完整代码
```cpp
#include <iostream>
using namespace std;

// 抽象基类：图形（定义接口）
class Shape {
public:
    virtual void draw() = 0;  // 纯虚函数：强制子类实现绘制逻辑
    virtual double area() = 0; // 纯虚函数：计算面积
    virtual ~Shape() {}  // 虚析构
};

// 子类1：圆形
class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    void draw() override {
        cout << "绘制圆形（半径：" << radius << "）" << endl;
    }
    double area() override {
        return 3.14 * radius * radius;
    }
};

// 子类2：矩形
class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    void draw() override {
        cout << "绘制矩形（宽：" << width << "，高：" << height << "）" << endl;
    }
    double area() override {
        return width * height;
    }
};

// 测试：多态调用
int main() {
    Shape* shapes[] = {new Circle(2.0), new Rectangle(3.0, 4.0)};
    for (Shape* s : shapes) {
        s->draw();          // 多态绘制
        cout << "面积：" << s->area() << endl;  // 多态计算面积
        delete s;
    }
    return 0;
}
```

### 输出结果
```
绘制圆形（半径：2.0）
面积：12.56
绘制矩形（宽：3.0，高：4.0）
面积：12.0
```

---

## 八、核心总结
1. 继承的核心是代码复用，通过is-a关系实现子类复用基类成员，优先使用public继承。
2. 构造顺序“先基类后子类”，析构顺序相反；基类析构必须为虚析构，避免子类资源泄漏。
3. 多态的核心是虚函数+重写+基类指针/引用，底层依赖虚函数表实现动态绑定。
4. 抽象类（含纯虚函数）用于定义接口，不能实例化；final和override增强代码安全性。
5. 菱形继承通过虚继承解决；设计时优先选择组合（低耦合），仅在需要多态时使用继承。
