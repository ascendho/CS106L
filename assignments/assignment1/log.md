## SimpleEnroll

1. 在做的过程中发现了一个很隐蔽的问题——在容器遍历的过程中修改容器很可能导致意外的行为！

如；

```c++
// 这个循环：
for (const auto &course : all_courses) {
    // 做某些操作
}

// 实际上等价于：
for (auto it = all_courses.begin(); it != all_courses.end(); ++it) {
    const auto &course = *it;
    // 做某些操作
}

// 当试图删除元素时
std::vector<Course> courses = {course1, course2, course3, course4};

for (const auto &course : courses) {
    if (某个条件) {
        delete_elem_from_vector(courses, course);  // ❌ 危险！
    }
}

// 删除前: [course1][course2][course3][course4]
//                  ^迭代器指向course2

// 删除course2后: [course1][course3][course4][  ]
//                         ^迭代器仍指向这里，但现在是course3！
```

在此题中正确的做法是**先收集要删除的元素，后删除**（其它方法还有反向遍历等），如：

```c++
// 错误：在 range-based for 循环中删除元素
for (const auto &course:all_courses) {
    delete_elem_from_vector(all_courses, course); // ❌ 迭代器失效
}

// 正确：先收集要删除的元素，后删除
std::vector<Course> courses_to_delete;
// 先收集...
for (const auto &course : courses_to_delete) {
    delete_elem_from_vector(all_courses, course); // ✅
}
```



2. 在 C++ 中，判断字符串 “是不是 null” 需要根据字符串的类型来区分，主要分为两种情况：**C 风格字符串（字符指针）**和**C++ 标准库字符串（`std::string`）**

- C 风格字符串本质是字符指针（`char*`），“null” 通常指指针本身为 `nullptr`（或旧标准中的 `NULL`），即指针未指向任何有效内存。判断其 “不是 null”，就是判断指针**不等于 `nullptr`**：

```c++
#include <iostream>

int main() {
    const char* cstr = "hello";  		// 非null的C风格字符串
    const char* nullStr = nullptr;  // null的C风格字符串（指针为空）

    // 判断字符串不是null（指针非空）
    if (cstr != nullptr) {
        std::cout << "cstr不是null" << std::endl;
    }

    if (nullStr != nullptr) {
        std::cout << "nullStr不是null" << std::endl;  // 不会执行
    } else {
        std::cout << "nullStr是null" << std::endl;
    }

    return 0;
}


/* 

注：即使指针非 null（!= nullptr），也可能指向空字符串（如 const char* emptyStr = "";），此时字符串长度为 0，但指针本身不是 null。若需同时判断 “非 null 且非空字符串”，需额外检查：

if (cstr != nullptr && cstr[0] != '\0') {
    // 指针非null，且字符串非空（至少有一个字符）
}

*/
```

- `std::string` 是一个类类型，其对象**永远不会是 `nullptr`**（因为它不是指针）。我们通常说的 “空” 是指字符串中没有字符（长度为 0）。判断 `std::string`“不是空字符串”（即有内容），可以使用 `empty()` 成员函数或 `size()` 成员函数：

```c++
#include <iostream>
#include <string>

int main() {
    std::string str = "hello";  // 非空字符串
    std::string emptyStr;       // 空字符串（默认初始化）

    // 判断字符串不是空（有内容）
    if (!str.empty()) {  				// empty()返回true表示空字符串
        std::cout << "str不是空字符串" << std::endl;
    }

    if (emptyStr.empty()) {
        std::cout << "emptyStr是空字符串" << std::endl;
    }

    // 也可以用size()判断（size()返回0表示空字符串）
    if (str.size() > 0) {
        std::cout << "str不是空字符串" << std::endl;
    }

    return 0;
}
```

> 注意区分 “指针为 null” 和 “字符串为空” 的概念：前者是指针未指向有效内存，后者是指针指向了有效内存但内存中没有字符。



### 总结：**为什么不能在遍历时修改容器？**

1. **迭代器失效**：删除/插入操作使迭代器指向无效内存
2. **跳过元素**：容器结构改变导致遍历逻辑错误
3. **未定义行为**：可能导致程序崩溃或数据损坏



**另**：大家知道怎么关闭 vscode 的 AI 工具 copilot 吗？在做作业的时候它一直自动提示，严重干扰了学习的过程，所以后面我切到 clion 去做了。大家平时在做作业的时候也最好不要借助AI工具。