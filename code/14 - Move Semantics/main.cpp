#include <iostream>   // 用于输入输出操作
#include <functional> // 用于函数对象（std::function）支持
#include <cmath>      // 数学函数库（此处未直接使用，可能为预留）
#include <utility>    // 提供实用工具（如移动语义支持，此处用于基础类型操作）
#include <chrono>     // 用于高精度计时

/**
 * @brief 照片类，用于模拟图像数据的存储与管理
 *
 * 包含照片的宽、高、像素数据，支持拷贝语义和（可选的）移动语义，
 * 并通过日志输出跟踪对象的创建、拷贝、移动和销毁过程。
 */
class Photo
{
public:
    /**
     * @brief 构造函数，初始化照片的宽、高和标签
     * @param width 照片宽度
     * @param height 照片高度
     * @param tag 日志标签（用于区分不同对象的操作日志），默认 nullptr（不输出日志）
     */
    Photo(int width, int height, const char *tag = nullptr);

    /**
     * @brief 拷贝构造函数（深拷贝）
     * @param other 被拷贝的Photo对象
     */
    Photo(const Photo &other);

    /**
     * @brief 拷贝赋值运算符（深拷贝）
     * @param other 被赋值的Photo对象
     * @return 引用当前对象（支持链式赋值）
     */
    Photo &operator=(const Photo &other);

    /**
     * @brief 析构函数，释放动态分配的像素数据
     */
    ~Photo();

#ifdef ENABLE_MOVE_SEMANTICS
    // 移动特殊成员函数（仅在定义ENABLE_MOVE_SEMANTICS宏时启用）

    /**
     * @brief 移动构造函数
     * @param other 被移动的Photo对象（右值引用）
     */
    Photo(Photo &&other);

    /**
     * @brief 移动赋值运算符
     * @param other 被移动的Photo对象（右值引用）
     * @return 引用当前对象
     */
    Photo &operator=(Photo &&other);
#endif

private:
    int width;  // 照片宽度
    int height; // 照片高度
    int *data;  // 动态分配的像素数据数组（存储width*height个int像素）

    /**
     * @brief 日志输出工具函数
     * @return 若tag不为空则返回带标签的cout流，否则返回空流（不输出）
     */
    std::ostream &log() const;
    const char *tag = nullptr; // 日志标签，用于区分不同对象的操作日志
};

/**
 * @brief 构造函数实现
 *
 * 初始化宽、高，动态分配像素数据数组，并输出构造日志
 */
Photo::Photo(int width, int height, const char *tag)
    : width(width) // 初始化列表：设置宽度
      ,
      height(height) // 初始化列表：设置高度
      ,
      data(new int[width * height]) // 分配像素数据内存（width*height个int）
      ,
      tag(tag) // 初始化日志标签
{
    log() << "Photo(" << width << ", " << height << ")" << std::endl; // 输出构造日志
}

/**
 * @brief 拷贝构造函数实现（深拷贝）
 *
 * 复制源对象的宽、高，重新分配像素数据内存，并复制源对象的像素数据
 */
Photo::Photo(const Photo &other)
    : width(other.width) // 复制宽度
      ,
      height(other.height) // 复制高度
      ,
      data(new int[width * height]) // 为当前对象分配新的像素内存
{
    log() << "Photo(const Photo&)" << std::endl; // 输出拷贝构造日志
    log() << "  ↪ copying " << width << "x" << height << " pixels..." << std::endl;
    // 深拷贝：将源对象的像素数据复制到当前对象的内存中
    std::copy(other.data, other.data + width * height, data);
}

/**
 * @brief 拷贝赋值运算符实现（深拷贝）
 *
 * 先检查自赋值，释放当前对象原有像素数据，再复制源对象的宽、高和像素数据
 */
Photo &Photo::operator=(const Photo &other)
{
    log() << "Photo::operator=(const Photo&)" << std::endl; // 输出拷贝赋值日志

    // 自赋值检查：若当前对象与源对象是同一个，直接返回（避免无效操作）
    if (this == &other)
        return *this;

    log() << "  ↪ cleaning up " << width << "x" << height << " pixels..." << std::endl;
    delete[] data; // 释放当前对象原有像素数据（避免内存泄漏）

    // 复制源对象的宽、高
    width = other.width;
    height = other.height;
    // 分配新的像素内存
    data = new int[width * height];

    log() << "  ↪ copying " << width << "x" << height << " pixels..." << std::endl;
    // 深拷贝像素数据
    std::copy(other.data, other.data + width * height, data);
    return *this;
}

/**
 * @brief 析构函数实现
 *
 * 输出销毁日志，并释放动态分配的像素数据数组
 */
Photo::~Photo()
{
    log() << "~Photo()" << std::endl; // 输出析构日志
    delete[] data;                    // 释放像素数据内存
}

#ifdef ENABLE_MOVE_SEMANTICS

/**
 * @brief 移动构造函数实现
 *
 * 窃取源对象的资源（像素数据指针），并将源对象的指针置空（避免二次释放）
 */
Photo::Photo(Photo &&other)
    : width(other.width) // 窃取源对象的宽度
      ,
      height(other.height) // 窃取源对象的高度
      ,
      data(other.data) // 窃取源对象的像素数据指针
{
    log() << "Photo(Photo&&)" << std::endl; // 输出移动构造日志
    other.data = nullptr;                   // 源对象指针置空，避免析构时释放已转移的资源
}

/**
 * @brief 移动赋值运算符实现
 *
 * 先释放当前对象原有资源，再窃取源对象的资源，并将源对象指针置空
 */
Photo &Photo::operator=(Photo &&other)
{
    log() << "Photo::operator=(Photo&&)" << std::endl; // 输出移动赋值日志

    // 释放当前对象原有像素数据
    delete[] data;

    // 窃取源对象的资源
    width = other.width;
    height = other.height;
    data = other.data;
    other.data = nullptr; // 源对象指针置空

    return *this;
}

#endif

/**
 * @brief 日志输出函数实现
 *
 * 根据tag是否为空，返回带标签的输出流或空流（不输出日志）
 */
std::ostream &Photo::log() const
{
    static std::ostream null(nullptr); // 空输出流（不输出任何内容）
    if (!tag)
        return null;                            // 若tag为空，返回空流
    return std::cout << "[" << tag << "] \t\t"; // 否则返回带标签的cout
}

/**
 * @brief 模板函数：计算函数多次调用的平均耗时
 * @tparam Func 函数类型（可调用对象）
 * @param f 待计时的函数
 * @param invocations 调用次数
 */
template <typename Func>
void time_func(Func f, size_t invocations)
{
    // 临时禁用cout输出（避免日志干扰计时结果）
    std::cout.setstate(std::ios_base::failbit);

    auto start = std::chrono::high_resolution_clock::now(); // 记录开始时间
    // 多次调用函数f
    for (size_t i = 0; i < invocations; ++i)
        f();
    auto end = std::chrono::high_resolution_clock::now(); // 记录结束时间

    // 恢复cout输出状态
    std::cout.clear();

    // 计算并输出平均耗时（纳秒）
    std::cout << "Average time spent per call: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / invocations
              << "ns" << std::endl;
}

/**
 * @brief 创建并返回一个Photo对象
 * @return 一个500x500的Photo对象，标签为"take_photo()"
 */
Photo take_photo()
{
    Photo photo(500, 500, "take_photo()"); // 创建局部Photo对象
    return photo;                          // 返回对象（可能触发拷贝或移动，取决于是否启用移动语义）
}

/**
 * @brief 演示函数：展示Photo对象的赋值过程
 *
 * 创建一个selfie对象，然后将take_photo()的返回值赋值给它
 */
void run_example()
{
    Photo selfie(0, 0, "selfie"); // 创建初始selfie对象（0x0大小）
    selfie = take_photo();        // 赋值操作（可能触发拷贝赋值或移动赋值）
}

/**
 * @brief 主函数：程序入口
 *
 * 先运行示例展示对象操作流程，再计时多次调用的平均耗时
 */
int main()
{
    std::cout << "Example run:\n\n";
    run_example(); // 运行示例，输出对象创建、赋值、销毁的日志
    std::cout << "\n\n";

    constexpr size_t invocations = 10000; // 计时调用次数
    std::cout << "Timing " << invocations << " attempts to move/copy a Photo..." << std::endl;
    time_func(run_example, invocations); // 计时并输出平均耗时
    return 0;
}