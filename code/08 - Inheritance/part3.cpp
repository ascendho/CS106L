// To compile: g++ part3.cpp -o part3
// To run: ./part3

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
// 关键：update和render声明为虚函数（virtual），支持动态绑定
class Entity
{
protected:
    double x, y, z; // 实体的三维空间坐标
    HitBox hitbox;  // 实体的碰撞盒

public:
    // 虚函数：更新实体状态（基类空实现）
    virtual void update() {};
    // 虚函数：渲染实体（基类空实现）
    virtual void render() {};
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

    // override关键字：明确重写基类的虚函数update
    void update() override
    {
        std::cout << "Updating Player!" << std::endl;
    }

    // override关键字：明确重写基类的虚函数render
    void render() override
    {
        std::cout << "Rendering Player!" << std::endl;
    }
};

// 树类：继承自Entity，代表游戏中的树（静态实体）
class Tree : public Entity
{
public:
    // 重写基类虚函数update
    void update() override
    {
        std::cout << "Updating Tree!" << std::endl;
    }

    // 重写基类虚函数render
    void render() override
    {
        std::cout << "Rendering Tree!" << std::endl;
    }
};

// 投射物类：继承自Entity，代表游戏中的投射物
class Projectile : public Entity
{
    double vx, vy, vz; // 投射物速度（派生类特有成员）

public:
    // 重写基类虚函数update
    void update() override
    {
        std::cout << "Updating Projectile!" << std::endl;
    }

    // 重写基类虚函数render
    void render() override
    {
        std::cout << "Rendering Projectile!" << std::endl;
    }
};

int main()
{
    Player player;   // 玩家实例
    Tree tree;       // 树实例
    Projectile proj; // 投射物实例

    // 容器存储基类指针（Entity*），指向派生类对象，避免对象切片
    std::vector<Entity *> entities{&player, &tree, &proj};

    // 无限循环模拟游戏帧渲染
    while (true)
    {
        std::cout << "Rendering frame..." << std::endl;
        // 遍历所有实体指针，调用更新和渲染方法
        // 关键：虚函数+基类指针触发动态绑定，调用的是指针实际指向的派生类方法
        // 实际效果：正确输出各实体的更新和渲染信息
        for (auto &entity : entities)
        {
            entity->update();
            entity->render();
        }
    }
    return 0;
}