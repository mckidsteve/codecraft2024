#include<vector>
#include <cstring>

#define Ttime 3

#include "Path.hpp"
#include "State.hpp"
#include "ConstraintRobot.hpp"
#include "ClashRobot.hpp"

namespace Robotlib {
    struct AllPaths {
        std::vector<std::pair<int, int>> RobotPos{};//机器人坐标
        std::vector<Path> roads{};//所有路径
        std::vector<ConstraintRobot> constraints{};//目前对机器人的限制
        std::unordered_set<ClashRobot> clashs{};//目前的冲突情况
        double cost{};//路径总花费
        double min_f{};//min_f总价
        int time{};
        bool use{};
        bool infoc{};

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
                if (roads[i].min_f != 0)cost += roads[i].road.size();
            }
        }

        //改变路径
        void ChangePath(int robot_id, Path &path) {
            roads[robot_id] = path;
            int z = clashs.size();
            bool finish[z];
            memset(finish, false, sizeof(finish));
            //log("清理过时冲突");
            //遍历clashs
            int i = 0;
            for (auto it = clashs.begin(); it != clashs.end();) {
                ClashRobot clash = *it;
                int nexttime = clash.time - time;
                if (robot_id != clash.robot_id_1 && robot_id != clash.robot_id_2) {
                    it++;
                    continue;
                }
                if (robot_id == clash.robot_id_1 && clash.robot_id_2 == -1) {
                    if (roads[clash.robot_id_1].road.size() < nexttime) {
                        it = clashs.erase(it);
                        continue;
                    } else if (clash.dian &&
                               roads[clash.robot_id_1].road[nexttime - 1] != make_pair(clash.x, clash.y)) {
                        it = clashs.erase(it);
                        continue;
                    }
                    it++;
                    continue;
                }
                if (roads[clash.robot_id_1].road.size() < nexttime || roads[clash.robot_id_2].road.size() < nexttime) {
                    it = clashs.erase(it);
                    continue;
                }
                //路径冲突
                if (clash.dian &&
                    roads[clash.robot_id_1].road[nexttime - 1] == roads[clash.robot_id_2].road[nexttime - 1]) {
                    it++;
                    continue;
                }
                //交错身位
                if (!clash.dian) {
                    if (nexttime > 1) {
                        if ((roads[clash.robot_id_1].road[nexttime - 1] == roads[clash.robot_id_2].road[nexttime - 2]
                             &&
                             roads[clash.robot_id_1].road[nexttime - 2] ==
                             roads[clash.robot_id_2].road[nexttime - 1])) {
                            it++;
                            continue;
                        }
                    } else {
                        if ((roads[clash.robot_id_1].road[nexttime - 1] == RobotPos[clash.robot_id_2]
                             && RobotPos[clash.robot_id_1] == roads[clash.robot_id_2].road[nexttime - 1])) {
                            it++;
                            continue;
                        }
                    }
                }
                it = clashs.erase(it);
            }
            int road_size = path.road.size();
            for (int j = 0; j < robot_num; j++) {
                if (j == robot_id)continue;
                for (int i = 0; i < road_size; i++) {
                    unsigned long long int size1 = roads[j].road.size();
                    if (size1 <= i) {
                        if (i - size1 > Ttime)break;
                        if (size1 == 0) {
                            if (roads[robot_id].road[i] == RobotPos[j]) {
                                ClashRobot clashRobot(roads[robot_id].road[i], time + i + 1, robot_id, -1);
                                if (clashs.count(clashRobot) == 0) {
                                    clashs.insert(clashRobot);
                                }
                            }
                        } else {
                            if (roads[robot_id].road[i] == roads[j].road.back()) {
                                ClashRobot clashRobot(roads[robot_id].road[i], time + i + 1, robot_id, -1);
                                if (clashs.count(clashRobot) == 0) {
                                    clashs.insert(clashRobot);
                                }
                            }
                        }
                        continue;
                    }
                    if (roads[robot_id].road[i] == roads[j].road[i]) {
                        ClashRobot clashRobot(roads[robot_id].road[i], time + i + 1, robot_id, j);
                        if (clashs.count(clashRobot) == 0) {
                            clashs.insert(clashRobot);
                        }
                        continue;
                    }
                    if (i > 0) {
                        if ((roads[robot_id].road[i] == roads[j].road[i - 1]
                             && roads[robot_id].road[i - 1] == roads[j].road[i])) {
                            ClashRobot clashRobot(roads[robot_id].road[i], roads[j].road[i], time + i + 1, robot_id, j);
                            if (clashs.count(clashRobot) == 0) {
                                clashs.insert(clashRobot);
                            }
                            continue;
                        }
                    } else {
                        if ((roads[robot_id].road[i] == RobotPos[j]
                             && RobotPos[robot_id] == roads[j].road[i])) {
                            ClashRobot clashRobot(roads[robot_id].road[i], roads[j].road[i], time + i + 1, robot_id, j);
                            if (clashs.count(clashRobot) == 0) {
                                clashs.insert(clashRobot);
                            }
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
                unsigned long long int size1 = roads[j].road.size();
                if (size1 <= i) {
                    if (i - size1 > Ttime)continue;
                    if (roads[j].road.empty()) {
                        if (nextstate.x == RobotPos[j].first && nextstate.y == RobotPos[j].second) {
                            num++;
                        }
                    } else {
                        if (nextstate.x == roads[j].road.back().first && nextstate.y == roads[j].road.back().second) {
                            num++;
                        }
                    }
                    continue;
                }
                if (nextstate.x == roads[j].road[i].first && nextstate.y == roads[j].road[i].second) {
                    num++;
                    continue;
                }
                if (i > 0) {
                    if ((nextstate.x == roads[j].road[i - 1].first && nextstate.y == roads[j].road[i - 1].second)
                        && (nowstate.x == roads[j].road[i].first && nowstate.y == roads[j].road[i].second)) {
                        num++;
                        continue;
                    }
                } else {
                    if ((nextstate.x == RobotPos[j].first && nextstate.y == RobotPos[j].second)
                        && (nowstate.x == roads[j].road[i].first && nowstate.y == roads[j].road[i].second)) {
                        num++;
                        continue;
                    }
                }
            }
            return num;
        }
    };
}