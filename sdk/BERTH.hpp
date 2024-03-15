#pragma once

#include <list>

class Berth {
public:
    int x{};                      // 泊位的x
    int y{};                      // 泊位的y
    int id{};                     // 泊位的编号
    int transport_time{};         // 到达虚拟点的时间
    int loading_speed{};          // 装货速度
    queue<pair<int, int>> things; // 泊位上的物品,储存的是相对坐标
    std::list<int> boatids;            // 停靠或者准备停靠的船只编号

    Berth() = default;

    Berth(int x, int y, int transport_time, int loading_speed)
            : x(x), y(y), transport_time(transport_time), loading_speed(loading_speed) {}

    // 泊位向船装货
    void stowage();

    // 装船时间造成的价值
    double transport_time_value();
};