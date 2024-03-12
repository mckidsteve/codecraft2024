#pragma once

// 船
class Boat {
public:
    int num{};     // 装载的数量
    int berthid{}; // 停泊的泊位
    int status{};  // 船的状态
    int id{};      // 船的id
    Boat() {}

    void go();

    void ship(int id);
};
