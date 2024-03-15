#pragma once

// 每帧的输入
void PerframeInput() {
    scanf("%d%d", &zhen, &money);
    log("第" + to_string(zhen) + "帧");
    int num;
    scanf("%d", &num);
    for (int i = 1; i <= num; i++) {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        new_cargos.emplace_back(x, y, val, zhen);
    }
    for (int i = 0; i < robot_num; i++) {
        scanf("%d%d%d%d", &robots[i].goods, &robots[i].x, &robots[i].y, &robots[i].status);
    }
    for (int i = 0; i < 5; i++)
        scanf("%d%d\n", &boats[i].status, &boats[i].berthid);
    char okk[100];
    scanf("%s", okk);
}

// 每帧的更新
void PerframeUpdate() {
//    bool f = true;
//    int count = 0;
//    while (zhen == 5 && f) {
//        count = 1;
//    }
    queue<Cargo> c;
//    for (int i = 0; i < berth_num; i++) {
//        log("泊位还有物品个数" + to_string(berths[i].things.size()));
//    }
    // 当大于1000帧的时候物品消失
    int zsize = cargos.size();
    for (int i = 0; i < zsize; i++) {
        Cargo cargo = cargos.front();
        cargos.pop();
        if (zhen - cargo.time > 1000)
            continue;
        c.push(cargo);
    }
    cargos = c;
    // 每帧处理一个新货物
    bool have_robot = false;
    for (int i = 0; i < robot_num; i++) {
        if (robots[i].status == 0)continue;
        if (robots[i].goods == 0) {
            have_robot = true;
            break;
        }
    }
    while (have_robot && !new_cargos.empty()) {
        Cargo cargo = new_cargos.front();
        new_cargos.pop_front();
        int berth_id = CargotoBerth(cargo);
        if (berth_id == -1)
            continue;
        double berth_time = berth_dis[cargo.x][cargo.y][berth_id] + berths[berth_id].transport_time_value();
        double max_value = 0.0;
        int robot_id = -1;
        //找到最优的机器人
        for (int i = 0; i < robot_num; i++) {
            if (robots[i].status == 0)continue;
            //如果机器人携带货物就跳过
            if (robots[i].goods == 1)continue;
            //如果机器人到不了对应的泊位
            if (berth_dis[robots[i].x][robots[i].y][berth_id] == -1)continue;
            int robot_time = CargotoRobot(cargo, robots[i]);
            if (robot_time == MAXNUM)
                continue;
            double value = cargo.val * 1.0 / (berth_time + robot_time);
            double robot_value =
                    robots[i].cargo.val * 1.0 /
                    max(1.0, CargotoRobot(robots[i].cargo, robots[i]) + robots[i].cargotoberth);
            if (robot_value > value)
                continue;
            if (value - robot_value > max_value) {
                max_value = value - robot_value;
                robot_id = i;
            }
        }
        // 如果没有找到机器人就把货物重新放入队列
        if (robot_id == -1) {
            cargos.push(cargo);
            continue;
        }
        // 如果机器人在运输其他货物，就先把货物重新放入队列
        if (robots[robot_id].cargo.val != 0) {
            //log("机器人id:" + to_string(robot_id) + "将旧目标放回");
            new_cargos.push_front(robots[robot_id].cargo);
        }
        //log("机器人id:" + to_string(robot_id) + "获取新的目标");
        robots[robot_id].setGoal(cargo, berth_time, berth_id);
        break;
    }
    // 如果机器人携带的货物时间超过1000帧就重新获取新的任务目标
    for (int i = 0; i < robot_num; i++) {
        if (robots[i].goods == 0 && robots[i].cargo.val != 0 &&
            (robots[i].path.road.size() + zhen - robots[i].cargo.time > 1000)) {
            //log("机器人id:" + to_string(i) + "时间超时");
            robots[i].Reset(false);
        }
    }
    //有目标但是失败的机器人重新获取新的泊位
    for (int i = 0; i < robot_num; i++) {
        if (robots[i].status == 0)continue;
        if (robots[i].goods == 1 &&
            robots[i].path.road.empty()) {
            int berth_id = RobottoBerth(robots[i]);
            ecbs->Search(i, berths[berth_id].x, berths[berth_id].y, zhen, berth_id);
            break;
        }
    }
    //没有任务目标的机器人获得新的任务目标
    for (int i = 0; i < robot_num; i++) {
        if (robots[i].cargo.val == 0 && robots[i].goods == 0 && !cargos.empty()) {
            if (RobotFindNewGoal(cargos, robots[i]))
                break;
        }
    }
    //没有任务目标的机器人原地搜索避免碰撞
    for (int i = 0; i < robot_num; i++) {
        if (robots[i].path.road.empty()) {
            ecbs->Search(i, robots[i].x, robots[i].y, zhen, -1);
        }
    }
//    for (int i = 0; i < robot_num; i++) {
//        log("机器人" + to_string(i) + "路径长度" + to_string(robots[i].path.road.size()));
//    }
}

// 每帧的输出
void PerframeOutput() {
    //    log("第" + to_string(zhen) + "帧输出");
    //    for (int i = 0; i < berth_num; i++) {
    //        if (berths[i].things.empty())continue;
    //        log("第" + to_string(i) + "号泊位");
    //        log("物品数量:" + to_string(berths[i].things.size()));
    //    }
    for (int i = 0; i < robot_num; i++) {
        // 如果机器人路径不为空就继续走
        if (!robots[i].path.road.empty()) {
            pair<int, int> next = robots[i].path.road.front();
            robots[i].path.road.erase(robots[i].path.road.begin());
            int dis = abs(robots[i].x - next.first) + abs(robots[i].y - next.second);
            if (dis > 1) {
                //log("机器人id:" + to_string(i) + "移动错误");
                robots[i].Reset(false);
            } else {
                // 机器人移动
                robots[i].move(next.first, next.second);
                if (robots[i].path.road.empty()) {
                    if (robots[i].goods == 0) {
                        // 机器人没有货物,就去拿货物
                        //log("机器人id:" + to_string(i) + "正常取货");
                        robots[i].getThings(next.first, next.second);
                    } else {
                        // 机器人有货物,就去放货物
                        //log("机器人id:" + to_string(i) + "正常放货");
                        robots[i].putThings(next.first, next.second);
                    }
                }
            }
        } else if (robots[i].goods == 1) { // 机器人正好在港口上的时候
            robots[i].putThings(robots[i].x, robots[i].y);
        }
    }
    for (int i = 0; i < boat_num; i++) {
        if (boats[i].status == 0)
            continue;
        if (boats[i].num == boat_capacity || // 当装满的时候直接出发开始运输
            boats[i].berthid != -1 && berths[boats[i].berthid].transport_time + zhen > 14950 &&
            boats[i].num != 0) { // 当时间快到的时候也直接开始运输
            boats[i].go();
            continue;
        } else if (boats[i].berthid == -1) {
            int max = 0, goal = -1;
            for (int j = 0; j < berth_num; j++) {
                if (berths[j].boatids.size() >= berths[j].things.size() * 1.0 / boat_capacity)
                    continue;
                int z = berths[j].things.size();
                if (z > max) {
                    max = z;
                    goal = j;
                }
            }
            if (goal == -1) {
                for (int j = 0; j < berth_num; j++) {
                    if (berths[j].boatids.empty()) {
                        goal = j;
                        break;
                    }
                }
            }
            boats[i].ship(goal);
            continue;
        } else if (!berths[boats[i].berthid].things.empty()) {
            continue;
        } else {
            int max = 0, goal = -1;
            for (int j = 0; j < berth_num; j++) {
                if (berths[j].boatids.size() >= berths[j].things.size() * 1.0 / boat_capacity)
                    continue;
                if (zhen + 500 + berths[j].transport_time > 14950)
                    continue;
                int z = berths[j].things.size();
                if (z > max) {
                    max = z;
                    goal = j;
                }
            }
            boats[i].ship(goal);
        }
    }
    for (int i = 0; i < berth_num; i++) {
        berths[i].stowage();
    }
    if (zhen == 15000) {
        for (int i = 0; i < berth_num; i++) {
            log("泊位" + to_string(i) + "的货物数量" + to_string(berths[i].things.size()));
        }
        for (int i = 0; i < boat_num; i++) {
            log("船只" + to_string(i) + "的货物" + to_string(boats[i].num));
        }
    }
    puts("OK");
    fflush(stdout);
}
