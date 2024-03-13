#pragma once

#include "Path.hpp"

void RobotFindNewGoal(queue<Cargo> cars, Robot &r) {
    double max_value = 0;
    int goal_id;
    double goal_time;
    Cargo goal = default_cargo;
    for (int i = 0; i < cargos.size(); i++) {
        Cargo cargo = cars.front();
        cars.pop();
        int berth_id = CargotoBerth(cargo);
        if (berth_id == -1 || berth_dis[r.x][r.y][berth_id] == -1)
            continue;
        double berth_time = berth_dis[cargo.x][cargo.y][berth_id] + berths[berth_id].transport_time_value();
        int robot_time = CargotoRobot(cargo, r);
        if (robot_time == MAXNUM)
            continue;
        double value = cargo.val * 1.0 / (berth_time + robot_time);
        if (value > max_value) {
            max_value = value;
            goal = cargo;
            goal_id = berth_id;
            goal_time = berth_time;
        }
    }
    if (max_value <= 0)
        return;
//    log("重新获取货物:" + to_string(r.id));
//    log("新物品的val:" + to_string(goal.val));
    Robotlib::Path path = getRoadtoCargo(r.x, r.y, goal.x, goal.y);
    if (path.road.empty())
        return;
    // 删除被拿走的物品
    queue<Cargo> c;
    while (!cargos.empty()) {
        Cargo ca = cargos.front();
        cargos.pop();
        if (ca.val == goal.val && ca.x == goal.x && ca.y == goal.y && ca.time == goal.time)
            continue;
        c.push(ca);
    }
    cargos = c;
    r.setGoal(goal, goal_time, goal_id, path);
}

void RobotFindNewGoalatLast(queue<Cargo> cars, Robot& r, int * berth_set, int size=3) {
    double max_value = 0;
    int goal_id;
    double goal_time;
    Cargo goal = default_cargo;
    for (int i = 0; i < cargos.size(); i++) {
        Cargo cargo = cars.front();
        cars.pop();
        int berth_id = CargotoBerth(cargo);
        if (berth_id == -1 || berth_dis[r.x][r.y][berth_id] == -1)
            continue;
        //找到的最优目的地不是最后指定的三个港口
        if (berth_id != berth_set[0] || berth_id != berth_set[1] || berth_id != berth_set[2])
            continue;

        double berth_time = berth_dis[cargo.x][cargo.y][berth_id] + berths[berth_id].transport_time_value();
        int robot_time = CargotoRobot(cargo, r);
        if (robot_time == MAXNUM)
            continue;
        double value = cargo.val * 1.0 / (berth_time + robot_time);
        if (value > max_value) {
            max_value = value;
            goal = cargo;
            goal_id = berth_id;
            goal_time = berth_time;
        }
    }

    Robotlib::Path path = getRoadtoCargo(r.x, r.y, goal.x, goal.y);
    if (path.road.empty()) {        //在这里说明要么场上没货了，要么离我指定3个港口近的没了（此时我的策略是随便拿一个）
        if (cargos.empty())  return;
        else {
            goal = cargos.front();
            goal_id = berth_set[rand() % 3];
            goal_time = 0.00001;      //这里我随便给，反正没有最近的了，将就运这个货吧
            path = getRoadtoCargo(r.x, r.y, goal.x, goal.y);
        }
    }
       
    // 删除被拿走的物品
    queue<Cargo> c;
    while (!cargos.empty()) {
        Cargo ca = cargos.front();
        cargos.pop();
        if (ca.val == goal.val && ca.x == goal.x && ca.y == goal.y && ca.time == goal.time)
            continue;
        c.push(ca);
    }
    cargos = c;
    r.setGoal(goal, goal_time, goal_id, path);
}