#include <bits/stdc++.h>

using namespace std;

const int n = 200;
const int robot_num = 10;
const int boat_num = 5;
const int berth_num = 10;
int money;//钱数 （分数）
int boat_capacity;//船装货上限
#ifdef _WIN32
ofstream outfile;//日志文件
#endif
int id;
/**
 * 地图
 * .空地
 * *海洋
 * #障碍
 * 0-9泊位所在的位置
 * */
char game_map[n][n];

void log(string s);

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
    int berthid{-1};
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

    //重置机器人
    void Reset();
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

void Robot::Reset() {
    queue<pair<int, int>> road;
    this->road = road;
    cargotoberth = 0;
    berthid = -1;
    cargos.push(cargo);
    cargo = default_cargo;
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
    int dir[4][2] = {{0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0}};
    int dis[n][n];
    memset(dis, -1, sizeof(dis));
    //优先队列
    priority_queue<pair<int, pair<int, pair<int, int>>>> q;
    q.emplace(0, make_pair(0, make_pair(x, y)));
    dis[x][y] = 0;
    int flag = 0;
    //A*算法
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
            //标记已经访问
            dis[nextx][nexty] = nowg + 1;
            //估价函数
            int nextg = nowg + 1;
            //曼哈顿距离
            int h = abs(nextx - x1) + abs(nexty - y1);
            //加入优先队列
            q.emplace(-(nextg + h), make_pair(nextg, make_pair(nextx, nexty)));
        }
    }
    if (flag == 0)return road;
    int nowx = x1, nowy = y1;
    //获取路径
    while (nowx != x || nowy != y) {
        road.push(make_pair(nowx, nowy));
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
    //翻转路径
    queue<pair<int, int>> road1;
    while (!road.empty()) {
        road1.push(road.front());
        road.pop();
    }
    road = road1;
    return road;
}

queue<pair<int, int>> Astar(int x, int y, int x1, int y1, int berthid) {
    queue<pair<int, int>> road;
    int dir[4][2] = {{0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0}};
    int dis[n][n];
    memset(dis, -1, sizeof(dis));
    //优先队列
    priority_queue<pair<int, pair<int, pair<int, int>>>> q;
    q.emplace(0, make_pair(0, make_pair(x, y)));
    dis[x][y] = 0;
    int flag = 0;
    int fx, fy;//终点坐标
    //A*算法
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
            //标记已经访问
            dis[nextx][nexty] = nowg + 1;
            //估价函数
            int nextg = nowg + 1;
            //曼哈顿距离
            int h = abs(nextx - x1) + abs(nexty - y1);
            //加入优先队列
            q.emplace(-(nextg + h), make_pair(nextg, make_pair(nextx, nexty)));
        }
    }
    if (flag == 0)return road;
    int nowx = fx, nowy = fy;
    //获取路径
    while (nowx != x || nowy != y) {
        road.push(make_pair(nowx, nowy));
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
    //翻转路径
    queue<pair<int, int>> road1;
    while (!road.empty()) {
        road1.push(road.front());
        road.pop();
    }
    road = road1;
    return road;
}

//获取到物品的路径
queue<pair<int, int>> getRoadtoCargo(int x, int y, int x1, int y1) {
    return Astar(x, y, x1, y1);
}

//获取到泊位的路径
queue<pair<int, int>> getRoadtoBerth(int x, int y, int x1, int y1, int berthid) {
    return Astar(x, y, x1, y1, berthid);
}

//货物到机器人的距离,曼哈顿距离
int CargotoRobot(Cargo &c, Robot &r) {
    return abs(c.x - r.x) + abs(c.y - r.y);
}

//每帧的输入
int PerframeInput() {
    scanf("%d%d", &id, &money);
    log("第" + to_string(id) + "帧输入");
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
    log("第" + to_string(id) + "帧更新");
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
        //如果没有找到机器人就把货物重新放入队列
        if (robot_id == -1) {
            cargos.push(cargo);
            continue;
        }
        queue<pair<int, int>> road = getRoadtoCargo(robots[robot_id].x, robots[robot_id].y, cargo.x, cargo.y);
        //如果没有找到路径就把货物重新放入队列
        if (road.empty()) {
            cargos.push(cargo);
            continue;
        }
        //如果机器人在运输其他货物，就先把货物重新放入队列
        if (robots[robot_id].cargo.val != 0) {
            new_cargos.push_front(robots[robot_id].cargo);
        }
        robots[robot_id].cargo = cargo;
        robots[robot_id].cargotoberth = berth_time;
        robots[robot_id].berthid = berth_id;
        robots[robot_id].road = road;
        break;
    }
}

//每帧的输出
void PerframeOutput() {
    log("第" + to_string(id) + "帧输出");
    for (int i = 0; i < robot_num; i++) {
        //如果机器人路径不为空就继续走
        if (!robots[i].road.empty()) {
            pair<int, int> next = robots[i].road.front();
            robots[i].road.pop();
            int dis = abs(robots[i].x - next.first) + abs(robots[i].y - next.second);
            if (dis > 1) {
                robots[i].Reset();
            } else {
                //机器人移动
                if (robots[i].x - next.first == 1)printf("move %d %d\n", i, 2);
                else if (robots[i].x - next.first == -1)printf("move %d %d\n", i, 3);
                else if (robots[i].y - next.second == 1)printf("move %d %d\n", i, 1);
                else if (robots[i].y - next.second == -1)printf("move %d %d\n", i, 0);
            }
            if (robots[i].road.empty()) {
                if (robots[i].goods == 0) {
                    //机器人没有货物,就去拿货物
                    printf("get %d\n", i);
                    robots[i].road = getRoadtoBerth(next.first, next.second, berths[robots[i].berthid].x,
                                                    berths[robots[i].berthid].y, robots[i].berthid);
                } else {
                    //机器人有货物,就去放货物
                    printf("pull %d\n", i);
                    robots[i].Reset();
                }
            }
        }
    }
    puts("OK");
    fflush(stdout);
}

//日志信息
void log(string s) {
#ifdef _WIN32
    outfile << s << endl;
#endif
}

//主函数
int main() {
    Init();
#ifdef _WIN32
    // 以写模式打开文件
    outfile.open("log.txt", ios::out | ios::trunc);
#endif
    for (int zhen = 1; zhen <= 15000; zhen++) {
        PerframeInput();
        PerframeUpdate();
        PerframeOutput();
    }
#ifdef _WIN32
    outfile.close();
#endif
    return 0;
}
