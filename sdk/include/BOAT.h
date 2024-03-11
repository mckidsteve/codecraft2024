// 船
class Boat
{
public:
    int num{};     // 装载的数量
    int berthid{}; // 停泊的泊位
    int status{};  // 船的状态
    int id{};      // 船的id
    Boat() {}

    void go();

    void ship(int id);
};

void Boat::go()
{
    // log("第" + to_string(id) + "号船开向虚拟点");
    printf("go %d\n", id);
    if (berthid != -1)
    {
        berths[berthid].boatid = -1; // 对应的船归零
    }
    num = 0;
    berthid = -1;
    status = 0;
}

void Boat::ship(int goal)
{
    if (goal == -1)
        return;
    // log("第" + to_string(id) + "号船开向第" + to_string(goal) + "号泊位");
    printf("ship %d %d\n", id, goal);
    if (berthid != -1)
    {
        berths[berthid].boatid = -1;
    }
    berths[goal].boatid = id;
    berthid = goal;
    status = 0;
}