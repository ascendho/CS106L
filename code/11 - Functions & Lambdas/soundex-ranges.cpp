#include "soundex.h" // 包含Soundex算法声明

#include <algorithm> // 包含标准算法（如find_if、unique_copy）
#include <iterator>  // 包含迭代器工具（如back_inserter）
#include <map>       // 包含map容器（存储字符-编码映射）
#include <ranges>    // 包含C++20 ranges视图（简化序列处理）

/**
 * @brief 将单个字符编码为Soundex对应的字符（ranges版本辅助函数）
 *
 * 映射规则与标准Soundex一致，返回字符形式的编码（如'1'、'2'等，0用'0'表示）
 * @param c 待编码的字符
 * @return char 对应的编码字符（'0'-'6'）
 */
static char soundexEncode(char c)
{
    // 静态映射表：字符到Soundex编码的对应关系（大写字母）
    static const std::map<char, char> encoding = {
        {'A', '0'}, {'E', '0'}, {'I', '0'}, {'O', '0'}, {'U', '0'}, {'H', '0'}, {'W', '0'}, {'Y', '0'}, {'B', '1'}, {'F', '1'}, {'P', '1'}, {'V', '1'}, {'C', '2'}, {'G', '2'}, {'J', '2'}, {'K', '2'}, {'Q', '2'}, {'S', '2'}, {'X', '2'}, {'Z', '2'}, {'D', '3'}, {'T', '3'}, {'L', '4'}, {'M', '5'}, {'N', '5'}, {'R', '6'}};
    // 转换为大写后查找映射（若为非字母，此处可能抛出异常，需确保输入已过滤）
    return encoding.at(std::toupper(c));
}

/**
 * @brief 过滤函数：判断字符是否不是'0'
 *
 * 用于ranges过滤步骤，移除编码中的'0'字符
 * @param c 待判断的字符
 * @return bool 若字符不是'0'则返回true，否则返回false
 */
static bool notZero(char c)
{
    return c != '0';
}

/**
 * @brief 使用C++20 ranges库实现的Soundex算法
 *
 * 编码步骤（基于ranges视图流水线）：
 * 1. 取输入字符串第一个字母作为首字符（大写）
 * 2. 过滤出所有字母字符
 * 3. 将字母转换为Soundex编码字符
 * 4. 移除连续重复的编码字符
 * 5. 替换首字符为原始第一个字母的大写形式
 * 6. 过滤掉'0'字符，取前4位（不足4位自然截断）
 *
 * @param s 输入字符串
 * @return std::string 生成的Soundex编码
 */
std::string soundexRanges(const std::string &s)
{
    namespace rv = std::ranges::views; // 简化ranges视图命名空间

    // 找到输入中第一个字母（作为编码首字符），若没有字母则返回空
    auto first_letter_it = std::ranges::find_if(s, ::isalpha);
    if (first_letter_it == s.end())
    {
        return "";
    }
    char first = *first_letter_it;

    // 步骤1-4：过滤字母->转换编码->去重连续重复
    auto encoded_view = s | rv::filter(::isalpha)       // 仅保留字母
                        | rv::transform(soundexEncode); // 转换为Soundex编码字符
    std::string encoded;
    std::ranges::unique_copy(encoded_view, std::back_inserter(encoded)); // 去重连续重复

    // 替换首字符为原始第一个字母的大写形式
    if (!encoded.empty())
    {
        encoded[0] = std::toupper(first);
    }

    // 步骤5-6：过滤'0'->取前4位->转换为字符串
    return encoded | rv::filter(notZero)     // 过滤掉'0'
           | rv::take(4)                     // 截取前4个字符
           | std::ranges::to<std::string>(); // 转换为string返回
}