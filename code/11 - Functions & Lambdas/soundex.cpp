#include "soundex.h" // 包含Soundex算法声明
#include <cctype>    // 包含字符处理函数（如toupper）
#include <string>    // 包含字符串操作

/**
 * @brief 辅助函数：获取字符对应的Soundex编码数字
 *
 * 遵循Soundex编码规则：
 * - 元音(A,E,I,O,U)及H,W,Y返回0（后续会被过滤）
 * - B,F,P,V返回1；C,G,J,K,Q,S,X,Z返回2
 * - D,T返回3；L返回4；M,N返回5；R返回6
 * 非字母字符返回特殊值（此处用-1标记）
 *
 * @param c 待编码的字符
 * @return int 对应的编码数字（0-6或-1）
 */
static int getSoundexCode(char c)
{
    // 转换为大写字母统一处理
    char upper_c = std::toupper(c);

    // 检查是否为字母，非字母返回-1
    if (!std::isalpha(upper_c))
    {
        return -1;
    }

    // 按规则返回对应编码
    switch (upper_c)
    {
    case 'B':
    case 'F':
    case 'P':
    case 'V':
        return 1;
    case 'C':
    case 'G':
    case 'J':
    case 'K':
    case 'Q':
    case 'S':
    case 'X':
    case 'Z':
        return 2;
    case 'D':
    case 'T':
        return 3;
    case 'L':
        return 4;
    case 'M':
    case 'N':
        return 5;
    case 'R':
        return 6;
    // 元音及H,W,Y返回0
    case 'A':
    case 'E':
    case 'I':
    case 'O':
    case 'U':
    case 'H':
    case 'W':
    case 'Y':
        return 0;
    default:
        return -1; // 理论上不会触发（已通过isalpha检查）
    }
}

/**
 * @brief 标准Soundex算法实现
 *
 * 编码步骤：
 * 1. 取输入字符串第一个字母作为编码首字符（大写）
 * 2. 忽略非字母字符
 * 3. 对剩余字母转换为Soundex编码数字
 * 4. 移除连续重复的编码数字
 * 5. 过滤掉编码中的0
 * 6. 保留前3位有效数字，不足3位补0，最终编码长度为4
 *
 * @param s 输入字符串
 * @return std::string 4位Soundex编码
 */
std::string soundex(const std::string &s)
{
    // 空字符串直接返回空
    if (s.empty())
    {
        return "";
    }

    std::string result;
    // 保存首字符（大写）
    char first_char = std::toupper(s[0]);
    // 首字符必须是字母，否则返回空（按Soundex标准）
    if (!std::isalpha(first_char))
    {
        return "";
    }
    result += first_char;

    // 记录上一个编码数字（用于去重）
    int last_code = getSoundexCode(first_char);

    // 处理剩余字符
    for (size_t i = 1; i < s.size(); ++i)
    {
        int code = getSoundexCode(s[i]);
        // 跳过非字母（code=-1）或编码为0的字符
        if (code == -1 || code == 0)
        {
            continue;
        }
        // 跳过与前一个编码重复的字符
        if (code == last_code)
        {
            continue;
        }
        // 添加当前编码（转换为字符）
        result += std::to_string(code);
        last_code = code;
        // 达到4位长度时停止（首字符+3位数字）
        if (result.size() >= 4)
        {
            break;
        }
    }

    // 不足4位补0
    while (result.size() < 4)
    {
        result += '0';
    }

    return result;
}