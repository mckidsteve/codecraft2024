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
struct cargo {
    int x, y;//货物的坐标
    int val;//货物的价值
    int time;//货物的产生时间
    cargo(int x, int y, int val, int time) : x(x), y(y), val(val), time(time) {}
};

//所有货物队列
queue<cargo> cargos;
//新产生的货物队列
queue<cargo> new_cargos;

//机器人
class Robot {
public:
    int goods{};//是否携带货物
    int x{}, y{};//机器人的x，y坐标
    int status{};//机器人是否处于运行状态
    int id{};//机器人的编号
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

//每帧的输入
int PerframeInput() {
    scanf("%d%d", &id, &money);
    int num;
    scanf("%d", &num);
    for (int i = 1; i <= num; i++) {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        new_cargos.emplace(x, y, val, id);
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

void PerframeUpdate() {
    for (int i = 0; i < robot_num; i++)
        printf("move %d %d\n", i, rand() % 4);
    puts("OK");
    fflush(stdout);
}

//主函数
int main() {
    Init();
    for (int zhen = 1; zhen <= 15000; zhen++) {
        PerframeInput();
        PerframeUpdate();
    }
    return 0;
}
