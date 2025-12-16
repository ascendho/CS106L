// To compile: g++ part2.cpp -o part2
// To run: ./part2

#include <iostream>
#include <vector>

// 碰撞盒类：用于存储实体的碰撞检测区域信息
class HitBox
{
public:
    double x, y;          // 碰撞盒左上角坐标
    double width, height; // 碰撞盒宽高
};

// 实体基类：所有游戏实体的抽象基类
// 注意：update和render是普通成员函数（非虚函数）
class Entity
{
protected:
    double x, y, z; // 实体的三维空间坐标
    HitBox hitbox;  // 实体的碰撞盒

public:
    // 更新实体状态（基类空实现）
    void update() {};
    // 渲染实体（基类空实现）
    void render() {};
};

// 玩家类：继承自Entity，代表游戏中的玩家
class Player : public Entity
{
    double hitpoints = 100; // 玩家生命值（派生类特有成员）

public:
    // 玩家受伤害的方法（派生类特有方法）
    void damage(double hp)
    {
        hitpoints -= hp;
    }

    // 重写基类update：玩家特有的更新逻辑
    void update()
    {
        std::cout << "Updating Player!" << std::endl;
    }

    // 重写基类render：玩家特有的渲染逻辑
    void render()
    {
        std::cout << "Rendering Player!" << std::endl;
    }
};

// 树类：继承自Entity，代表游戏中的树（静态实体）
class Tree : public Entity
{
public:
    // 重写基类update：树特有的更新逻辑
    void update()
    {
        std::cout << "Updating Tree!" << std::endl;
    }

    // 重写基类render：树特有的渲染逻辑
    void render()
    {
        std::cout << "Rendering Tree!" << std::endl;
    }
};

// 投射物类：继承自Entity，代表游戏中的投射物
class Projectile : public Entity
{
    double vx, vy, vz; // 投射物速度（派生类特有成员）

public:
    // 重写基类update：投射物特有的更新逻辑
    void update()
    {
        std::cout << "Updating Projectile!" << std::endl;
    }

    // 重写基类render：投射物特有的渲染逻辑
    void render()
    {
        std::cout << "Rendering Projectile!" << std::endl;
    }
};

int main()
{
    Player player;   // 玩家实例
    Tree tree;       // 树实例
    Projectile proj; // 投射物实例

    // 改进点：容器存储基类指针（Entity*），避免了对象切片问题
    // 指针指向实际的派生类对象（Player/Tree/Projectile）
    std::vector<Entity *> entities{&player, &tree, &proj};

    // 无限循环模拟游戏帧渲染
    while (true)
    {
        std::cout << "Rendering frame..." << std::endl;
        // 遍历所有实体指针，调用更新和渲染方法
        // 问题点：基类方法非虚函数，调用由指针类型（Entity*）决定，而非实际对象类型
        // 实际效果：仍调用基类Entity的空实现（无输出）
        for (auto &entity : entities)
        {
            entity->update();
            entity->render();
        }
    }
    return 0;
}