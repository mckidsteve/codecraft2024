void Robot::getThings(int x, int y)
{
    printf("get %d\n", id);
}

void Robot::putThings(int x, int y)
{
    printf("pull %d\n", id);
    int num = game_map[x][y] - '0';
    if (num < 0 || num >= BERTH_NUM)
        return;
    // log("第" + to_string(id) + "号机器人向第" + to_string(num) + "号泊位放置货物");
//    int pos_x = x - berths[num].x;
//    int pos_y = y - berths[num].y;
//    pair<int, int> pair1(pos_x, pos_y);
    berths[num].things.push_back(cargo);
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

void Robot::Reset(bool complete) {
    path.clear();
    berthid = -1;
    if (!complete && goods == 0)
        cargos.push(cargo);
    cargo = default_cargo;
}

void Robot::setGoal(Cargo c, int berth_id) {
    //    log("机器人id:" + to_string(id));
    //    log("新物品的x坐标:" + to_string(c.x));
    //    log("新物品的y坐标:" + to_string(c.y));
    //    log("新物品的平均价值:" + to_string(c.val * 1.0 / (r.size() + dis)));
    cargo = c;
    berthid = berth_id;
    path.clear();
    ecbs->Search(id , cargo.x , cargo.y , zhen , -1);
}


void Boat::go()
{
    // log("第" + to_string(id) + "号船开向虚拟点");
    printf("go %d\n", id);
    if (berthid != -1) {
        //删除id
        for (auto it = berths[berthid].boatids.begin(); it != berths[berthid].boatids.end(); it++) {
            if (*it == id) {
                berths[berthid].boatids.erase(it);
                break;
            }
        }
    }
    num = 0;
    berthid = -1;
    status = 0;
}

void Boat::ship(int goal)
{
    if (goal == -1)
        return;
    //log("第" + to_string(id) + "号船开向第" + to_string(goal) + "号泊位");
    printf("ship %d %d\n", id, goal);
    if (berthid != -1) {
        for (auto it = berths[berthid].boatids.begin(); it != berths[berthid].boatids.end(); it++) {
            if (*it == id) {
                berths[berthid].boatids.erase(it);
                break;
            }
        }
    }
    berths[goal].boatids.push_back(id);
    berthid = goal;
    status = 0;
}
void Berth::stowage()
{
    if (boatids.empty())
        return;
    int boatid = boatids.front();
    if (boats[boatid].status == 0)
        return;
    if (things.empty())
        return;
    // log("第" + to_string(zhen) + "号泊位向第" + to_string(boatid) + "号船装载货物");
    for (int i = 0; i < loading_speed; i++) {
        if (boats[boatid].num < boat_capacity && !things.empty()) {
            // pair<int, int> a = things.front();
            things.erase(things.begin());
            boats[boatid].num++;
        } else
            return;
    }
}

double Berth::transport_time_value() const
{
//    if (boat_capacity >= 10)
        return 0;
//    else
//        return (10 - boat_capacity) * 0.1 * transport_time;
}
