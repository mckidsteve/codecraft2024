#pragma once

#include "Path.hpp"
#include "State.hpp"

// 机器人到泊位的距离
int RobottoBerth(Robot &r) {
    int min_time = MAXNUM;
    int min_id = -1;
    for (int i = 0; i < berth_num; i++) {
        if (berth_dis[r.x][r.y][i] == -1)
            continue;
        int time = berth_dis[r.x][r.y][i] + berths[i].transport_time_value();
        if (time < min_time) {
            min_time = time;
            min_id = i;
        }
    }
    return min_id;
}

// 货物到泊位的距离
int CargotoBerth(Cargo &c) {
    int min_time = MAXNUM;
    int min_id = -1;
    for (int i = 0; i < berth_num; i++) {
        if (berth_dis[c.x][c.y][i] == -1)
            continue;
        int time = berth_dis[c.x][c.y][i] + berths[i].transport_time_value();
        if (time < min_time) {
            min_time = time;
            min_id = i;
        }
    }
    return min_id;
}


//获取到物品的路径
Robotlib::Path getRoadtoCargo(int x, int y, int x1, int y1) {
//    log("获取到物品的路径");
//    log("从x:" + to_string(x) + " y:" + to_string(y) + "到x1:" + to_string(x1) + " y1:" + to_string(y1));
    Robotlib::Path path;
    unordered_set<Robotlib::State> obstacles;
    starEpsilon->SearchToCargo(path, id, make_pair(x, y), make_pair(x1, y1), obstacles);
//    log("路径长度:" + to_string(path.size()));
//    log("获取到物品的路径成功");
    return path;
}

//获取到泊位的路径
Robotlib::Path getRoadtoBerth(int x, int y, int x1, int y1, int berthid) {
//    log("获取到泊位的路径");
//    log("从x:" + to_string(x) + " y:" + to_string(y) + "到x1:" + to_string(x1) + " y1:" + to_string(y1));
    Robotlib::Path path;
    unordered_set<Robotlib::State> obstacles;
    starEpsilon->SearchToBerth(path, id, make_pair(x, y), make_pair(x1, y1), obstacles, berthid);
//    log("路径长度:" + to_string(path.size()));
//    log("获取到泊位的路径成功");
    return path;
}

// 货物到机器人的距离
int CargotoRobot(Cargo &c, Robot &r) {
    int min = MAXNUM;
    int size = random_point.size();
    for (int i = 0; i < size; i++) {
        if (random_dis[c.x][c.y][i] == -1)
            continue;
        if (random_dis[r.x][r.y][i] == -1)
            continue;
        int z = random_dis[c.x][c.y][i] + random_dis[r.x][r.y][i];
        if (z < min) {
            min = z;
        }
    }
    return min;
}
