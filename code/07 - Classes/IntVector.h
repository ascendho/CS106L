#pragma once // 防止头文件被重复包含

#include <cstddef> // 包含size_t类型的定义

/**
 * @brief 自定义动态整数容器类，类似简化版的std::vector<int>
 * 功能：管理整数元素的动态存储、添加、访问及遍历
 */
class IntVector
{
public:
    /**
     * @brief 迭代器类型定义，本质是int指针，用于遍历容器元素
     * 支持++、*、==等操作，适配范围for循环
     */
    using iterator = int *;

    /** @brief 默认构造函数，初始化空容器 */
    IntVector();

    /** @brief 析构函数，释放动态分配的内存，防止内存泄漏 */
    ~IntVector();

    /**
     * @brief 向容器末尾添加元素
     * @param value 待添加的整数（通过const引用传递，避免拷贝开销）
     * 若当前容量不足，会自动调用resize()扩容
     */
    void push_back(const int &value);

    /**
     * @brief 访问指定索引的元素（带边界检查）
     * @param index 元素索引（从0开始）
     * @return 索引对应元素的引用（可修改）
     * @warning 若index越界（>=_size），应抛出std::out_of_range异常
     */
    int &at(size_t index);

    /**
     * @brief 重载[]运算符，访问指定索引的元素（无边界检查）
     * @param index 元素索引（从0开始）
     * @return 索引对应元素的引用（可修改）
     * @note 效率更高，但越界访问会导致未定义行为（如内存错误）
     */
    int &operator[](size_t index);

    /** @brief 获取当前容器中元素的实际数量 */
    size_t size();

    /** @brief 判断容器是否为空（元素数量为0） */
    bool empty();

    /** @brief 返回指向容器首元素的迭代器 */
    iterator begin();

    /** @brief 返回指向容器尾后位置（最后一个元素的下一位）的迭代器 */
    iterator end();

private:
    size_t _size;     // 当前元素数量（实际存储的元素个数）
    size_t _capacity; // 容器容量（最多可存储的元素个数，不重新分配内存的情况下）
    int *_data;       // 指向动态分配的整数数组，用于存储元素

    /**
     * @brief 私有扩容函数，当元素数量达到容量上限时调用
     * 扩容策略：将容量翻倍，重新分配内存并拷贝原元素，释放旧内存
     */
    void resize();
};