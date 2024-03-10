#include <bits/stdc++.h>

using namespace std;

const int n = 200;
const int robot_num = 10;
const int boat_num = 5;
const int berth_num = 10;
int money;//钱数 （分数）
int boat_capacity;//船装货上限
int id;
/**
 * 地图
 * .空地
 * *海洋
 * #障碍
 * 0-9泊位所在的位置
 * */
char game_map[n][n];

//货物
struct Cargo {
    int x, y;//货物的坐标
    int val;//货物的价值
    int time;//货物的产生时间
    Cargo(int x, int y, int val, int time) : x(x), y(y), val(val), time(time) {}
} default_cargo(0, 0, 0, 0);

//所有货物队列
queue<Cargo> cargos;
//新产生的货物队列
deque<Cargo> new_cargos;

//机器人
class Robot {
public:
    int goods{};//是否携带货物
    int x{}, y{};//机器人的x，y坐标
    int status{};//机器人是否处于运行状态
    int id{};//机器人的编号
    int cargotoberth{};//机器人携带的货物到泊位的距离
    Cargo cargo{default_cargo};//机器人的货物
    queue<pair<int, int>> road;//机器人的路径

    Robot() = default;

    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }

    //机器人获取物品
    void getThings();

    //机器人放下物品
    void putThings();
} robots[robot_num];

//船
struct Boat {
    int num;//装载的数量
    int pos;//停泊的泊位
    int status;//船的状态
} boats[boat_num];

//泊位
class Berth {
public:
    int x{};//泊位的x
    int y{};//泊位的y
    int id{};//泊位的编号
    int transport_time{};//到达虚拟点的时间
    int loading_speed{};//装货速度
    queue<pair<int, int>> things;//泊位上的物品,储存的是相对坐标
    Berth() = default;

    Berth(int x, int y, int transport_time, int loading_speed)
            : x(x), y(y), transport_time(transport_time), loading_speed(loading_speed) {}

    //泊位向船装货
    void stowage(Boat &boat);

} berths[berth_num];


void Robot::getThings() {
}

void Robot::putThings() {
    int num = game_map[x][y] - '0';
    if (num < 0 || num >= berth_num)return;
    game_map[x][y] = '.';
    int pos_x = x - berths[num].x;
    int pos_y = y - berths[num].y;
    pair<int, int> pair1(pos_x, pos_y);
    berths[num].things.push(pair1);
}

void Berth::stowage(Boat &boat) {
    for (int i = 0; i < loading_speed; i++) {
        if (boat.num < boat_capacity) {
            pair<int, int> a = things.front();
            things.pop();
            game_map[a.first][a.second] = id + '0';
            boat.num++;
        } else return;
    }
}

//初始化
void Init() {
    for (int i = 0; i < n; i++)
        scanf("%s", game_map[i]);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (game_map[i][j] == 'A')game_map[i][j] = '.';
        }
    }
    for (int i = 0; i < berth_num; i++) {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berths[id].x, &berths[id].y, &berths[id].transport_time, &berths[id].loading_speed);
        berths[id].id = id;
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                game_map[berths[id].x + j][berths[id].y + k] = '0' + i;
            }
        }
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}

//货物到泊位的距离
int CargotoBerth(Cargo &c) {
    int min_time = 0x3f3f3f3f;
    int min_id = -1;
    for (int i = 0; i < berth_num; i++) {
        int time = abs(c.x - berths[i].x) + abs(c.y - berths[i].y) + berths[i].transport_time;
        if (time < min_time) {
            min_time = time;
            min_id = i;
        }
    }
    return min_id;
}

//A*算法
queue<pair<int, int>> Astar(int x, int y, int x1, int y1) {
    queue<pair<int, int>> road;
    return road;
}

//获取路径
queue<pair<int, int>> getroad(int x, int y, int x1, int y1) {
    return Astar(x, y, x1, y1);
}

//货物到机器人的距离
int CargotoRobot(Cargo &c, Robot &r) {
    return abs(c.x - r.x) + abs(c.y - r.y);
}

//每帧的输入
int PerframeInput() {
    scanf("%d%d", &id, &money);
    int num;
    scanf("%d", &num);
    for (int i = 1; i <= num; i++) {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        new_cargos.emplace_back(x, y, val, id);
    }
    for (int i = 0; i < robot_num; i++) {
        robots[i].id = i;
        scanf("%d%d%d%d", &robots[i].goods, &robots[i].x, &robots[i].y, &robots[i].status);
    }
    for (int i = 0; i < 5; i++)
        scanf("%d%d\n", &boats[i].status, &boats[i].pos);
    char okk[100];
    scanf("%s", okk);
    return id;
}

//每帧的更新
void PerframeUpdate() {
    //每帧处理一个新货物
    bool have_robot = false;
    for (int i = 0; i < robot_num; i++) {
        if (robots[i].goods == 0) {
            have_robot = true;
            break;
        }
    }
    while (have_robot && !new_cargos.empty()) {
        Cargo cargo = new_cargos.front();
        new_cargos.pop_front();
        int berth_id = CargotoBerth(cargo);
        if (berth_id == -1)continue;
        int berth_time =
                abs(berths[berth_id].x - cargo.x) + abs(berths[berth_id].y - cargo.y) + berths[berth_id].transport_time;
        double max_value = 0.0;
        int robot_id = -1;
        //找到最优的机器人
        for (int i = 0; i < robot_num; i++) {
            //如果机器人携带货物就跳过
            if (robots[i].goods == 1)continue;
            int robot_time = CargotoRobot(cargo, robots[i]);
            double value = cargo.val * 1.0 / (berth_time + robot_time);
            double robot_value =
                    robots[i].cargo.val * 1.0 / max(1, (int) robots[i].road.size() + robots[i].cargotoberth);
            if (robot_value > value)continue;
            if (value > max_value) {
                max_value = value;
                robot_id = i;
            }
        }
        if (robot_id == -1)continue;
        //如果机器人在运输其他货物，就先把货物重新放入队列
        if (robots[robot_id].cargo.val != 0) {
            new_cargos.push_front(robots[robot_id].cargo);
        }
        robots[robot_id].cargo = cargo;
        robots[robot_id].cargotoberth = berth_time;
        robots[robot_id].road = getroad(robots[robot_id].x, robots[robot_id].y, cargo.x, cargo.y);
        break;
    }
}

//每帧的输出
void PerframeOutput() {
    for (int i = 0; i < robot_num; i++)
        puts("OK");
    fflush(stdout);
}

//主函数
int main() {
    Init();
    for (int zhen = 1; zhen <= 15000; zhen++) {
        PerframeInput();
        PerframeUpdate();
        PerframeOutput();
    }
    return 0;
}
