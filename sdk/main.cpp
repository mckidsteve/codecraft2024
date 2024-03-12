// #pragma once
#include <bits/stdc++.h>
#include ".\include\log.hpp"
#include ".\include\main.h"
using namespace std;

class Robot;

class Cargo;

vector<pair<int, int>> getRoadtoBerth(int x, int y, int x1, int y1, int berthid);

void RobotFindNewGoal(queue<Cargo> cars, Robot &r);

#include "./include/CARGO.hpp"

// 货物
struct Cargo default_cargo(0, 0, 0, 0);

// 所有货物队列
queue<Cargo> cargos;
// 新产生的货物队列
deque<Cargo> new_cargos;

#include "./include/ROBOT.hpp"

class Robot robots[robot_num];

#include "./include/BOAT.hpp"

// 船
class Boat boats[boat_num];

#include "./include/BERTH.hpp"

// 泊位
class Berth berths[berth_num];

#include "./include/Robot_imp.hpp"
#include "./include/Boat_imp.hpp"
#include "./include/Berth_imp.hpp"
#include "./include/AStarEpsilon.hpp"
#include "./include/CoverToVector.hpp"
#include "./include/frame.h"

using Robotlib::AStarEpsilon;
using Robotlib::State;
AStarEpsilon *starEpsilon;



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

// A*算法
vector<pair<int, int>> Astar(int x, int y, int x1, int y1) {
    vector<pair<int, int>> road;
    int dir[4][2] = {{0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0}};
    int dis[n][n];
    memset(dis, -1, sizeof(dis));
    // 优先队列
    priority_queue<pair<int, pair<int, pair<int, int>>>> q;
    q.emplace(0, make_pair(0, make_pair(x, y)));
    dis[x][y] = 0;
    int flag = 0;
    // A*算法
    while (!q.empty()) {
        int nowx = q.top().second.second.first;
        int nowy = q.top().second.second.second;
        int nowg = q.top().second.first;
        q.pop();
        if (nowx == x1 && nowy == y1) {
            flag = 1;
            break;
        }
        for (int i = 0; i < 4; i++) {
            int nextx = nowx + dir[i][0];
            int nexty = nowy + dir[i][1];
            if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || dis[nextx][nexty] != -1 ||
                game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                continue;
            // 标记已经访问
            dis[nextx][nexty] = nowg + 1;
            // 估价函数
            int nextg = nowg + 1;
            // 曼哈顿距离
            int h = abs(nextx - x1) + abs(nexty - y1);
            // 加入优先队列
            q.emplace(-(nextg + h), make_pair(nextg, make_pair(nextx, nexty)));
        }
    }
    if (flag == 0)
        return road;
    int nowx = x1, nowy = y1;
    // 获取路径
    while (nowx != x || nowy != y) {
        road.emplace_back(nowx, nowy);
        for (int i = 0; i < 4; i++) {
            int nextx = nowx + dir[i][0];
            int nexty = nowy + dir[i][1];
            if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || dis[nextx][nexty] == -1 ||
                game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                continue;
            if (dis[nextx][nexty] == dis[nowx][nowy] - 1) {
                nowx = nextx;
                nowy = nexty;
                break;
            }
        }
    }
    // 翻转路径
    reverse(road.begin(), road.end());
    return road;
}

vector<pair<int, int>> Astar(int x, int y, int x1, int y1, int berthid) {
    vector<pair<int, int>> road;
    int dir[4][2] = {{0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0}};
    int dis[n][n];
    memset(dis, -1, sizeof(dis));
    // 优先队列
    priority_queue<pair<int, pair<int, pair<int, int>>>> q;
    q.emplace(0, make_pair(0, make_pair(x, y)));
    dis[x][y] = 0;
    int flag = 0;
    int fx, fy; // 终点坐标
    // A*算法
    while (!q.empty()) {
        int nowx = q.top().second.second.first;
        int nowy = q.top().second.second.second;
        int nowg = q.top().second.first;
        q.pop();
        if (nowx == x1 && nowy == y1 || game_map[nowx][nowy] == '0' + berthid) {
            flag = 1;
            fx = nowx;
            fy = nowy;
            break;
        }
        for (int i = 0; i < 4; i++) {
            int nextx = nowx + dir[i][0];
            int nexty = nowy + dir[i][1];
            if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || dis[nextx][nexty] != -1 ||
                game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                continue;
            // 标记已经访问
            dis[nextx][nexty] = nowg + 1;
            // 估价函数
            int nextg = nowg + 1;
            // 曼哈顿距离
            int h = berth_dis[nextx][nexty][berthid];
            // 加入优先队列
            q.emplace(-(nextg + h), make_pair(nextg, make_pair(nextx, nexty)));
        }
    }
    if (flag == 0)
        return road;
    int nowx = fx, nowy = fy;
    // 获取路径
    while (nowx != x || nowy != y) {
        road.emplace_back(nowx, nowy);
        for (int i = 0; i < 4; i++) {
            int nextx = nowx + dir[i][0];
            int nexty = nowy + dir[i][1];
            if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || dis[nextx][nexty] == -1 ||
                game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                continue;
            if (dis[nextx][nexty] == dis[nowx][nowy] - 1) {
                nowx = nextx;
                nowy = nexty;
                break;
            }
        }
    }
    // 翻转路径
    reverse(road.begin(), road.end());
    return road;
}

//获取到物品的路径
vector<pair<int, int>> getRoadtoCargo(int x, int y, int x1, int y1) {
    //return Astar(x, y, x1, y1);
    vector<pair<int, int>> path = Astar(x, y, x1, y1);
//    vector<pair<int, int>> path;
//    unordered_set<State> obstacles;
    log("计算机器人到物品的路径");
//    starEpsilon->Search(path, id, make_pair(x, y), make_pair(x1, y1), obstacles);
    log("path的长度:" + to_string(path.size()));
    return path;
}

//获取到泊位的路径
vector<pair<int, int>> getRoadtoBerth(int x, int y, int x1, int y1, int berthid) {
    //log("计算机器人到港口的路径");
    return Astar(x, y, x1, y1, berthid);
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

// 获取一个新的物品
void RobotFindNewGoal(queue<Cargo> cars, Robot &r) {
    double max_value = 0;
    int goal_id;
    double goal_time;
    Cargo goal = default_cargo;
    for (int i = 0; i < cargos.size(); i++) {
        Cargo cargo = cars.front();
        cars.pop();
        int berth_id = CargotoBerth(cargo);
        if (berth_id == -1)
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


// 主函数
int main() {
    Init();
    LogInit();
    for (int zhen = 1; zhen <= 15000; zhen++) {
        PerframeInput();
        PerframeUpdate();
        PerframeOutput();
    }
    LogClose();
    return 0;
}
