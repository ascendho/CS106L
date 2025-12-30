#ifndef SOUNDEX_H // 防止头文件重复包含的宏定义
#define SOUNDEX_H

#include <string> // 包含字符串类型定义

/**
 * @brief 标准Soundex算法实现（非ranges版本）
 *
 * 将输入字符串（通常为人名）转换为Soundex编码，编码规则遵循传统Soundex标准
 * @param s 输入字符串（待编码的人名）
 * @return std::string 生成的4位Soundex编码
 */
std::string soundex(const std::string &s);

/**
 * @brief 使用C++20 ranges库实现的Soundex算法
 *
 * 功能与soundex相同，但内部使用ranges视图简化序列处理流程
 * @param s 输入字符串（待编码的人名）
 * @return std::string 生成的4位Soundex编码
 */
std::string soundexRanges(const std::string &s);

#endif // SOUNDEX_H  // 头文件宏定义结束