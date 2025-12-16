#include <iostream>
#include "StudentID.h"

/**
 * @brief 宏定义，控制构造函数的初始化方式
 * 当LIST_INITIALIZATION为1时，使用成员初始化列表；为0时，使用构造函数体内赋值
 */
#define LIST_INITIALIZATION 0

/// @brief 默认构造函数实现，初始化默认的学生信息
StudentID::StudentID()
{
    name = "John Appleseed"; // 默认姓名
    sunet = "jappleseed";    // 默认SUNet ID
    idNumber = 00000001;     // 默认学号（注意：前导0不影响实际值，仅为演示）
}

/// @brief 析构函数实现，输出提示信息表示析构过程被调用
StudentID::~StudentID()
{
    std::cout << "Destructor is called" << std::endl;
}

/**
 * @brief 条件编译：根据LIST_INITIALIZATION选择构造函数的实现方式
 * 两种方式均用于初始化对象成员，但成员初始化列表（LIST_INITIALIZATION=1）效率更高
 */
#if LIST_INITIALIZATION
// 使用成员初始化列表初始化成员变量（直接在初始化阶段赋值，效率更高）
StudentID::StudentID(std::string name, std::string sunet, int idNumber)
    : name{name}, sunet{sunet}, idNumber{idNumber}
{
}
#else

/// @brief 带参数的构造函数实现，通过参数初始化成员变量
/// @param name 学生姓名
/// @param sunet 学生的SUNet ID
/// @param idNumber 学生学号（若为负数则自动设为0）
StudentID::StudentID(std::string name, std::string sunet, int idNumber)
{
    this->name = name;   // 使用this指针区分成员变量和参数
    this->sunet = sunet; // this->sunet表示成员变量，参数sunet为输入值

    // 对学号进行合法性检查：若为负数则设为0
    if (idNumber >= 0)
    {
        this->idNumber = idNumber;
    }
    else
    {
        this->idNumber = 0;
    }
}

/// @brief 获取学生姓名的getter函数实现
/// @return 成员变量name的值
std::string StudentID::getName()
{
    return name;
}

/// @brief 获取SUNet ID的getter函数实现
/// @return 成员变量sunet的值
std::string StudentID::getSunet()
{
    return sunet;
}

/// @brief 获取学号的getter函数实现
/// @return 成员变量idNumber的值
int StudentID::getIdNumber()
{
    return idNumber;
}

/// @brief 修改学生姓名的setter函数实现
/// @param name 新的姓名，将覆盖原有name的值
void StudentID::setName(std::string name)
{
    this->name = name; // 通过this指针更新成员变量
}

/// @brief 修改SUNet ID的setter函数实现
/// @param sunet 新的SUNet ID，将覆盖原有sunet的值
void StudentID::setSunet(std::string sunet)
{
    this->sunet = sunet; // 通过this指针更新成员变量
}

/// @brief 修改学号的setter函数实现
/// @param id 新的学号，将覆盖原有idNumber的值（此处未做合法性检查，直接赋值）
void StudentID::setIdNumber(int id)
{
    this->idNumber = id; // 通过this指针更新成员变量
}
#endif