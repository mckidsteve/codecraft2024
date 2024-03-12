#pragma once

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
        int robot_time = CargotoRobot(cargo, robots[i]);
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
    log("重新获取货物:" + to_string(r.id));
    log("新物品的val:" + to_string(goal.val));
    vector<pair<int, int>> road = getRoadtoCargo(r.x, r.y, goal.x, goal.y);
    if (road.empty())
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
    r.setGoal(goal, goal_time, goal_id, road);
}