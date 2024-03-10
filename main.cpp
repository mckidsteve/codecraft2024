#include <bits/stdc++.h>

using namespace std;

const int n = 200;
const int robot_num = 10;
const int boat_num = 5;
const int berth_num = 10;
int money;//钱数 （分数）
int boat_capacity;//船装货上限
//到对应泊位的距离
int berth_dis[n][n][berth_num];
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

class Cargo;

class Robot;

void log(string s);

queue<pair<int, int>> getRoadtoBerth(int x, int y, int x1, int y1, int berthid);

void RobotFindNewGoal(queue<Cargo> cars, Robot &r);

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
    void getThings(int x, int y);

    //机器人放下物品
    void putThings(int x, int y);

    //设置机器人要拿取什么物品
    void setGoal(Cargo c, int dis, int brenth_id, queue<pair<int, int>> r);

    //机器人移动
    void move(int x, int y);

    //重置机器人
    void Reset(bool complete);
} robots[robot_num];

//船
class Boat {
public:
    int num{};//装载的数量
    int berthid{};//停泊的泊位
    int status{};//船的状态
    int id{};//船的id
    Boat() {}

    void go();

    void ship(int id);
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
    int boatid{-1};//停靠或者准备停靠的船只编号

    Berth() = default;

    Berth(int x, int y, int transport_time, int loading_speed)
            : x(x), y(y), transport_time(transport_time), loading_speed(loading_speed) {}

    //泊位向船装货
    void stowage();

} berths[berth_num];


void Robot::getThings(int x, int y) {
    printf("get %d\n", id);
    road = getRoadtoBerth(x, y, berths[berthid].x,
                          berths[berthid].y, berthid);
}

void Robot::putThings(int x, int y) {
    printf("pull %d\n", id);
    int num = game_map[x][y] - '0';
    if (num < 0 || num >= berth_num)return;
    //log("第" + to_string(id) + "号机器人向第" + to_string(num) + "号泊位放置货物");
    int pos_x = x - berths[num].x;
    int pos_y = y - berths[num].y;
    pair<int, int> pair1(pos_x, pos_y);
    berths[num].things.push(pair1);
    this->Reset(true);
}

void Robot::move(int x, int y) {
    if (this->x - x == 1)printf("move %d %d\n", id, 2);
    else if (this->x - x == -1)printf("move %d %d\n", id, 3);
    else if (this->y - y == 1)printf("move %d %d\n", id, 1);
    else if (this->y - y == -1)printf("move %d %d\n", id, 0);
}

void Robot::Reset(bool complete) {
    while (!road.empty())road.pop();
    cargotoberth = 0;
    berthid = -1;
    if (!complete && goods == 0)cargos.push(cargo);
    cargo = default_cargo;
}

void Robot::setGoal(Cargo c, int dis, int brenth_id, queue<pair<int, int>> r) {
//    log("机器人id:" + to_string(id));
//    log("新物品的x坐标:" + to_string(c.x));
//    log("新物品的y坐标:" + to_string(c.y));
//    log("新物品的平均价值:" + to_string(c.val * 1.0 / (r.size() + dis)));
    cargo = c;
    cargotoberth = dis;
    berthid = brenth_id;
    road = r;
}

void Boat::go() {
    log("第" + to_string(id) + "号船开向虚拟点");
    printf("go %d\n", id);
    if (berthid != -1) {
        berths[berthid].boatid = -1;//对应的船归零
    }
    num = 0;
    berthid = -1;
    status = 0;
}

void Boat::ship(int goal) {
    if (goal == -1)return;
    log("第" + to_string(id) + "号船开向第" + to_string(goal) + "号泊位");
    printf("ship %d %d\n", id, goal);
    if (berthid != -1) {
        berths[berthid].boatid = -1;
    }
    berths[goal].boatid = id;
    berthid = goal;
    status = 0;
}

void Berth::stowage() {
    if (boatid == -1)return;
    if (boats[boatid].status == 0)return;
    if (things.empty())return;
    //log("第" + to_string(id) + "号泊位向第" + to_string(boatid) + "号船装载货物");
    for (int i = 0; i < loading_speed; i++) {
        if (boats[boatid].num < boat_capacity && !things.empty()) {
            //pair<int, int> a = things.front();
            things.pop();
            boats[boatid].num++;
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
    for (int i = 0; i < robot_num; i++) {
        robots[i].id = i;
    }
    for (int i = 0; i < boat_num; i++) {
        boats[i].id = i;
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
    //初始化所有泊位到所有位置的距离
    memset(berth_dis, -1, sizeof(berth_dis));
    int dir[4][2] = {{0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0}};
    for (int i = 0; i < berth_num; i++) {
        queue<pair<int, pair<int, int>>> q;
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                q.emplace(0, make_pair(berths[i].x + j, berths[i].y + k));
                berth_dis[berths[i].x + j][berths[i].y + k][i] = 0;
            }
        }
        while (!q.empty()) {
            int nowx = q.front().second.first;
            int nowy = q.front().second.second;
            int nowg = q.front().first;
            q.pop();
            for (int j = 0; j < 4; j++) {
                int nextx = nowx + dir[j][0];
                int nexty = nowy + dir[j][1];
                if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || berth_dis[nextx][nexty][i] != -1 ||
                    game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                    continue;
                //标记已经访问
                berth_dis[nextx][nexty][i] = nowg + 1;
                //加入优先队列
                q.emplace(nowg + 1, make_pair(nextx, nexty));
            }
        }
    }
    printf("OK\n");
    fflush(stdout);
}

//机器人到泊位的距离
int RobottoBerth(Robot &r) {
    int min_time = 0x3f3f3f3f;
    int min_id = -1;
    for (int i = 0; i < berth_num; i++) {
        if (berth_dis[r.x][r.y][i] == -1)continue;
        int time = berth_dis[r.x][r.y][i] + (boat_capacity < 10 ? berths[i].transport_time : 0);
        if (time < min_time) {
            min_time = time;
            min_id = i;
        }
    }
    return min_id;
}

//货物到泊位的距离
int CargotoBerth(Cargo &c) {
    int min_time = 0x3f3f3f3f;
    int min_id = -1;
    for (int i = 0; i < berth_num; i++) {
        if (berth_dis[c.x][c.y][i] == -1)continue;
        int time = berth_dis[c.x][c.y][i] + (boat_capacity < 10 ? berths[i].transport_time : 0);
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
        road.emplace(nowx, nowy);
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
    stack<pair<int, int>> road1;
    while (!road.empty()) {
        road1.push(road.front());
        road.pop();
    }
    while (!road1.empty()) {
        road.push(road1.top());
        road1.pop();
    }
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
            int h = berth_dis[nextx][nexty][berthid];
            //加入优先队列
            q.emplace(-(nextg + h), make_pair(nextg, make_pair(nextx, nexty)));
        }
    }
    if (flag == 0)return road;
    int nowx = fx, nowy = fy;
    //获取路径
    while (nowx != x || nowy != y) {
        road.emplace(nowx, nowy);
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
    stack<pair<int, int>> road1;
    while (!road.empty()) {
        road1.push(road.front());
        road.pop();
    }
    while (!road1.empty()) {
        road.push(road1.top());
        road1.pop();
    }
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

//获取一个新的物品
void RobotFindNewGoal(queue<Cargo> cars, Robot &r) {
    double max_value = 0;
    int goal_id, goal_time;
    Cargo goal = default_cargo;
    for (int i = 0; i < cargos.size(); i++) {
        Cargo cargo = cars.front();
        cars.pop();
        int berth_id = CargotoBerth(cargo);
        if (berth_id == -1)continue;
        //log("距离最近的泊位:" + to_string(berth_id));
        int berth_time =
                berth_dis[cargo.x][cargo.y][berth_id] + (boat_capacity < 10 ? berths[berth_id].transport_time : 0);
        int robot_time = CargotoRobot(cargo, robots[i]);
        double value = cargo.val * 1.0 / (berth_time + robot_time);
        if (value > max_value) {
            max_value = value;
            goal = cargo;
            goal_id = berth_id;
            goal_time = berth_time;
        }
    }
    if (max_value <= 0)return;
    queue<pair<int, int>> road = getRoadtoCargo(r.x, r.y, goal.x, goal.y);
    if (road.empty())return;
    //删除被拿走的物品
    queue<Cargo> c;
    while (!cargos.empty()) {
        Cargo ca = cargos.front();
        cargos.pop();
        if (ca.val == goal.val && ca.x == goal.x && ca.y == goal.y && ca.time == goal.time) continue;
        c.push(ca);
    }
    cargos = c;
    r.setGoal(goal, goal_time, goal_id, road);
}

//每帧的输入
int PerframeInput() {
    scanf("%d%d", &id, &money);
    log("第" + to_string(id) + "帧");
    int num;
    scanf("%d", &num);
    for (int i = 1; i <= num; i++) {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        new_cargos.emplace_back(x, y, val, id);
    }
    for (int i = 0; i < robot_num; i++) {
        scanf("%d%d%d%d", &robots[i].goods, &robots[i].x, &robots[i].y, &robots[i].status);
    }
    for (int i = 0; i < 5; i++)
        scanf("%d%d\n", &boats[i].status, &boats[i].berthid);
    char okk[100];
    scanf("%s", okk);
    return id;
}

//每帧的更新
void PerframeUpdate() {
    queue<Cargo> c;
    //当大于1000帧的时候物品消失
    for (int i = 0; i < cargos.size(); i++) {
        Cargo cargo = cargos.front();
        cargos.pop();
        if (cargo.time - id > 1000)continue;
        c.push(cargo);
    }
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
                berth_dis[cargo.x][cargo.y][berth_id] + (boat_capacity < 10 ? berths[berth_id].transport_time : 0);
        double max_value = 0.0;
        int robot_id = -1;
        //找到最优的机器人
        for (int i = 0; i < robot_num; i++) {
            //如果机器人携带货物就跳过
            if (robots[i].goods == 1)continue;
            //如果机器人到不了对应的泊位
            if (berth_dis[robots[i].x][robots[i].y][berth_id] == -1)continue;
            int robot_time = CargotoRobot(cargo, robots[i]);
            double value = cargo.val * 1.0 / (berth_time + robot_time);
            double robot_value =
                    robots[i].cargo.val * 1.0 / max(1, (int) robots[i].road.size() + robots[i].cargotoberth);
            if (robot_value > value)continue;
            if (value - robot_value > max_value) {
                max_value = value - robot_value;
                robot_id = i;
            }
        }
        //log("最优的机器人:" + to_string(robot_id));
        //如果没有找到机器人就把货物重新放入队列
        if (robot_id == -1) {
            cargos.push(cargo);
            continue;
        }
        queue<pair<int, int>> road = getRoadtoCargo(robots[robot_id].x, robots[robot_id].y, cargo.x, cargo.y);
        //log("路径长度:" + to_string(road.size()));
        //log("机器人坐标:\nx:" + to_string(robots[robot_id].x) + "\ny:" + to_string(robots[robot_id].y));
        //log("路径第一个坐标:\nx:" + to_string(road.front().first) + "\ny:" + to_string(road.front().second));
        //如果没有找到路径就把货物重新放入队列
        if (road.empty()) {
            cargos.push(cargo);
            continue;
        }
        //如果机器人在运输其他货物，就先把货物重新放入队列
        if (robots[robot_id].cargo.val != 0) {
            new_cargos.push_front(robots[robot_id].cargo);
        }
        robots[robot_id].setGoal(cargo, berth_time, berth_id, road);
        break;
    }
    //没有任务目标的机器人获得新的任务目标,有目标但是失败的机器人重新获取新的泊位
    for (int i = 0; i < robot_num; i++) {
        if (robots[i].goods == 1 &&
            robots[i].road.size() == 0) {
            int berth_id = RobottoBerth(robots[i]);
            robots[i].road = getRoadtoBerth(robots[i].x, robots[i].y, berths[berth_id].x,
                                            berths[berth_id].y, berth_id);
        }
        if (robots[i].cargo.val == 0 && robots[i].goods == 0) {
            RobotFindNewGoal(cargos, robots[i]);
        }
    }
}

//每帧的输出
void PerframeOutput() {
//    log("第" + to_string(id) + "帧输出");
//    for (int i = 0; i < berth_num; i++) {
//        if (berths[i].things.empty())continue;
//        log("第" + to_string(i) + "号泊位");
//        log("物品数量:" + to_string(berths[i].things.size()));
//    }
    for (int i = 0; i < robot_num; i++) {
        //如果机器人路径不为空就继续走
        if (!robots[i].road.empty()) {
            pair<int, int> next = robots[i].road.front();
            robots[i].road.pop();
            int dis = abs(robots[i].x - next.first) + abs(robots[i].y - next.second);
            if (dis > 1) {
                robots[i].Reset(false);
            } else {
                //机器人移动
                robots[i].move(next.first, next.second);
            }
            if (robots[i].road.empty()) {
                if (robots[i].goods == 0) {
                    //机器人没有货物,就去拿货物
                    robots[i].getThings(next.first, next.second);
                } else {
                    //机器人有货物,就去放货物
                    robots[i].putThings(next.first, next.second);
                }
            }
        } else if (robots[i].goods == 1) {//机器人正好在港口上的时候
            robots[i].putThings(robots[i].x, robots[i].y);
        }
    }
    for (int i = 0; i < boat_num; i++) {
        if (boats[i].status == 0)continue;
        if (boats[i].num == boat_capacity ||//当装满的时候直接出发开始运输
            boats[i].berthid != -1 && berths[boats[i].berthid].transport_time + id > 14950 &&
            boats[i].num != 0) {//当时间快到的时候也直接开始运输
            boats[i].go();
            continue;
        } else if (boats[i].berthid == -1) {
            int max = 0, goal = -1;
            for (int j = 0; j < berth_num; j++) {
                if (berths[j].boatid != -1)continue;
                int z = berths[j].things.size();
                if (z > max) {
                    max = z;
                    goal = j;
                }
            }
            if (goal == -1) {
                for (int j = 0; j < berth_num; j++) {
                    if (berths[j].boatid == -1) {
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
                if (berths[j].boatid != -1)continue;
                if (id + 500 + berths[j].transport_time > 14950)continue;
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
    puts("OK");
    fflush(stdout);
}

//日志信息
void log(string s) {
#ifdef _WIN32
    outfile << s << endl;
#endif
}

#ifdef _WIN32

//创建一张地图
void CreateMap() {
    char map[n][n];
    memset(map, '.', sizeof(map));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == n - 2) {
                outfile << '#';
            } else if (i < 4 && j % 5 != 0 && j <= 50) {
                outfile << 'B';
            } else {
                outfile << map[i][j];
            }
        }
        outfile << endl;
    }
}

#endif

//主函数
int main() {
#ifdef _WIN32
    // 以写模式打开文件
    outfile.open("log.txt", ios::out | ios::trunc);
    //CreateMap();
#endif
    Init();
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
