#pragma once

// 货物
struct Cargo {
    int x, y; // 货物的坐标
    int val;  // 货物的价值
    int time; // 货物的产生时间
    Cargo(int x, int y, int val, int time) : x(x), y(y), val(val), time(time) {}
};