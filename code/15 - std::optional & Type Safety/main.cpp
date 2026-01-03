// To compile: g++ -o optional_example main.cpp
// To run:     ./optional_example

/*
 * 该示例展示了C++中std::optional的使用，用于处理可能失败的操作（如除法中的除零情况）
 */

// 包含输入输出流库，用于控制台输出
#include <iostream>
// 包含optional库，提供std::optional类型，用于表示可能存在或不存在的值
#include <optional>

/**
 * 实现整数除法运算，处理除数为0的情况
 * @param numerator 被除数（分子）
 * @param denominator 除数（分母）
 * @return 当除数不为0时，返回包含商的std::optional<int>；当除数为0时，返回std::nullopt表示无有效结果
 */
std::optional<int> divide(int numerator, int denominator)
{
    // 检查除数是否不为0，避免除零错误
    if (denominator != 0)
    {
        // 除数有效，返回除法结果（整数除法）
        return numerator / denominator;
    }
    else
    {
        // 除数为0，返回nullopt表示操作失败
        return std::nullopt;
    }
}

int main()
{
    // 定义被除数a和除数b，此处测试正常除法（除数不为0）
    int a = 10;
    int b = 2;

    // 调用divide函数进行除法，接收返回的optional结果
    std::optional<int> result = divide(a, b);

    // 检查result是否包含有效结果（即除法成功）
    if (result)
    {
        // 若有结果，通过value()方法获取值并输出
        std::cout << "Result: " << result.value() << std::endl;
    }
    else
    {
        // 若无结果（除数为0），输出错误提示
        std::cout << "Division by zero occurred." << std::endl;
    }

    // 测试除数为0的情况，调用divide函数
    result = divide(10, 0);

    // 再次检查结果是否有效
    if (result)
    {
        std::cout << "Result: " << result.value() << std::endl;
    }
    else
    {
        std::cout << "Division by zero occurred." << std::endl;
    }

    return 0;
}