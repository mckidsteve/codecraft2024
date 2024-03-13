#pragma once

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