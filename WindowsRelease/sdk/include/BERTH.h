class Berth
{
public:
    int x{};                      // 泊位的x
    int y{};                      // 泊位的y
    int id{};                     // 泊位的编号
    int transport_time{};         // 到达虚拟点的时间
    int loading_speed{};          // 装货速度
    queue<pair<int, int>> things; // 泊位上的物品,储存的是相对坐标
    int boatid{-1};               // 停靠或者准备停靠的船只编号

    Berth() = default;

    Berth(int x, int y, int transport_time, int loading_speed)
        : x(x), y(y), transport_time(transport_time), loading_speed(loading_speed) {}

    // 泊位向船装货
    void stowage();

    // 装船时间造成的价值
    double transport_time_value();
}

void
Berth::stowage()
{
    if (boatid == -1)
        return;
    if (boats[boatid].status == 0)
        return;
    if (things.empty())
        return;
    // log("第" + to_string(id) + "号泊位向第" + to_string(boatid) + "号船装载货物");
    for (int i = 0; i < loading_speed; i++)
    {
        if (boats[boatid].num < boat_capacity && !things.empty())
        {
            // pair<int, int> a = things.front();
            things.pop();
            boats[boatid].num++;
        }
        else
            return;
    }
}

double Berth::transport_time_value()
{
    if (boat_capacity >= 10)
        return 0;
    else
        return (10 - boat_capacity) * 0.1 * transport_time;
}
