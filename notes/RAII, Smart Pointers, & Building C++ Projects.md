# RAII, Smart Pointers, & Building C++ Projects

## 一、RAII（Resource Acquisition Is Initialization）
### 1. 核心问题：资源管理的痛点
C++中资源（内存、文件、锁、套接字等）需手动获取和释放，但实际代码存在两大问题：
- **代码路径复杂**：函数中可能有多个return语句、异常抛出点（如字符串构造、函数调用可能抛异常），导致手动释放资源（如`delete`、`unlock`）的代码被跳过。
- **资源泄漏风险**：以动态内存为例，`new`分配的指针若在异常抛出前未执行`delete`，会导致内存泄漏；同理，锁未解锁会导致死锁，文件未关闭会导致资源占用。

### 2. RAII核心思想与规则
- **定义**：资源的“获取”（如分配内存、加锁、打开文件）在类的构造函数中完成，资源的“释放”（如释放内存、解锁、关闭文件）在类的析构函数中完成。
- **核心原理**：C++保证对象超出作用域时，析构函数一定会被调用（无论是否有异常、是否提前return），从而确保资源必然释放。
- **RAII的优势**：
  1. 避免“半有效状态”：资源要么完全获取（对象构造成功），要么完全未获取（构造失败，无资源需释放）。
  2. 资源创建后立即可用：构造函数完成资源获取，对象创建后可直接使用。
  3. 自动释放：无需手动管理释放逻辑，减少人为失误。

### 3. RAII示例：解决实际问题
#### （1）反例：非RAII的锁管理（存在死锁风险）
```cpp
void cleanDatabase(mutex& dbLock, map<int, int>& db) {
  dbLock.lock(); // 获取锁
  modifyDB(db);  // 若此处抛异常，锁永远无法解锁→死锁
  dbLock.unlock(); // 释放锁（可能被跳过）
}
```

#### （2）正例：RAII的锁管理（`lock_guard`）
```cpp
void cleanDatabase(mutex& dbLock, map<int, int>& db) {
  lock_guard<mutex> lg(dbLock); // 构造时加锁（RAII）
  modifyDB(db);  // 即使抛异常，lg超出作用域时析构→自动解锁
}
```
`lock_guard`是RAII兼容的锁包装类，构造时获取锁，析构时释放锁，无需显式调用`unlock`。

## 二、智能指针（Smart Pointers）
### 1. 本质：RAII在内存管理中的应用
智能指针是封装原始指针的“资源句柄”，遵循RAII规则：
- 构造时获取动态内存（或接管原始指针）。
- 析构时自动释放内存（调用`delete`）。
- 核心目标：避免显式`new`/`delete`，根除内存泄漏。

### 2. 核心原则
- 避免手动调用`new`和`delete`：裸指针（`T*`）易导致泄漏，应将`new`的结果交给智能指针管理。
- 优先使用`std::make_unique`/`std::make_shared`初始化：避免二次内存分配，保证一致性，且更安全（若构造过程抛异常，已分配内存会自动释放）。

### 3. 三种智能指针的特性与使用场景
#### （1）`std::unique_ptr`：唯一所有权指针
- **核心特性**：独占所指向的内存，不可拷贝（编译时报错），仅可移动（通过`std::move`转移所有权）。
- **原理**：若允许拷贝，会导致多个`unique_ptr`指向同一内存，析构时重复释放（未定义行为）。
- **初始化与使用**：
  ```cpp
  // 正确：使用make_unique（推荐）
  std::unique_ptr<Node> ptr = std::make_unique<Node>();
  // 错误：不可拷贝
  std::unique_ptr<Node> copy = ptr; // 编译报错
  // 正确：移动所有权
  std::unique_ptr<Node> moved = std::move(ptr); // ptr变为空
  ```
- **适用场景**：单个对象的独占管理（如局部动态对象、容器中的元素）。

#### （2）`std::shared_ptr`：共享所有权指针
- **核心特性**：可拷贝，多个`shared_ptr`共享同一内存，内存仅在“最后一个`shared_ptr`超出作用域”时释放。
- **实现原理**：
  - 内部包含“控制块”：存储引用计数（当前指向该内存的`shared_ptr`数量）、弱引用计数、自定义删除器等。
  - 拷贝时引用计数+1，析构时引用计数-1；引用计数为0时，释放内存。
- **初始化与使用**：
  ```cpp
  // 正确：使用make_shared（推荐）
  std::shared_ptr<Node> ptr1 = std::make_shared<Node>();
  std::shared_ptr<Node> ptr2 = ptr1; // 拷贝，引用计数=2
  // ptr1和ptr2析构后，引用计数=0→内存释放
  ```
- **适用场景**：多个对象需要共享同一资源（如父子对象共享数据）。

#### （3）`std::weak_ptr`：解决循环依赖的弱引用指针
- **核心问题**：`shared_ptr`的循环依赖会导致内存泄漏：
  ```cpp
  class A { public: std::shared_ptr<B> ptr_b; };
  class B { public: std::shared_ptr<A> ptr_a; };
  // 循环依赖：a的ptr_b指向b，b的ptr_a指向a→引用计数永远不为0→内存泄漏
  std::shared_ptr<A> a = std::make_shared<A>();
  std::shared_ptr<B> b = std::make_shared<B>();
  a->ptr_b = b;
  b->ptr_a = a;
  ```
- **核心特性**：
  - 弱引用：指向`shared_ptr`管理的内存，但不增加引用计数。
  - 需通过`lock()`方法转换为`shared_ptr`才能访问内存（避免访问已释放的内存）。
- **解决循环依赖示例**：
  ```cpp
  class A { public: std::shared_ptr<B> ptr_b; };
  class B { public: std::weak_ptr<A> ptr_a; }; // 弱引用
  // a的ptr_b指向b（引用计数=1），b的ptr_a指向a（不增加计数）
  // a析构时引用计数=0→释放内存，b的ptr_a.lock()返回空→无泄漏
  ```
- **适用场景**：打破`shared_ptr`的循环依赖，仅需“观察”资源而非“拥有”资源。

### 4. 智能指针初始化对比
| 初始化方式                | 优点                                  | 缺点                                  |
|---------------------------|---------------------------------------|---------------------------------------|
| `std::make_unique<T>()`   | 无二次分配、安全、简洁                | 不可自定义删除器（需手动指定）        |
| `std::make_shared<T>()`   | 控制块与内存一次性分配、效率高        | 内存释放延迟（弱引用存在时控制块不释放） |
| `std::unique_ptr<T>(new T)`| 可自定义删除器                        | 二次分配、异常安全风险                |

## 三、C++项目构建（Compilation & Build Systems）
### 1. 编译基础流程
C++代码需通过编译器转换为机器码才能运行，核心流程：
```
源代码（.cpp）→ 编译器（g++/clang）→ 可执行文件（二进制）
```
- **编译命令示例**：
  ```bash
  g++ main.cpp -o main -std=c++20  # 编译main.cpp，生成可执行文件main，使用C++20标准
  ./main                           # 运行可执行文件
  ```

### 2. Makefile：简化编译命令
当项目有多个源文件时，手动输入编译命令繁琐，Makefile可自动化编译流程。

#### （1）Makefile核心结构
```makefile
# 编译器配置
CXX = g++                  # 指定编译器
CXXFLAGS = -std=c++20      # 编译选项（C++20标准）
# 源文件与目标文件
SRCS = $(wildcard *.cpp)   # 匹配所有.cpp源文件
TARGET = main              # 可执行文件名称
# 默认目标（执行make时自动运行）
all:
  $(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)  # 编译命令
# 清理目标（执行make clean时运行）
clean:
  rm -f $(TARGET)          # 删除可执行文件
```

#### （2）使用方式
```bash
make        # 执行all目标，编译项目
make clean  # 执行clean目标，清理可执行文件
```

### 3. CMake：跨平台构建系统生成器
CMake是更高层次的构建工具，可根据`CMakeLists.txt`生成适配不同平台（Windows/Linux/Mac）的Makefile、Visual Studio项目等。

#### （1）CMakeLists.txt核心结构
```cmake
cmake_minimum_required(VERSION 3.10)  # 最低CMake版本要求
project(cs106l_project)               # 项目名称
set(CMAKE_CXX_STANDARD 20)            # 指定C++20标准
file(GLOB SRC_FILES "*.cpp")          # 匹配所有.cpp源文件
add_executable(main ${SRC_FILES})     # 生成可执行文件main
```

#### （2）CMake使用步骤
1. 在项目根目录创建`CMakeLists.txt`。
2. 创建`build`文件夹并进入：`mkdir build && cd build`（隔离编译产物）。
3. 生成Makefile：`cmake ..`（`..`表示从上级目录读取CMakeLists.txt）。
4. 编译项目：`make`（执行生成的Makefile）。
5. 运行可执行文件：`./main`。

### 4. 核心优势
- Makefile：简化单平台编译命令，自动化构建流程。
- CMake：跨平台兼容性强，无需手动编写不同平台的构建脚本，适合大型项目。

## 四、课程总结

- RAII是资源管理的核心思想，智能指针是RAII在内存管理的具体实现，项目构建工具（Makefile/CMake）是工程化落地的保障。
- 开发原则：优先使用RAII兼容的工具（智能指针、`lock_guard`等），避免手动管理资源；工程化项目优先用CMake生成Makefile，提升开发效率。
