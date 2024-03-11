class Robot
{
public:
    int goods{};           // 是否携带货物
    int x{}, y{};          // 机器人的x，y坐标
    int status{};          // 机器人是否处于运行状态
    int id{};              // 机器人的编号
    double cargotoberth{}; // 机器人携带的货物到泊位的距离
    int berthid{-1};
    Cargo cargo{default_cargo}; // 机器人的货物
    queue<pair<int, int>> road; // 机器人的路径

    Robot() = default;

    Robot(int startX, int startY)
    {
        x = startX;
        y = startY;
    }

    // 机器人获取物品
    void getThings(int x, int y);

    // 机器人放下物品
    void putThings(int x, int y);

    // 设置机器人要拿取什么物品
    void setGoal(Cargo c, double dis, int brenth_id, queue<pair<int, int>> r);

    // 机器人移动
    void move(int x, int y);

    // 重置机器人
    void Reset(bool complete);
};

void Robot::getThings(int x, int y)
{
    printf("get %d\n", id);
    road = getRoadtoBerth(x, y, berths[berthid].x,
                          berths[berthid].y, berthid);
}

void Robot::putThings(int x, int y)
{
    printf("pull %d\n", id);
    int num = game_map[x][y] - '0';
    if (num < 0 || num >= berth_num)
        return;
    // log("第" + to_string(id) + "号机器人向第" + to_string(num) + "号泊位放置货物");
    int pos_x = x - berths[num].x;
    int pos_y = y - berths[num].y;
    pair<int, int> pair1(pos_x, pos_y);
    berths[num].things.push(pair1);
    this->Reset(true);
}

void Robot::move(int x, int y)
{
    if (this->x - x == 1)
        printf("move %d %d\n", id, 2);
    else if (this->x - x == -1)
        printf("move %d %d\n", id, 3);
    else if (this->y - y == 1)
        printf("move %d %d\n", id, 1);
    else if (this->y - y == -1)
        printf("move %d %d\n", id, 0);
}

void Robot::Reset(bool complete)
{
    while (!road.empty())
        road.pop();
    cargotoberth = 0;
    berthid = -1;
    if (!complete && goods == 0)
        cargos.push(cargo);
    cargo = default_cargo;
}

void Robot::setGoal(Cargo c, double dis, int brenth_id, queue<pair<int, int>> r)
{
    //    log("机器人id:" + to_string(id));
    //    log("新物品的x坐标:" + to_string(c.x));
    //    log("新物品的y坐标:" + to_string(c.y));
    //    log("新物品的平均价值:" + to_string(c.val * 1.0 / (r.size() + dis)));
    cargo = c;
    cargotoberth = dis;
    berthid = brenth_id;
    road = r;
}