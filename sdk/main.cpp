#include <bits/stdc++.h>
#include "log.hpp"

using namespace std;

#define MAXNUM 0x3f3f3f3f
const int n = 200; // map size
const int robot_num = 10;
const int boat_num = 5;
const int berth_num = 10;
const int random_bfs_point = 100;
int money;         // 钱数 （分数）
int boat_capacity; // 船装货上限
/**
 * 地图
 * .空地
 * *海洋
 * #障碍
 * 0-9泊位所在的位置
 * */
vector<vector<char>> game_map(n, vector<char>(n, '#'));
// 到对应泊位的距离
vector<vector<vector<int>>> berth_dis(n, vector<vector<int>>(n, vector<int>(berth_num, -1)));
// 到随机点的距离
vector<vector<vector<int>>> random_dis(n, vector<vector<int>>(n, vector<int>(random_bfs_point + 50, -1)));
// 所有随机点的坐标
vector<pair<int, int>> random_point;


int id;

class Robot;

class Cargo;

vector<pair<int, int>> getRoadtoBerth(int x, int y, int x1, int y1, int berthid);

void RobotFindNewGoal(queue<Cargo> cars, Robot &r);

#include "CARGO.hpp"

// 货物
struct Cargo default_cargo(0, 0, 0, 0);

// 所有货物队列
queue<Cargo> cargos;
// 新产生的货物队列
deque<Cargo> new_cargos;

#include "ROBOT.hpp"

class Robot robots[robot_num];

#include "BOAT.hpp"

// 船
class Boat boats[boat_num];

#include "BERTH.hpp"

// 泊位
class Berth berths[berth_num];

#include "Robot_imp.hpp"
#include "Boat_imp.hpp"
#include "Berth_imp.hpp"
#include "AStarEpsilon.hpp"
#include "CoverToVector.hpp"

using Robotlib::AStarEpsilon;
using Robotlib::State;
AStarEpsilon *starEpsilon;

// 初始化
#include "Init.hpp"

// 距离计算
#include "Dist.hpp"

// A*算法
#include "Astar.hpp"

// 获取一个新的物品
#include "FindNewGoal.hpp"

// Frame
#include "Frame.hpp"

// 主函数
int main() {
    LogInit();
    Init();
    for (int zhen = 1; zhen <= 15000; zhen++) {
        PerframeInput();
        PerframeUpdate();
        PerframeOutput();
    }
    LogClose();
    return 0;
}
