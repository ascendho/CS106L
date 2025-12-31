# Move Semantics

## 一、核心结论
移动语义是C++11引入的核心特性，通过“窃取”临时对象的资源（如动态内存）替代无意义的拷贝，解决数据拷贝的高昂成本问题；其实现依赖左值/右值的区分，以及移动构造函数、移动赋值运算符两个特殊成员函数，遵循零规则、三规则、五规则指导实践。

## 二、特殊成员函数（SMFs）回顾
### 1. 核心SMFs（此前已学）
| 函数类型               | 语法格式                          | 核心作用                                  |
|------------------------|-----------------------------------|-------------------------------------------|
| 拷贝构造函数           | `Type::Type(const Type& other)`    | 基于已有对象创建新对象，深拷贝资源        |
| 拷贝赋值运算符        | `Type& Type::operator=(const Type& other)` | 为已有对象赋值，先释放自身资源再深拷贝    |
| 析构函数               | `Type::~Type()`                    | 释放对象资源（如动态内存），避免内存泄漏  |

### 2. 示例：Photo类的基础SMFs实现
```cpp
class Photo {
public:
  // 构造函数：分配像素数据内存
  Photo(int width, int height) : width(width), height(height), data(new int[width * height]) {}
  
  // 拷贝构造函数：深拷贝数据
  Photo(const Photo& other) : width(other.width), height(other.height), data(new int[width * height]) {
    std::copy(other.data, other.data + width * height, data);
  }
  
  // 拷贝赋值运算符：先释放自身资源，再深拷贝
  Photo& operator=(const Photo& other) {
    if (this == &other) return *this; // 避免自赋值
    delete[] data; // 释放旧资源
    width = other.width;
    height = other.height;
    data = new int[width * height];
    std::copy(other.data, other.data + width * height, data);
    return *this;
  }
  
  // 析构函数：释放数据内存
  ~Photo() { delete[] data; }

private:
  int width, height;
  int* data; // 动态分配的像素数据
};
```

## 三、核心问题：拷贝的高昂成本
### 1. 数据拷贝的性能损耗
- 实验表明：数据移动（拷贝）的能量消耗占设备总能耗的35%，在科学计算中，内存数据移动的成本比寄存器运算高两个数量级。
- 临时对象拷贝的无意义性：函数返回值（如`takePhoto()`的返回值）是临时对象，拷贝后立即被析构，拷贝操作完全浪费资源。

### 2. 示例：临时对象拷贝的问题
```cpp
Photo takePhoto(); // 返回临时Photo对象
int main() {
  Photo selfie = takePhoto(); // 拷贝构造：深拷贝3840×2160像素数据
}
```
- 过程：临时对象的`data`指针指向内存→`selfie`深拷贝该内存→临时对象析构释放内存，拷贝操作耗时耗资源。
- 需求：直接“窃取”临时对象的`data`指针，无需拷贝。

## 四、关键概念：左值（lvalue）与右值（rvalue）
### 1. 核心区分
| 特性               | 左值（lvalue）                          | 右值（rvalue）                          |
|--------------------|-----------------------------------------|-----------------------------------------|
| 本质               | 持久对象，有明确内存地址                | 临时对象，无明确内存地址                |
| 生命周期           | 直到作用域结束                          | 直到当前语句结束                        |
| 示例               | 变量（`selfie`）、引用（`int& b = a`）  | 字面量（`5`）、函数返回值（`takePhoto()`）、表达式结果（`c[1]`） |
| 赋值操作           | 可出现在赋值符号左右两侧                | 仅能出现在赋值符号右侧                  |
| 地址获取           | 可通过`&`取地址（`&selfie`合法）         | 不可通过`&`取地址（`&takePhoto()`非法） |

### 2. 引用绑定规则
- 左值引用（`Type&`）：仅能绑定左值，需保证对象在函数调用后仍处于有效状态。
- 右值引用（`Type&&`）：仅能绑定右值，临时对象可被“窃取”资源，无需维持其有效状态。

### 3. 重载区分左/右值
通过重载左值引用和右值引用参数，编译器自动根据实参类型选择调用版本：
```cpp
// 处理左值：不窃取资源（对象可能后续被使用）
void upload(Photo& pic); 
// 处理右值：可窃取资源（临时对象即将析构）
void upload(Photo&& pic); 

// 调用示例
int main() {
  Photo selfie = takePhoto();
  upload(selfie); // 调用左值引用版本（selfie是左值）
  upload(takePhoto()); // 调用右值引用版本（返回值是右值）
}
```

## 五、解决方案：移动语义的实现
### 1. 两个新特殊成员函数
#### （1）移动构造函数
- 语法：`Type::Type(Type&& other)`
- 核心逻辑：窃取`other`（右值，临时对象）的资源，重置`other`为“可析构状态”（避免重复释放）。
- Photo类实现：
  ```cpp
  Photo::Photo(Photo&& other) : width(other.width), height(other.height), data(other.data) {
    other.data = nullptr; // 关键：重置原对象指针，析构时无操作
    other.width = 0;
    other.height = 0;
  }
  ```

#### （2）移动赋值运算符
- 语法：`Type& Type::operator=(Type&& other)`
- 核心逻辑：释放自身资源→窃取`other`的资源→重置`other`。
- Photo类实现：
  ```cpp
  Photo& Photo::operator=(Photo&& other) {
    if (this == &other) return *this; // 避免自赋值
    
    delete[] data; // 释放自身旧资源
    // 窃取other的资源
    width = other.width;
    height = other.height;
    data = other.data;
    // 重置other
    other.data = nullptr;
    other.width = 0;
    other.height = 0;
    
    return *this;
  }
  ```

### 2. 移动语义的优势
- 无数据拷贝：仅复制指针（4/8字节），替代大规模像素数据拷贝，效率极大提升。
- 安全析构：原对象指针被置为`nullptr`，析构时`delete[] nullptr`无副作用。

## 六、std::move：强制触发移动语义
### 1. 核心作用
`std::move`并非“移动”操作，仅将左值强制转换为右值引用，让编译器调用移动相关SMFs，而非拷贝版本。

### 2. 适用场景
当明确左值后续不再被使用时，通过`std::move`避免不必要的拷贝：
```cpp
void PhotoCollection::insert(const Photo& pic, int pos) {
  // 移动元素时，原元素不再使用，强制触发移动赋值
  for (int i = size(); i > pos; i--) {
    myPhotos[i] = std::move(myPhotos[i-1]); // 替代拷贝赋值
  }
  myPhotos[pos] = pic;
}
```

### 3. 注意事项
- 移动后原对象状态：被移动的对象（即使是左值）会处于“有效但未定义”状态，不可再使用（如调用`get_pixel`）。
- 避免滥用：仅在确定对象不再使用时使用`std::move`，否则可能导致程序崩溃。
- 本质：`std::move`等价于`static_cast<Type&&>(t)`，无额外逻辑。

## 七、实践规则：零规则、三规则、五规则
### 1. 零规则（Rule of Zero）
- 核心：若类不直接管理动态内存/外部资源（仅含`std::string`、`std::vector`等自管理成员），无需自定义任何SMFs，编译器默认生成的版本足够安全高效。
- 示例：
  ```cpp
  struct Post {
    Photo photo; // Photo已实现完整SMFs
    std::string caption; // std::string自带移动语义
  };
  // 无需自定义拷贝/移动/析构函数
  ```

### 2. 三规则（Rule of Three）
- 核心：若类需要自定义析构函数（通常因管理动态内存），则必须同时自定义拷贝构造函数和拷贝赋值运算符，避免浅拷贝导致的内存问题。
- 原因：自定义析构函数意味着手动管理资源，编译器默认的浅拷贝会导致多个对象共享资源，析构时重复释放。

### 3. 五规则（Rule of Five）
- 核心：若已自定义三规则要求的3个SMFs（析构、拷贝构造、拷贝赋值），建议额外自定义移动构造函数和移动赋值运算符，避免不必要的拷贝，提升代码效率。
- 说明：移动相关SMFs是“可选增强”，而非强制要求，但缺失会导致临时对象仍触发拷贝，性能损耗。

## 八、核心总结
### 1. 移动语义的核心价值
- 解决临时对象拷贝的浪费问题，尤其适用于动态内存、大尺寸数据场景。
- 区分左值/右值，实现“持久对象拷贝、临时对象移动”的合理逻辑。

### 2. 关键注意事项
- 移动仅适用于临时对象或明确不再使用的左值，避免“使用已移动对象”。
- `std::move`是类型转换工具，不触发移动操作，仅让编译器选择移动SMFs。
- 优先遵循零规则，减少自定义SMFs的代码冗余和出错风险。

### 3. 完整SMFs家族（6个）
| 函数类型               | 语法格式                          |
|------------------------|-----------------------------------|
| 默认构造函数           | `Type::Type()`                    |
| 拷贝构造函数           | `Type::Type(const Type& other)`    |
| 移动构造函数           | `Type::Type(Type&& other)`         |
| 拷贝赋值运算符        | `Type& Type::operator=(const Type& other)` |
| 移动赋值运算符        | `Type& Type::operator=(Type&& other)` |
| 析构函数               | `Type::~Type()`                    |
