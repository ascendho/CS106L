// To compile: g++ -std=c++20 main.cpp StudentID.cpp IntVector.cpp -o main
// To run: ./main

#include <iostream>       // 包含标准输入输出流库，用于控制台输出
#include "StudentID.h"    // 包含StudentID类的头文件，提供学生身份信息类的定义
#include "IntVector.h"    // 包含IntVector类的头文件，提供动态整数容器类的定义（当前示例未使用，可能为后续扩展预留）

/**
 * @brief 创建StudentID对象并打印其信息的示例函数
 * 功能：实例化一个StudentID对象，设置学生的姓名、SUNet ID和学号，并通过成员函数获取这些信息并输出到控制台
 */
void createStudentID()
{
    // 实例化StudentID对象，参数分别为：姓名、SUNet ID、学号
    StudentID sid{"Jacob Roberts-Baca", "jtrb", 545};
    // 输出学生姓名（通过getName()成员函数获取）
    std::cout << "Name: " << sid.getName() << std::endl;
    // 输出学生的SUNet ID（通过getSunet()成员函数获取）
    std::cout << "Sunet: " << sid.getSunet() << std::endl;
    // 输出学生学号（通过getIdNumber()成员函数获取）
    std::cout << "ID Number: " << sid.getIdNumber() << std::endl;
}

/**
 * @brief 程序入口函数
 * 功能：调用createStudentID()函数执行学生信息的创建与打印，演示StudentID类的基本使用
 * @return int 程序执行成功返回0
 */
int main()
{
    // 调用函数创建并展示学生信息
    createStudentID();
    // 程序正常结束
    return 0;
}