#include <string>   // 引入string头文件，用于使用std::string类型
#include <iostream> // 引入iostream头文件，用于输入输出相关操作

/**
 * @brief 斯坦福ID类，用于存储和管理斯坦福成员的身份信息
 *
 * 该类包含成员的姓名、SUNet ID（斯坦福网络标识）和ID号码，
 * 并提供了构造函数、获取信息的方法以及比较运算符的重载。
 */
class StanfordID
{
private:
    std::string name;  // 存储成员的姓名
    std::string sunet; // 存储成员的SUNet ID（斯坦福网络标识）
    int idNumber;      // 存储成员的ID号码（整数类型）

public:
    /**
     * @brief 带参数的构造函数，用于初始化StanfordID对象
     * @param name 成员姓名
     * @param sunet 成员的SUNet ID
     * @param idNumber 成员的ID号码
     */
    StanfordID(std::string name, std::string sunet, int idNumber);

    /**
     * @brief 默认构造函数，无参数初始化StanfordID对象
     */
    StanfordID();

    /**
     * @brief 获取成员姓名
     * @return 返回存储的姓名（std::string类型）
     * @note 该函数为const成员函数，不会修改对象状态
     */
    std::string getName() const;

    /**
     * @brief 获取成员的SUNet ID
     * @return 返回存储的SUNet ID（std::string类型）
     * @note 该函数为const成员函数，不会修改对象状态
     */
    std::string getSunet() const;

    /**
     * @brief 获取成员的ID号码
     * @return 返回存储的ID号码（int类型）
     * @note 该函数为const成员函数，不会修改对象状态
     */
    int getIdNumber() const;

#ifdef MEMBER_WISE
    /**
     * @brief 成员函数形式的小于运算符重载（条件编译）
     *
     * 当定义了MEMBER_WISE宏时，启用该成员函数，用于比较当前对象与另一个StanfordID对象
     * @param other 另一个待比较的StanfordID对象
     * @return 若当前对象小于other则返回true，否则返回false
     * @note 该函数为const成员函数，不会修改对象状态
     */
    bool operator<(const StanfordID &other) const;
#endif // MEMBER_WISE

    /**
     * @brief 等于运算符重载
     *
     * 判断当前对象与另一个StanfordID对象是否相等
     * @param other 另一个待比较的StanfordID对象
     * @return 若相等则返回true，否则返回false
     * @note 该函数为const成员函数，不会修改对象状态
     */
    bool operator==(const StanfordID &other) const;

    /**
     * @brief 不等于运算符重载
     *
     * 判断当前对象与另一个StanfordID对象是否不相等
     * @param other 另一个待比较的StanfordID对象
     * @return 若不相等则返回true，否则返回false
     * @note 该函数为const成员函数，不会修改对象状态
     */
    bool operator!=(const StanfordID &other) const;

    /**
     * @brief 友元形式的小于运算符重载
     *
     * 作为友元函数，可访问类的私有成员，用于比较两个StanfordID对象
     * @param lhs 左侧的StanfordID对象
     * @param rhs 右侧的StanfordID对象
     * @return 若lhs小于rhs则返回true，否则返回false
     */
    friend bool operator<(const StanfordID &lhs, const StanfordID &rhs);
};