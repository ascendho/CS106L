# Optional Lecture: Unit Testing & C++ Iceberg

## 一、单元测试（Unit Testing）
### 1. 定义与核心特性
单元测试是测试驱动开发（TDD）的核心方法，专注于验证**软件最小独立组件**（通常是单个类或方法）的功能正确性。其核心是“小范围、高隔离、快执行”，确保每个单元的逻辑独立可靠。

### 2. 单元测试与集成/系统测试的对比
| 特性                | 单元测试（Unit Tests）                          | 集成/系统测试（Integration/System Tests）        |
|---------------------|-------------------------------------------------|-------------------------------------------------|
| 测试范围            | 单个类、方法等最小单元                          | 多个组件集成、整个系统或子系统                  |
| 隔离性              | 必须隔离，无外部依赖（依赖需模拟，如Mock）       | 无需隔离，在实际系统上下文或部分集成环境中测试  |
| 执行速度            | 极快（毫秒级），可频繁运行                      | 较慢（秒级/分钟级），测试流程更复杂             |
| 核心目的            | 验证单个单元的逻辑正确性，定位细粒度bug          | 验证组件协作、系统整体功能与兼容性              |

### 3. 单元测试的编写步骤（IBM标准）
1. **识别测试单元**：明确要测试的最小组件（如`BankAccount`类的`withdraw`方法）。
2. **选择测试方法**：确定验证逻辑（如正常场景、边界场景、异常场景）。
3. **搭建测试环境**：选择测试框架（如GoogleTest）、准备测试数据。
4. **设计测试用例**：覆盖关键场景（如存款、取款成功/失败、初始余额为0等）。
5. **调试与修复**：运行测试用例，定位并解决单元中的问题，确保测试通过。

### 4. 编写单元测试的核心优势
1. **早发现bug**：测试粒度细、执行快，可在开发过程中频繁运行，提前暴露问题。
2. **变更安全网**：修改代码后，单元测试可快速验证是否破坏原有功能，降低重构风险。
3. **天然文档**：测试用例直观展示单元的预期行为（输入、输出、边界条件），比注释更易维护。
4. **提升代码质量**：倒逼开发者编写低耦合、高内聚的代码（否则难以隔离测试）。

## 二、Google C++ 测试套件（GoogleTest）
### 1. GoogleTest 简介与核心特性
GoogleTest（简称GTest）是Google开发的C++测试框架，专为单元测试设计，核心优势：
- 提供**测试固件、参数化测试**等灵活功能，适配复杂测试场景。
- 基于宏和断言实现，预处理阶段自动插入测试逻辑。
- **编译时自动注册测试**：无需手动管理测试用例列表。
- 与Google Mock（GMock）无缝集成：支持模拟外部依赖（如文件、网络），保证测试隔离性。

### 2. GoogleTest 核心宏与用法（以`BankAccount`类为例）
#### 测试对象：`BankAccount`类
```cpp
// bank_account.h
struct BankAccount {
  double balance;
  BankAccount();                // 默认构造（余额0）
  explicit BankAccount(double initial_balance); // 自定义初始余额
  void deposit(double amount);  // 存款（余额增加）
  bool withdraw(double amount); // 取款（成功返回true，余额足够时扣除）
};
```

#### （1）基础测试：`TEST`宏（无共享初始化）
- **用途**：测试独立场景，无需重复初始化/清理资源。
- **语法**：`TEST(测试套件名, 测试用例名) { 测试逻辑 }`。
- **示例**：测试默认构造的账户初始余额为0。
  ```cpp
  // 测试套件名：AccountTest，测试用例名：BankAccountStartsEmpty
  TEST(AccountTest, BankAccountStartsEmpty) {
    BankAccount* account = new BankAccount(); // 手动初始化
    EXPECT_EQ(0, account->balance);           // 断言：预期余额为0
    delete account;                           // 手动清理
  }
  ```

#### （2）共享初始化：`TEST_F`宏（测试固件）
- **用途**：多个测试用例需共享相同初始化/清理逻辑（如创建`BankAccount`对象），避免代码冗余。
- **核心逻辑**：定义测试固件类（继承`testing::Test`），在构造函数中初始化资源，析构函数中清理资源。
- **语法**：`TEST_F(固件类名, 测试用例名) { 测试逻辑 }`。
- **示例**：
  ```cpp
  // 定义测试固件类（共享初始化/清理）
  struct BankAccountTest : testing::Test {
    BankAccount* account;
    // 每个测试用例执行前调用（初始化）
    BankAccountTest() { account = new BankAccount(); }
    // 每个测试用例执行后调用（清理）
    ~BankAccountTest() override { delete account; }
  };
  
  // 使用固件类，无需手动创建/删除account
  TEST_F(BankAccountTest, BankAccountStartsEmpty) {
    EXPECT_EQ(0, account->balance); // 直接使用固件中的account
  }
  
  TEST_F(BankAccountTest, DepositIncreasesBalance) {
    account->deposit(100);
    EXPECT_EQ(100, account->balance);
  }
  ```

#### （3）批量测试：`TEST_P`宏（参数化测试）
- **用途**：同一测试逻辑需验证多组输入输出（如不同初始余额、取款金额的组合），批量生成测试用例。
- **核心步骤**：
  1. 定义参数结构体：存储输入（初始余额、取款金额）和预期输出（最终余额、取款是否成功）。
  2. 定义参数化固件类：继承`测试固件类`和`testing::WithParamInterface<参数结构体>`。
  3. 用`TEST_P`定义测试逻辑：通过`GetParam()`获取当前测试参数。
  4. 实例化测试套件：用`INSTANTIATE_TEST_SUITE_P`指定多组参数。
- **示例**：测试`withdraw`方法的多场景表现。
  ```cpp
  // 1. 定义参数结构体
  struct account_state {
    int initial_balance;  // 输入：初始余额
    int withdraw_amount;  // 输入：取款金额
    int final_balance;    // 预期输出：最终余额
    bool success;         // 预期输出：取款是否成功
    // 重载<<运算符，优化错误提示（可选）
    friend std::ostream& operator<<(std::ostream& os, const account_state& obj) {
      return os << "初始余额:" << obj.initial_balance 
                << " 取款金额:" << obj.withdraw_amount 
                << " 预期余额:" << obj.final_balance 
                << " 预期成功:" << obj.success;
    }
  };
  
  // 2. 定义参数化固件类
  struct WithdrawAccountTest : BankAccountTest, testing::WithParamInterface<account_state> {
    // 初始化当前测试用例的账户余额
    WithdrawAccountTest() {
      account->balance = GetParam().initial_balance;
    }
  };
  
  // 3. 定义参数化测试逻辑
  TEST_P(WithdrawAccountTest, FinalBalance) {
    auto param = GetParam();
    bool actual_success = account->withdraw(param.withdraw_amount);
    // 断言：实际结果与预期一致
    EXPECT_EQ(param.final_balance, account->balance);
    EXPECT_EQ(param.success, actual_success);
  }
  
  // 4. 实例化测试套件（多组参数）
  INSTANTIATE_TEST_SUITE_P(
    WithdrawScenarios,  // 实例名（自定义）
    WithdrawAccountTest, // 固件类名
    testing::Values(
      account_state{100, 50, 50, true},  // 场景1：取款成功（余额足够）
      account_state{100, 200, 100, false}// 场景2：取款失败（余额不足）
    )
  );
  ```
- **效果**：自动生成2个测试用例，分别验证两组参数，批量覆盖核心场景。

### 3. GoogleTest 底层原理
- 宏展开：`TEST`/`TEST_F`/`TEST_P`本质是预处理宏，展开后生成继承自`testing::Test`的测试类。
- 自动注册：测试类通过静态成员函数在编译时自动注册到测试注册表，运行时由GTest框架遍历执行。
- 无需关注细节：开发者只需使用高层宏，框架自动处理注册、执行和结果统计。

## 三、C++ 冰山（The C++ Iceberg）
### 1. 核心概念
C++ 冰山形象地描述了其特性：表面是常用的基础语法（如变量、循环、类），水下是大量易混淆、隐藏的高级特性和陷阱，这些特性虽不常用，但理解后能避免踩坑、提升代码质量。

### 2. 典型“冰山之下”的知识点（示例）
- 语法陷阱：`inline`不代表“内联”、`constexpr`的实际含义与直觉不符、`std::move`不直接“移动”数据。
- 设计缺陷：`vector<bool>`并非真正的`vector`（空间优化导致接口不一致）、“最令人头疼的解析”（Most Vexing Parse）。
- 隐藏规则：`rvalue reference`本质是左值、`C++`并非`C`的超集、`std::remove`不实际删除容器元素。
- 冷知识：`O[arr]`与`arr[O]`等价（数组索引语法糖）、`C++0x`是十六进制数、模板的图灵完备性是意外产物。

### 3. 探索方式

推荐通过互动网站探索：https://victorpoughon.github.io/cppiceberg/，深入了解C++的隐藏特性，避免因知识盲区导致bug。
