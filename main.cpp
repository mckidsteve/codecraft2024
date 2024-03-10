#include <bits/stdc++.h>

using namespace std;

const int n = 200;
const int robot_num = 10;
const int boat_num = 5;
const int berth_num = 10;
int money;//Ǯ�� ��������
int boat_capacity;//��װ������
//����Ӧ��λ�ľ���
int berth_dis[n][n][berth_num];
#ifdef _WIN32
ofstream outfile;//��־�ļ�
#endif
int id;
/**
 * ��ͼ
 * .�յ�
 * *����
 * #�ϰ�
 * 0-9��λ���ڵ�λ��
 * */
char game_map[n][n];

class Cargo;

class Robot;

void log(string s);

queue<pair<int, int>> getRoadtoBerth(int x, int y, int x1, int y1, int berthid);

void RobotFindNewGoal(queue<Cargo> cars, Robot& r);

//����
struct Cargo {
    int x, y;//���������
    int val;//����ļ�ֵ
    int time;//����Ĳ���ʱ��
    Cargo(int x, int y, int val, int time) : x(x), y(y), val(val), time(time) {}
} default_cargo(0, 0, 0, 0);

//���л������
queue<Cargo> cargos;
//�²����Ļ������
deque<Cargo> new_cargos;

//������
class Robot {
public:
    int goods{};//�Ƿ�Я������
    int x{}, y{};//�����˵�x��y����
    int status{};//�������Ƿ�������״̬
    int id{};//�����˵ı��
    int cargotoberth{};//������Я���Ļ��ﵽ��λ�ľ���
    int berthid{ -1 };
    Cargo cargo{ default_cargo };//�����˵Ļ���
    queue<pair<int, int>> road;//�����˵�·��

    Robot() = default;

    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }

    //�����˻�ȡ��Ʒ
    void getThings(int x, int y);

    //�����˷�����Ʒ
    void putThings(int x, int y);

    //���û�����Ҫ��ȡʲô��Ʒ
    void setGoal(Cargo c, int dis, int brenth_id, queue<pair<int, int>> r);

    //�������ƶ�
    void move(int x, int y);

    //���û�����
    void Reset(bool complete);
} robots[robot_num];

//��
class Boat {
public:
    int num{};//װ�ص�����
    int berthid{};//ͣ���Ĳ�λ
    int status{};//����״̬
    int id{};//����id
    Boat() {}

    void go();

    void ship(int id);
} boats[boat_num];

//��λ
class Berth {
public:
    int x{};//��λ��x
    int y{};//��λ��y
    int id{};//��λ�ı��
    int transport_time{};//����������ʱ��
    int loading_speed{};//װ���ٶ�
    queue<pair<int, int>> things;//��λ�ϵ���Ʒ,��������������
    int boatid{ -1 };//ͣ������׼��ͣ���Ĵ�ֻ���

    Berth() = default;

    Berth(int x, int y, int transport_time, int loading_speed)
        : x(x), y(y), transport_time(transport_time), loading_speed(loading_speed) {}

    //��λ��װ��
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
    //log("��" + to_string(id) + "�Ż��������" + to_string(num) + "�Ų�λ���û���");
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
    //    log("������id:" + to_string(id));
    //    log("����Ʒ��x����:" + to_string(c.x));
    //    log("����Ʒ��y����:" + to_string(c.y));
    //    log("����Ʒ��ƽ����ֵ:" + to_string(c.val * 1.0 / (r.size() + dis)));
    cargo = c;
    cargotoberth = dis;
    berthid = brenth_id;
    road = r;
}

void Boat::go() {
    //log("��" + to_string(id) + "�Ŵ����������");
    printf("go %d\n", id);
    if (berthid != -1) {
        berths[berthid].boatid = -1;//��Ӧ�Ĵ�����
    }
    num = 0;
    berthid = -1;
    status = 0;
}

void Boat::ship(int goal) {
    if (goal == -1)return;
    //log("��" + to_string(id) + "�Ŵ������" + to_string(goal) + "�Ų�λ");
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
    //log("��" + to_string(id) + "�Ų�λ���" + to_string(boatid) + "�Ŵ�װ�ػ���");
    for (int i = 0; i < loading_speed; i++) {
        if (boats[boatid].num < boat_capacity && !things.empty()) {
            //pair<int, int> a = things.front();
            things.pop();
            boats[boatid].num++;
        }
        else return;
    }
}

//��ʼ��
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
    //��ʼ�����в�λ������λ�õľ���
    memset(berth_dis, -1, sizeof(berth_dis));
    int dir[4][2] = { {0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0} };
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
                //����Ѿ�����
                berth_dis[nextx][nexty][i] = nowg + 1;
                //�������ȶ���
                q.emplace(nowg + 1, make_pair(nextx, nexty));
            }
        }
    }
    printf("OK\n");
    fflush(stdout);
}

//�����˵���λ�ľ���
int RobottoBerth(Robot& r) {
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

//���ﵽ��λ�ľ���
int CargotoBerth(Cargo& c) {
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

//A*�㷨
queue<pair<int, int>> Astar(int x, int y, int x1, int y1) {
    queue<pair<int, int>> road;
    int dir[4][2] = { {0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0} };
    int dis[n][n];
    memset(dis, -1, sizeof(dis));
    //���ȶ���
    priority_queue<pair<int, pair<int, pair<int, int>>>> q;
    q.emplace(0, make_pair(0, make_pair(x, y)));
    dis[x][y] = 0;
    int flag = 0;
    //A*�㷨
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
            //����Ѿ�����
            dis[nextx][nexty] = nowg + 1;
            //���ۺ���
            int nextg = nowg + 1;
            //�����پ���
            int h = abs(nextx - x1) + abs(nexty - y1);
            //�������ȶ���
            q.emplace(-(nextg + h), make_pair(nextg, make_pair(nextx, nexty)));
        }
    }
    if (flag == 0)return road;
    int nowx = x1, nowy = y1;
    //��ȡ·��
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
    //��ת·��
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
    int dir[4][2] = { {0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0} };
    int dis[n][n];
    memset(dis, -1, sizeof(dis));
    //���ȶ���
    priority_queue<pair<int, pair<int, pair<int, int>>>> q;
    q.emplace(0, make_pair(0, make_pair(x, y)));
    dis[x][y] = 0;
    int flag = 0;
    int fx, fy;//�յ�����
    //A*�㷨
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
            //����Ѿ�����
            dis[nextx][nexty] = nowg + 1;
            //���ۺ���
            int nextg = nowg + 1;
            //�����پ���
            int h = berth_dis[nextx][nexty][berthid];
            //�������ȶ���
            q.emplace(-(nextg + h), make_pair(nextg, make_pair(nextx, nexty)));
        }
    }
    if (flag == 0)return road;
    int nowx = fx, nowy = fy;
    //��ȡ·��
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
    //��ת·��
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

//��ȡ����Ʒ��·��
queue<pair<int, int>> getRoadtoCargo(int x, int y, int x1, int y1) {
    return Astar(x, y, x1, y1);
}

//��ȡ����λ��·��
queue<pair<int, int>> getRoadtoBerth(int x, int y, int x1, int y1, int berthid) {
    return Astar(x, y, x1, y1, berthid);
}

//���ﵽ�����˵ľ���,�����پ���
int CargotoRobot(Cargo& c, Robot& r) {
    return abs(c.x - r.x) + abs(c.y - r.y);
}

//��ȡһ���µ���Ʒ
void RobotFindNewGoal(queue<Cargo> cars, Robot& r) {
    double max_value = 0;
    int goal_id, goal_time;
    Cargo goal = default_cargo;
    for (int i = 0; i < cargos.size(); i++) {
        Cargo cargo = cars.front();
        cars.pop();
        int berth_id = CargotoBerth(cargo);
        if (berth_id == -1)continue;
        //log("��������Ĳ�λ:" + to_string(berth_id));
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
    //ɾ�������ߵ���Ʒ
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

//ÿ֡������
void PerframeInput() {
    scanf("%d%d", &id, &money);
    log("��" + to_string(id) + "֡");
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
}

//ÿ֡�ĸ���
void PerframeUpdate() {
    queue<Cargo> c;
    //������1000֡��ʱ����Ʒ��ʧ
    for (int i = 0; i < cargos.size(); i++) {
        Cargo cargo = cargos.front();
        cargos.pop();
        if (cargo.time - id > 1000)continue;
        c.push(cargo);
    }
    //ÿ֡����һ���»���
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
        //�ҵ����ŵĻ�����
        for (int i = 0; i < robot_num; i++) {
            //���������Я�����������
            if (robots[i].goods == 1)continue;
            //��������˵����˶�Ӧ�Ĳ�λ
            if (berth_dis[robots[i].x][robots[i].y][berth_id] == -1)continue;
            int robot_time = CargotoRobot(cargo, robots[i]);
            double value = cargo.val * 1.0 / (berth_time + robot_time);
            double robot_value =
                robots[i].cargo.val * 1.0 / max(1, (int)robots[i].road.size() + robots[i].cargotoberth);
            if (robot_value > value)continue;
            if (value - robot_value > max_value) {
                max_value = value - robot_value;
                robot_id = i;
            }
        }
        //log("���ŵĻ�����:" + to_string(robot_id));
        //���û���ҵ������˾Ͱѻ������·������
        if (robot_id == -1) {
            cargos.push(cargo);
            continue;
        }
        queue<pair<int, int>> road = getRoadtoCargo(robots[robot_id].x, robots[robot_id].y, cargo.x, cargo.y);
        //log("·������:" + to_string(road.size()));
        //log("����������:\nx:" + to_string(robots[robot_id].x) + "\ny:" + to_string(robots[robot_id].y));
        //log("·����һ������:\nx:" + to_string(road.front().first) + "\ny:" + to_string(road.front().second));
        //���û���ҵ�·���Ͱѻ������·������
        if (road.empty()) {
            cargos.push(cargo);
            continue;
        }
        //�������������������������Ȱѻ������·������
        if (robots[robot_id].cargo.val != 0) {
            new_cargos.push_front(robots[robot_id].cargo);
        }
        robots[robot_id].setGoal(cargo, berth_time, berth_id, road);
        break;
    }
    //û������Ŀ��Ļ����˻���µ�����Ŀ��,��Ŀ�굫��ʧ�ܵĻ��������»�ȡ�µĲ�λ
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
        if (robots[i].cargo.val != 0 && (robots[i].road.size() + id - robots[i].cargo.time > 1000)) {
            robots[i].Reset(false);
        }
    }
}

//ÿ֡�����
void PerframeOutput() {
    //    log("��" + to_string(id) + "֡���");
    //    for (int i = 0; i < berth_num; i++) {
    //        if (berths[i].things.empty())continue;
    //        log("��" + to_string(i) + "�Ų�λ");
    //        log("��Ʒ����:" + to_string(berths[i].things.size()));
    //    }
    for (int i = 0; i < robot_num; i++) {
        //���������·����Ϊ�վͼ�����
        if (!robots[i].road.empty()) {
            pair<int, int> next = robots[i].road.front();
            robots[i].road.pop();
            int dis = abs(robots[i].x - next.first) + abs(robots[i].y - next.second);
            if (dis > 1) {
                robots[i].Reset(false);
            }
            else {
                //�������ƶ�
                robots[i].move(next.first, next.second);
            }
            if (robots[i].road.empty()) {
                if (robots[i].goods == 0) {
                    //������û�л���,��ȥ�û���
                    robots[i].getThings(next.first, next.second);
                }
                else {
                    //�������л���,��ȥ�Ż���
                    robots[i].putThings(next.first, next.second);
                }
            }
        }
        else if (robots[i].goods == 1) {//�����������ڸۿ��ϵ�ʱ��
            robots[i].putThings(robots[i].x, robots[i].y);
        }
    }
    for (int i = 0; i < boat_num; i++) {
        if (boats[i].status == 0)continue;
        if (boats[i].num == boat_capacity ||//��װ����ʱ��ֱ�ӳ�����ʼ����
            boats[i].berthid != -1 && berths[boats[i].berthid].transport_time + id > 14950 &&
            boats[i].num != 0) {//��ʱ��쵽��ʱ��Ҳֱ�ӿ�ʼ����
            boats[i].go();
            continue;
        }
        else if (boats[i].berthid == -1) {
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
        }
        else if (!berths[boats[i].berthid].things.empty()) {
            continue;
        }
        else {
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

//��־��Ϣ
void log(string s) {
#ifdef _WIN32
    outfile << s << endl;
#endif
}

#ifdef _WIN32

//����һ�ŵ�ͼ
void CreateMap() {
    char map[n][n];
    memset(map, '.', sizeof(map));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == n - 2) {
                outfile << '#';
            }
            else if (i < 4 && j % 5 != 0 && j <= 50) {
                outfile << 'B';
            }
            else {
                outfile << map[i][j];
            }
        }
        outfile << endl;
    }
}

#endif

//������
int main() {
#ifdef _WIN32
    // ��дģʽ���ļ�
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
