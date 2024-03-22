#pragma once

// 机器人到泊位的距离
int RobottoBerth(Robot &r) {
    double min_time = MAXNUM;
    int min_id = -1;
    for (int i = 0; i < berth_num; i++) {
        if (berth_dis[r.x][r.y][i] == -1)
            continue;
        double time = berth_dis[r.x][r.y][i] + berths[i].transport_time_value();
        if (time < min_time) {
            min_time = time;
            min_id = i;
        }
    }
    return min_id;
}

// 机器人到泊位的距离
int RobottoBerthOfValue(Robot &r) {
    double max_val = -1;
    int max_id = -1;
    for (int i = 0; i < berth_num; i++) {
        if (berth_dis[r.x][r.y][i] == -1) continue;
        double val = (r.cargo.val + berths[i].carge_value) /
                     max(1.0, berth_dis[r.x][r.y][i] + berths[i].transport_time_value() + berths[i].cargo_dis);
        if (val > max_val) {
            max_val = val;
            max_id = i;
        }
    }
    return max_id;
}

// 货物到泊位的距离
int CargotoBerth(Cargo &c) {
    double min_time = MAXNUM;
    int min_id = -1;
    for (int i = 0; i < berth_num; i++) {
        if (berth_dis[c.x][c.y][i] == -1)
            continue;
        //if (berths[i].things.size() >= boat_capacity)continue;
        double time = berth_dis[c.x][c.y][i] + berths[i].transport_time_value();
        if (time < min_time) {
            min_time = time;
            min_id = i;
        }
    }
    return min_id;
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