#include "IntVector.h"
#include <stdexcept> // 包含std::out_of_range异常定义

/**
 * @brief 默认构造函数实现
 * 初始化容器：元素数量为0，初始容量为4，分配可存储4个int的动态数组
 */
IntVector::IntVector() : _size(0), _capacity(4), _data(new int[_capacity]) {}

/**
 * @brief 析构函数实现
 * 释放_data指向的动态数组内存，避免内存泄漏
 */
IntVector::~IntVector()
{
    delete[] _data; // 释放动态数组（注意用delete[]匹配new[]）
}

/**
 * @brief 扩容函数实现
 * 当_size == _capacity时，将容量翻倍，重新分配内存并拷贝原元素
 */
void IntVector::resize()
{
    // 1. 计算新容量（翻倍）
    size_t new_capacity = _capacity * 2;

    // 2. 分配新的更大内存
    int *new_data = new int[new_capacity];

    // 3. 拷贝原数组元素到新内存
    for (size_t i = 0; i < _size; ++i)
    {
        new_data[i] = _data[i];
    }

    // 4. 释放旧内存
    delete[] _data;

    // 5. 更新指针和容量
    _data = new_data;
    _capacity = new_capacity;
}

/**
 * @brief 向容器末尾添加元素
 * 先检查容量，不足则扩容，再添加元素并更新_size
 */
void IntVector::push_back(const int &value)
{
    // 若容量不足，先扩容
    if (_size == _capacity)
    {
        resize();
    }

    // 添加新元素到末尾，更新元素数量
    _data[_size] = value;
    _size++;
}

/**
 * @brief 带边界检查的元素访问
 * 若索引越界，抛出异常；否则返回元素引用
 */
int &IntVector::at(size_t index)
{
    // 检查索引是否越界（index >= _size即为无效索引）
    if (index >= _size)
    {
        throw std::out_of_range("IntVector::at: index out of bounds");
    }
    return _data[index]; // 返回元素引用
}

/**
 * @brief 无边界检查的元素访问（[]运算符重载）
 * 直接返回指定索引的元素引用，效率更高
 */
int &IntVector::operator[](size_t index)
{
    return _data[index]; // 不检查边界，直接访问
}

/** @brief 返回当前元素数量 */
size_t IntVector::size()
{
    return _size;
}

/** @brief 判断容器是否为空（元素数量为0） */
bool IntVector::empty()
{
    return _size == 0;
}

/** @brief 返回指向首元素的迭代器（指针） */
IntVector::iterator IntVector::begin()
{
    return _data; // 首元素地址
}

/** @brief 返回指向尾后位置的迭代器（指针） */
IntVector::iterator IntVector::end()
{
    return _data + _size; // 最后一个元素的下一位地址
}