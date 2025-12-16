#pragma once // 防止头文件重复包含
#include <string>

/**
 * @brief 学生身份信息类，用于存储和管理学生的基本身份数据
 * 包含姓名、SUNet ID和学号等信息，并提供访问和修改这些信息的接口
 */
class StudentID
{
protected:
    std::string name;  ///< 学生姓名（protected成员，允许派生类访问）
    std::string sunet; ///< 学生的SUNet ID（斯坦福大学网络身份标识，protected成员）
    int idNumber;      ///< 学生学号（整数类型，protected成员）

public:
    /**
     * @brief 带参数的构造函数，初始化学生的姓名、SUNet ID和学号
     * @param name 学生姓名
     * @param sunet 学生的SUNet ID
     * @param idNumber 学生学号
     */
    StudentID(std::string name, std::string sunet, int idNumber);

    /**
     * @brief 默认构造函数，无参数，初始化默认的学生信息
     */
    StudentID();

    /**
     * @brief 析构函数，清理对象资源（此处主要用于演示析构过程）
     */
    ~StudentID();

    // Getter函数：获取私有成员变量的值
    /**
     * @brief 获取学生姓名
     * @return 学生姓名的字符串
     */
    std::string getName();

    /**
     * @brief 获取学生的SUNet ID
     * @return SUNet ID的字符串
     */
    std::string getSunet();

    /**
     * @brief 获取学生学号
     * @return 学号的整数
     */
    int getIdNumber();

    // Setter函数：修改私有成员变量的值
    /**
     * @brief 设置学生姓名
     * @param name 新的学生姓名
     */
    void setName(std::string name);

    /**
     * @brief 设置学生的SUNet ID
     * @param sunet 新的SUNet ID
     */
    void setSunet(std::string sunet);

    /**
     * @brief 设置学生学号
     * @param idNumber 新的学号
     */
    void setIdNumber(int idNumber);
};