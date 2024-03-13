#pragma once

#include "CARGO.hpp"
#include "Path.hpp"

class Robot {
public:
    int goods{};           // 是否携带货物
    int x{}, y{};          // 机器人的x，y坐标
    int status{};          // 机器人是否处于运行状态
    int id{};              // 机器人的编号
    double cargotoberth{}; // 机器人携带的货物到泊位的距离
    int berthid{-1};
    Cargo cargo{default_cargo}; // 机器人的货物
    //vector<pair<int, int>> road;
    Robotlib::Path path;// 机器人的路径

    Robot() = default;

    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }

    // 机器人获取物品
    void getThings(int x, int y);

    // 机器人放下物品
    void putThings(int x, int y);

    // 设置机器人要拿取什么物品
    void setGoal(Cargo c, double dis, int brenth_id, Robotlib::Path &r);

    // 机器人移动
    void move(int x, int y);

    // 重置机器人
    void Reset(bool complete);
};
