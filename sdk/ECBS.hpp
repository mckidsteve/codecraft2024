#pragma once

#include "AStarEpsilon.hpp"
#include "ROBOT.hpp"

namespace Robotlib {
    class ECBS {
    public:
        ECBS(const std::vector<std::vector<char>> &map, double w, std::vector<std::vector<std::vector<int>>> &berth_dis)
                : map_(map), w(w), n((int) map.size()), berth_dis(berth_dis),
                  starEpsilon(AStarEpsilon(map, w, berth_dis)) {};
        Robot *robots[robot_num]{};

        Path Search(int robot_id, int end_x, int end_y, int time, int berth_id) {
            Path path;

            return path;
        }

    private:
        double w;//次优性范围
        int n;//地图大小
        AStarEpsilon starEpsilon;//A*算法
        //机器人
        std::vector<std::vector<char>> map_;//地图
        std::vector<std::vector<std::vector<int>>> berth_dis;//泊位距离
    };

}