#pragma once

#include "Path.hpp"

bool RobotFindNewGoal(queue<Cargo> cars, Robot &r) {
    double max_value = 0;
    int goal_id;
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
        if (cargo.val > 150) {
            value *= 1.5;
        }
        if (value > max_value) {
            max_value = value;
            goal = cargo;
            goal_id = berth_id;
        }
    }
    if (max_value <= 0)
        return false;
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
    //log("机器人" + to_string(r.id) + "获取空余目标" + to_string(goal.x) + " " + to_string(goal.y) + " " +
    //to_string(goal.val) + " " + to_string(goal_time) + " " + to_string(goal_id));
    r.setGoal(goal, goal_id);
    return true;
}