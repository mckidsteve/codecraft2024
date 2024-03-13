#pragma once

#include "ROBOT.hpp"

void Robot::getThings(int x, int y) {
    printf("get %d\n", id);
    ecbs->Search(id, berths[berthid].x, berths[berthid].y, zhen, berthid);
}

void Robot::putThings(int x, int y) {
    printf("pull %d\n", id);
    int num = game_map[x][y] - '0';
    if (num < 0 || num >= berth_num)
        return;
    int pos_x = x - berths[num].x;
    int pos_y = y - berths[num].y;
    pair<int, int> pair1(pos_x, pos_y);
    berths[num].things.push(pair1);
    this->Reset(true);
}

void Robot::move(int x, int y) {
    if (this->x - x == 1)
        printf("move %d %d\n", id, 2);
    else if (this->x - x == -1)
        printf("move %d %d\n", id, 3);
    else if (this->y - y == 1)
        printf("move %d %d\n", id, 1);
    else if (this->y - y == -1)
        printf("move %d %d\n", id, 0);
}

void Robot::Reset(bool complete) {
    path.clear();
    cargotoberth = 0;
    berthid = -1;
    if (!complete && goods == 0)
        cargos.push(cargo);
    cargo = default_cargo;
}

void Robot::setGoal(Cargo c, double dis, int brenth_id) {
    //    log("机器人id:" + to_string(zhen));
    //    log("新物品的x坐标:" + to_string(c.x));
    //    log("新物品的y坐标:" + to_string(c.y));
    //    log("新物品的平均价值:" + to_string(c.val * 1.0 / (r.size() + dis)));
    cargo = c;
    cargotoberth = dis;
    berthid = brenth_id;
    ecbs->Search(id, cargo.x, cargo.y, zhen, -1);
}