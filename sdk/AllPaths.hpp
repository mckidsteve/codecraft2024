#include<vector>

#include "Path.hpp"
#include "State.hpp"
#include "ConstraintRobot.hpp"
#include "ClashRobot.hpp"

namespace Robotlib {
    struct AllPaths {
        std::vector<std::pair<int, int>> RobotPos{};//机器人坐标
        std::vector<Path> roads{};//所有路径
        std::vector<ConstraintRobot> constraints{};//目前对机器人的限制
        std::vector<ClashRobot> clashs{};//目前的冲突情况
        double cost{};//路径总花费
        double min_f{};//min_f总价
        int time{};
        bool use{};

        explicit AllPaths(int time) : time(time) {};

        std::unordered_set<State> getRobotofConstraint(int robot_id) {
            std::unordered_set<State> states;
            for (auto &constraint: constraints) {
                if (constraint.robot_id == robot_id) {
                    states.insert(State(constraint.x, constraint.y, constraint.time));
                }
            }
            return states;
        }

        //清理
        void clear() {
            roads.clear();
            constraints.clear();
        }

        //计算对应的值
        void compute() {
            min_f = cost = 0;
            int z = roads.size();
            for (int i = 0; i < z; i++) {
                min_f += roads[i].min_f;
                cost += roads[i].road.size();
            }
        }

        //改变路径
        void ChangePath(int robot_id, Path &path) {
            roads[robot_id] = path;
            int z = clashs.size();
            bool finish[z];
            memset(finish, false, sizeof(finish));
            //log("清理过时冲突");
            for (int i = 0; i < z; i++) {
                ClashRobot &clash = clashs[i];
                int nexttime = clash.time - time;
                if (robot_id != clash.robot_id_1 && robot_id != clash.robot_id_2)continue;
                if (robot_id == clash.robot_id_1 && clash.robot_id_2 == -1) {
                    if (roads[clash.robot_id_1].road.size() < nexttime) {
                        finish[i] = true;
                    } else if (clash.dian &&
                               roads[clash.robot_id_1].road[nexttime - 1] != make_pair(clash.x, clash.y)) {
                        finish[i] = true;
                    }
                    continue;
                }
                if (roads[clash.robot_id_1].road.size() < nexttime || roads[clash.robot_id_2].road.size() < nexttime) {
                    finish[i] = true;
                    continue;
                }
                //路径冲突
                if (clash.dian &&
                    roads[clash.robot_id_1].road[nexttime - 1] == roads[clash.robot_id_2].road[nexttime - 1])
                    continue;
                //交错身位
                if (!clash.dian) {
                    if (nexttime > 1) {
                        if ((roads[clash.robot_id_1].road[nexttime - 1] == roads[clash.robot_id_2].road[nexttime - 2]
                             &&
                             roads[clash.robot_id_1].road[nexttime - 2] == roads[clash.robot_id_2].road[nexttime - 1]))
                            continue;
                    } else {
                        if ((roads[clash.robot_id_1].road[nexttime - 1] == RobotPos[clash.robot_id_2]
                             && RobotPos[clash.robot_id_1] == roads[clash.robot_id_2].road[nexttime - 1]))
                            continue;
                    }
                }
                finish[i] = true;
            }
            // 从后往前遍历，这样删除元素时不会影响未检查的元素的索引
            for (int i = clashs.size() - 1; i >= 0; --i) {
                if (finish[i]) {
                    clashs.erase(clashs.begin() + i);  // 删除满足条件的元素
                }
            }
            int road_size = path.road.size();
            for (int j = 0; j < robot_num; j++) {
                if (j == robot_id)continue;
                for (int i = 0; i < road_size; i++) {
                    if (roads[j].road.size() <= i) {
                        if (roads[j].road.empty()) {
                            if (roads[robot_id].road[i] == RobotPos[j]) {
                                clashs.emplace_back(roads[robot_id].road[i], time + i + 1, robot_id, -1);
                            }
                        } else {
                            if (roads[robot_id].road[i] == roads[j].road.back()) {
                                clashs.emplace_back(roads[robot_id].road[i], time + i + 1, robot_id, -1);
                            }
                        }
                        continue;
                    }
                    if (roads[robot_id].road[i] == roads[j].road[i]) {
                        clashs.emplace_back(roads[robot_id].road[i], time + i + 1, robot_id, j);
                        continue;
                    }
                    if (i > 0) {
                        if ((roads[robot_id].road[i] == roads[j].road[i - 1]
                             && roads[robot_id].road[i - 1] == roads[j].road[i])) {
                            clashs.emplace_back(roads[robot_id].road[i], roads[j].road[i], time + i + 1, robot_id, j);
                            continue;
                        }
                    } else {
                        if ((roads[robot_id].road[i] == RobotPos[j]
                             && RobotPos[robot_id] == roads[j].road[i])) {
                            clashs.emplace_back(roads[robot_id].road[i], roads[j].road[i], time + i + 1, robot_id, j);
                            continue;
                        }
                    }
                }
            }
            //log("搜索新的冲突成功");
            //log("计算数据");
            compute();
            //log("计算数据成功");
        }

        //判断是否有冲突
        int ClashTime(int robot_id, State nowstate, State nextstate, const int basetime) {
            int num = 0;
            int i = nextstate.time - basetime - 1;
            for (int j = 0; j < robot_num; j++) {
                if (j == robot_id)continue;
                if (roads[j].road.size() <= i) {
                    if (roads[j].road.empty()) {
                        if (make_pair(nextstate.x, nextstate.y) == RobotPos[j]) {
                            num++;
                        }
                    } else {
                        if (make_pair(nextstate.x, nextstate.y) == roads[j].road.back()) {
                            num++;
                        }
                    }
                    continue;
                }
                if (make_pair(nextstate.x, nextstate.y) == roads[j].road[i]) {
                    num++;
                    continue;
                }
                if (i > 0) {
                    if ((make_pair(nextstate.x, nextstate.y) == roads[j].road[i - 1]
                         && make_pair(nowstate.x, nowstate.y) == roads[j].road[i])) {
                        num++;
                        continue;
                    }
                } else {
                    if ((make_pair(nextstate.x, nextstate.y) == RobotPos[j]
                         && make_pair(nowstate.x, nowstate.y) == roads[j].road[i])) {
                        num++;
                        continue;
                    }
                }
            }
            return num;
        }
    };
}