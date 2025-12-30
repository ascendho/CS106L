#include "../include/StanfordID.h" // 包含对应的头文件，声明StanfordID类的成员和方法

// 被注释的列表初始化构造函数（可能为示例或备用实现）
// 采用成员初始化列表的方式初始化对象的成员变量
// StanfordID::StanfordID(std::string name, std::string sunet, int idNumber) : name(name), sunet(sunet), idNumber(idNumber) {}

/**
 * @brief 默认构造函数
 *
 * 初始化StanfordID对象为默认值：
 * - 姓名默认为"John Appleseed"
 * - SUNet ID默认为"jappleseed"
 * - ID号码默认为00000001（实际存储为整数1）
 */
StanfordID::StanfordID()
{
    name = "John Appleseed";
    sunet = "jappleseed";
    idNumber = 00000001; // 八进制表示，等价于十进制1
}

/**
 * @brief 带参数的构造函数
 *
 * 根据输入参数初始化StanfordID对象，对ID号码进行合法性处理
 * @param name 成员姓名
 * @param sunet 成员的SUNet ID
 * @param idNumber 成员的ID号码（若为负数则自动设为0）
 */
StanfordID::StanfordID(std::string name, std::string sunet, int idNumber)
{
    this->name = name; // 使用this指针区分成员变量和参数
    this->sunet = sunet;
    // 确保ID号码非负，若输入负数则设为0
    if (idNumber >= 0)
    {
        this->idNumber = idNumber;
    }
    else
    {
        this->idNumber = 0;
    }
}

/**
 * @brief 获取成员姓名
 * @return 返回存储的姓名（std::string类型）
 * @note 该函数为const成员函数，不修改对象状态
 */
std::string StanfordID::getName() const
{
    return name;
}

/**
 * @brief 获取成员的SUNet ID
 * @return 返回存储的SUNet ID（std::string类型）
 * @note 该函数为const成员函数，不修改对象状态
 */
std::string StanfordID::getSunet() const
{
    return sunet;
}

/**
 * @brief 获取成员的ID号码
 * @return 返回存储的ID号码（int类型）
 * @note 该函数为const成员函数，不修改对象状态
 */
int StanfordID::getIdNumber() const
{
    return idNumber;
}

/* 成员函数形式的运算符重载 */
#ifdef MEMBER_WISE // 当定义MEMBER_WISE宏时，启用成员函数形式的<运算符

/**
 * @brief 成员函数形式的小于运算符重载
 *
 * 基于ID号码比较两个StanfordID对象的大小
 * @param other 待比较的另一个StanfordID对象
 * @return 若当前对象的ID号码小于other的ID号码则返回true，否则返回false
 * @note 该函数为const成员函数，不修改对象状态
 */
bool StanfordID::operator<(const StanfordID &other) const
{
    return idNumber < other.idNumber;
}
#endif // MEMBER_WISE 结束MEMBER_WISE宏的条件编译

/**
 * @brief 等于运算符重载
 *
 * 基于ID号码判断两个StanfordID对象是否相等
 * @param other 待比较的另一个StanfordID对象
 * @return 若两者ID号码相等则返回true，否则返回false
 * @note 该函数为const成员函数，不修改对象状态
 */
bool StanfordID::operator==(const StanfordID &other) const
{
    return idNumber == other.idNumber;
}

/**
 * @brief 不等于运算符重载
 *
 * 基于ID号码判断两个StanfordID对象是否不相等（对==运算符结果取反）
 * @param other 待比较的另一个StanfordID对象
 * @return 若两者ID号码不相等则返回true，否则返回false
 * @note 该函数为const成员函数，不修改对象状态
 */
bool StanfordID::operator!=(const StanfordID &other) const
{
    return !(idNumber == other.idNumber); // 复用==运算符的逻辑
}

/**
 * 说明：并非所有运算符都能被重载（例如@）
 * 下方为尝试重载@运算符的示例（已注释，实际无法通过编译）
 */
// void StanfordID::operator@(const StanfordID& other) const {
//     std::cout << "It works!" << std::endl;
// }

/* 非成员函数形式的运算符重载 */

#ifndef MEMBER_WISE // 当未定义MEMBER_WISE宏时，启用非成员函数形式的<运算符

/**
 * @brief 非成员函数形式的小于运算符重载
 *
 * 基于ID号码比较两个StanfordID对象的大小（通过公有方法获取ID号码）
 * @param lhs 左侧的StanfordID对象
 * @param rhs 右侧的StanfordID对象
 * @return 若lhs的ID号码小于rhs的ID号码则返回true，否则返回false
 */
bool operator<(const StanfordID &lhs, const StanfordID &rhs)
{
    return lhs.getIdNumber() < rhs.getIdNumber();
}
#endif // MEMBER_WISE 结束MEMBER_WISE宏的条件编译