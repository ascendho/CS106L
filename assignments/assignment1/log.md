## SimpleEnroll

在做的过程中发现了一个很隐蔽的问题——在容器遍历的过程中修改容器很可能导致意外的行为！

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



### 总结：**为什么不能在遍历时修改容器？**

1. **迭代器失效**：删除/插入操作使迭代器指向无效内存
2. **跳过元素**：容器结构改变导致遍历逻辑错误
3. **未定义行为**：可能导致程序崩溃或数据损坏



**另**：大家知道怎么关闭 vscode 的 AI 工具 copilot 吗？在做作业的时候它一直自动提示，严重干扰了学习的过程，所以后面我切到 clion 去做了。大家平时在做作业的时候也最好不要借助AI工具。