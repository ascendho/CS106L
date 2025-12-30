#include <algorithm>  // 包含标准算法（如ranges::sample）
#include <chrono>     // 包含时间测量工具（计时用）
#include <iostream>   // 包含输入输出流
#include <fstream>    // 包含文件流（读取文件）
#include <functional> // 包含函数对象（std::function）
#include <map>        // 包含map容器（存储算法名称与实现的映射）
#include <vector>     // 包含vector容器（存储字符串列表）
#include <random>     // 包含随机数生成器（采样用）

#include "soundex.h" // 包含Soundex算法声明

/**
 * @brief 算法映射表：存储算法名称到实现函数的映射
 *
 * key为算法名称字符串，value为对应的Soundex编码函数（接受string返回string）
 */
static const std::map<std::string, std::function<std::string(const std::string &)>> algorithms = {
    {"soundex", soundex},            // 标准Soundex实现
    {"soundexRanges", soundexRanges} // ranges版本Soundex实现
};

/**
 * @brief 从命令行参数获取用户选择的算法
 *
 * 检查参数有效性：若未提供参数或参数无效，打印可选算法并退出程序
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return std::vector<std::string> 有效的算法名称列表
 */
std::vector<std::string> getAlgorithms(int argc, char *argv[])
{
    //  lambda表达式：打印所有可用算法选项
    auto printOptions = [&]()
    {
        std::cerr << "Options are: " << std::endl;
        for (const auto &[name, _] : algorithms) // 结构化绑定遍历map
        {
            std::cerr << "  - " << name << std::endl;
        }
    };

    // 将命令行参数（除程序名外）转换为字符串向量
    std::vector<std::string> arguments(argv + 1, argv + argc);
    if (arguments.empty()) // 无参数时提示并退出
    {
        std::cerr << "No arguments provided. ";
        printOptions();
        exit(1);
    }

    // 检查所有参数是否为有效算法名称
    for (const auto &algo : arguments)
    {
        if (algorithms.find(algo) == algorithms.end()) // 未找到算法
        {
            std::cerr << "Unknown algorithm: '" << algo << "'. ";
            printOptions();
            exit(1);
        }
    }

    return arguments;
}

/**
 * @brief 从文件读取所有行到字符串向量
 *
 * 逐行读取文件内容，每行作为向量的一个元素
 * @param path 文件路径（此处默认读取"names.txt"）
 * @return std::vector<std::string> 包含文件所有行的向量
 */
auto readLines(const std::string &path)
{
    std::ifstream file(path);       // 打开文件流
    std::vector<std::string> lines; // 存储行的向量

    // 逐行读取文件，直到文件结束
    for (std::string line; std::getline(file, line);)
    {
        lines.push_back(line);
    }

    return lines;
}

/**
 * @brief 从名字列表中随机采样n个名字
 *
 * 使用随机数生成器从输入向量中无放回采样n个元素
 * @param names 待采样的名字列表
 * @param n 采样数量
 * @return std::vector<std::string> 采样结果
 */
auto sampleN(const std::vector<std::string> &names, size_t n)
{
    std::random_device rd;              // 随机设备（种子源）
    std::mt19937 gen(rd());             // 梅森旋转算法随机数生成器
    std::vector<std::string> result(n); // 存储采样结果
    // 使用ranges::sample进行采样（从names中采样n个元素到result）
    std::ranges::sample(names, result.begin(), result.size(), gen);
    return result;
}

/**
 * @brief 显示指定算法对采样名字的编码结果
 *
 * 模板函数，可接受任意Soundex编码函数，打印每个名字的编码结果
 * @tparam Soundex 编码函数类型（需满足std::function<std::string(const std::string&)>）
 * @param names 待编码的名字列表
 * @param algo 算法名称（用于打印）
 * @param soundex 编码函数实例
 */
template <typename Soundex>
void showSoundexCodes(const std::vector<std::string> &names, const std::string &algo, Soundex soundex)
{
    std::cout << "Soundex output for '" << algo << "'" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    // 遍历名字列表，打印每个名字及其编码
    for (const auto &name : names)
    {
        std::cout << name << " -> " << soundex(name) << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief 测量指定算法的执行时间
 *
 * 多次运行算法并计算平均执行时间（纳秒级）
 * @tparam Soundex 编码函数类型
 * @param names 待编码的名字列表（用于批量测试）
 * @param algo 算法名称（用于打印）
 * @param soundex 编码函数实例
 * @param rounds 重复运行轮数（默认8轮，用于减少计时误差）
 */
template <typename Soundex>
void timeSoundex(
    const std::vector<std::string> &names,
    const std::string &algo,
    Soundex soundex,
    size_t rounds = 8)
{
    using namespace std::chrono; // 简化时间命名空间

    auto start = high_resolution_clock::now(); // 记录开始时间

    // 多轮运行算法（减少单次测量误差）
    for (size_t i = 0; i < rounds; ++i)
    {
        for (const auto &name : names)
        {
            soundex(name); // 执行编码
        }
    }

    auto end = high_resolution_clock::now(); // 记录结束时间
    // 计算总耗时（纳秒）
    auto duration = duration_cast<nanoseconds>(end - start).count();
    auto num_rounds = names.size() * rounds;   // 总执行次数
    auto average_time = duration / num_rounds; // 平均每次执行时间

    // 打印计时结果
    std::cout << "Ran algorithm '" << algo << "' " << num_rounds << " times" << std::endl;
    std::cout << "Average time per call: " << average_time << " nanoseconds\n"
              << std::endl;
}

/**
 * @brief 主函数：程序入口
 *
 * 流程：
 * 1. 解析命令行参数，获取用户选择的算法
 * 2. 从文件读取名字列表
 * 3. 随机采样名字并添加测试用例，显示编码结果
 * 4. 对选中的算法进行性能计时并输出结果
 *
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return int 程序退出码（0表示成功）
 */
int main(int argc, char *argv[])
{
    // 1. 获取用户选择的算法
    auto choices = getAlgorithms(argc, argv);

    // 2. 读取名字文件
    auto names = readLines("names.txt");

    // 3. 生成采样名字（5个随机+1个测试用例"Roberts-"）
    auto sample = sampleN(names, 5);
    sample.push_back("Roberts-"); // 包含特殊字符的测试用例

    // 4. 对每个选中的算法，显示编码结果并计时
    for (const auto &algo : choices)
    {
        showSoundexCodes(sample, algo, algorithms.at(algo)); // 显示编码
        timeSoundex(names, algo, algorithms.at(algo));       // 计时
    }

    return 0;
}