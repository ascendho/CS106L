#include <iostream>             // 引入iostream头文件，用于标准输入输出操作
#include "include/StanfordID.h" // 引入StanfordID类的头文件，用于使用该类定义对象

/**
 * @brief 主函数，程序的入口点
 *
 * 功能：创建两个StanfordID对象，比较它们的ID号码大小并输出结果
 */
int main()
{
    // 创建StanfordID对象jacob，初始化姓名为"Jacob Roberts-Baca"、SUNet ID为"jtrb"、ID号码为12345678
    StanfordID jacob{"Jacob Roberts-Baca", "jtrb", 12345678};
    // 创建StanfordID对象fabio，初始化姓名为"Fabio Ibanez"、SUNet ID为"fabioi"、ID号码为87654321
    StanfordID fabio{"Fabio Ibanez", "fabioi", 87654321};

    // 比较两个对象的ID号码大小（通过重载的<运算符，比较依据为对象的idNumber成员）
    if (jacob < fabio)
    {
        // 若jacob的ID号码小于fabio，输出对应信息
        std::cout << jacob.getName() << " has a lower ID number than " << fabio.getName() << std::endl;
    }
    else
    {
        // 否则，输出fabio的ID号码更小的信息
        std::cout << fabio.getName() << " has a lower ID number than " << jacob.getName() << std::endl;
    }

    // 将两个对象的比较结果（布尔值）存储在result变量中
    auto result = jacob < fabio;

    // 程序正常结束，返回0
    return 0;
}