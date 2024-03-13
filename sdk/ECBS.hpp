#pragma once

#include "AStarEpsilon.hpp"
#include "ROBOT.hpp"
#include "AllPaths.hpp"

namespace Robotlib {
    class ECBS {
    public:
        ECBS(const std::vector<std::vector<char>> &map, double w, std::vector<std::vector<std::vector<int>>> &berth_dis)
                : map_(map), w(w), n((int) map.size()), berth_dis(berth_dis),
                  starEpsilon(AStarEpsilon(map, w, berth_dis)) {};
        Robot *robots[robot_num]{};

        //机器人需要有自己的目标cargo或者Berth
        void Search(int robot_id, int end_x, int end_y, int time, int berth_id) {
            //路径不为空就不进行搜索
            if (!robots[robot_id]->path.road.empty())return;
            AllPaths *all_paths = new AllPaths(time);
            for (int i = 0; i < robot_num; i++) {
                all_paths->roads.push_back(robots[i]->path);
                all_paths->RobotPos.emplace_back(robots[i]->x, robots[i]->y);
            }
            //初始化搜索路径
            Path path;
            starEpsilon.SearchToBerth(path, time, make_pair(robots[robot_id]->x, robots[robot_id]->y),
                                      make_pair(end_x, end_y), all_paths->getRobotofConstraint(robot_id), berth_id);
            all_paths->ChangePath(robot_id, path);
            std::PriorityQueue<AllPaths *, AllPathsPtrComparator> open_set;//开放集合
            std::priority_queue<AllPaths *, std::vector<AllPaths *>, AllPathsCompareNode> focal_set;//焦点集合
            std::unordered_set<AllPaths *> finished_set;//完成集合
            open_set.push(all_paths);
            focal_set.push(all_paths);
            finished_set.insert(all_paths);
            double min_f = all_paths->min_f;
            bool flag = false;
            AllPaths *goal;
            int num = 0;
            while (!open_set.empty()) {
                double old_min_f = min_f;
                min_f = open_set.top()->min_f;
                if (min_f > old_min_f) {
                    for (auto it = open_set.begin(); it != open_set.end(); ++it) {
                        AllPaths *node = *it;
                        if (node->cost > old_min_f * w && node->cost <= min_f * w)
                            focal_set.push(node);
                    }
                }
                AllPaths *node = focal_set.top();
//                bool f = true;
//                int count = 0;
//                while (!node->clashs.empty() && f) {
//                    count = 1;
//                }
                //判断是否为目标状态
                if (node->clashs.empty()) {
                    flag = true;
                    goal = node;
                    break;
                }
                focal_set.pop();
                int z = node->clashs.size();
                int next_robot_id;
                int next_end_x, next_end_y;
                int next_berth_id;
                for (int i = 0; i < z; i++) {
                    ClashRobot &clashRobot = node->clashs[i];
                    if (clashRobot.dian) {
                        AllPaths *next_node_1 = new AllPaths(*node);
                        next_node_1->constraints.emplace_back(clashRobot.x, clashRobot.y, clashRobot.time,
                                                              clashRobot.robot_id_1);
                        Path next_path_1;
                        next_robot_id = clashRobot.robot_id_1;
                        if (clashRobot.robot_id_1 == robot_id) {
                            next_end_x = end_x;
                            next_end_y = end_y;
                            next_berth_id = berth_id;
                        } else {
                            if (robots[next_robot_id]->cargo.val != 0 && robots[next_robot_id]->goods == 0) {
                                next_end_x = robots[next_robot_id]->cargo.x;
                                next_end_y = robots[next_robot_id]->cargo.y;
                                next_berth_id = -1;
                            } else if (robots[next_robot_id]->goods == 1) {
                                next_berth_id =
                                        robots[next_robot_id]->berthid == -1
                                        ? RobottoBerth(*robots[next_robot_id])
                                        : robots[next_robot_id]->berthid;
                                next_end_x = berths[next_berth_id].x;
                                next_end_y = berths[next_berth_id].y;
                            }
                        }
                        starEpsilon.SearchToBerth(next_path_1, time,
                                                  make_pair(robots[next_robot_id]->x, robots[next_robot_id]->y),
                                                  make_pair(next_end_x, next_end_y),
                                                  next_node_1->getRobotofConstraint(next_robot_id),
                                                  next_berth_id);
                        next_node_1->ChangePath(next_robot_id, next_path_1);
                        finished_set.insert(next_node_1);
                        open_set.push(next_node_1);
                        if (next_node_1->cost <= min_f * w)
                            focal_set.push(next_node_1);
                        if (next_node_1->clashs.empty()) {
                            flag = true;
                            goal = next_node_1;
                            break;
                        }

                        AllPaths *next_node_2 = new AllPaths(*node);
                        next_node_2->constraints.emplace_back(clashRobot.x, clashRobot.y, clashRobot.time,
                                                              clashRobot.robot_id_2);
                        Path next_path_2;
                        next_robot_id = clashRobot.robot_id_2;
                        if (clashRobot.robot_id_2 == robot_id) {
                            next_end_x = end_x;
                            next_end_y = end_y;
                            next_berth_id = berth_id;
                        } else {
                            if (robots[next_robot_id]->cargo.val != 0 && robots[next_robot_id]->goods == 0) {
                                next_end_x = robots[next_robot_id]->cargo.x;
                                next_end_y = robots[next_robot_id]->cargo.y;
                                next_berth_id = -1;
                            } else if (robots[next_robot_id]->goods == 1) {
                                next_berth_id =
                                        robots[next_robot_id]->berthid == -1
                                        ? RobottoBerth(*robots[next_robot_id])
                                        : robots[next_robot_id]->berthid;
                                next_end_x = berths[next_berth_id].x;
                                next_end_y = berths[next_berth_id].y;
                            }
                        }
                        starEpsilon.SearchToBerth(next_path_2, time,
                                                  make_pair(robots[next_robot_id]->x, robots[next_robot_id]->y),
                                                  make_pair(next_end_x, next_end_y),
                                                  next_node_2->getRobotofConstraint(next_robot_id),
                                                  next_berth_id);
                        next_node_2->ChangePath(next_robot_id, next_path_2);
                        finished_set.insert(next_node_2);
                        open_set.push(next_node_2);
                        if (next_node_2->cost <= min_f * w)
                            focal_set.push(next_node_2);
                        if (next_node_2->clashs.empty()) {
                            flag = true;
                            goal = next_node_2;
                            break;
                        }
                    } else {
                        AllPaths *next_node_1 = new AllPaths(*node);
                        next_node_1->constraints.emplace_back(clashRobot.x, clashRobot.y, clashRobot.time,
                                                              clashRobot.robot_id_1);
                        Path next_path_1;
                        next_robot_id = clashRobot.robot_id_1;
                        if (clashRobot.robot_id_1 == robot_id) {
                            next_end_x = end_x;
                            next_end_y = end_y;
                            next_berth_id = berth_id;
                        } else {
                            if (robots[next_robot_id]->cargo.val != 0 && robots[next_robot_id]->goods == 0) {
                                next_end_x = robots[next_robot_id]->cargo.x;
                                next_end_y = robots[next_robot_id]->cargo.y;
                                next_berth_id = -1;
                            } else if (robots[next_robot_id]->goods == 1) {
                                next_berth_id =
                                        robots[next_robot_id]->berthid == -1
                                        ? RobottoBerth(*robots[next_robot_id])
                                        : robots[next_robot_id]->berthid;
                                next_end_x = berths[next_berth_id].x;
                                next_end_y = berths[next_berth_id].y;
                            }
                        }
                        starEpsilon.SearchToBerth(next_path_1, time,
                                                  make_pair(robots[next_robot_id]->x, robots[next_robot_id]->y),
                                                  make_pair(next_end_x, next_end_y),
                                                  next_node_1->getRobotofConstraint(next_robot_id),
                                                  next_berth_id);
                        next_node_1->ChangePath(next_robot_id, next_path_1);
                        finished_set.insert(next_node_1);
                        open_set.push(next_node_1);
                        if (next_node_1->cost <= min_f * w)
                            focal_set.push(next_node_1);
                        if (next_node_1->clashs.empty()) {
                            flag = true;
                            goal = next_node_1;
                            break;
                        }

                        AllPaths *next_node_2 = new AllPaths(*node);
                        next_node_2->constraints.emplace_back(clashRobot.x2, clashRobot.y2, clashRobot.time,
                                                              clashRobot.robot_id_2);
                        Path next_path_2;
                        next_robot_id = clashRobot.robot_id_2;
                        if (clashRobot.robot_id_2 == robot_id) {
                            next_end_x = end_x;
                            next_end_y = end_y;
                            next_berth_id = berth_id;
                        } else {
                            if (robots[next_robot_id]->cargo.val != 0 && robots[next_robot_id]->goods == 0) {
                                next_end_x = robots[next_robot_id]->cargo.x;
                                next_end_y = robots[next_robot_id]->cargo.y;
                                next_berth_id = -1;
                            } else if (robots[next_robot_id]->goods == 1) {
                                next_berth_id =
                                        robots[next_robot_id]->berthid == -1
                                        ? RobottoBerth(*robots[next_robot_id])
                                        : robots[next_robot_id]->berthid;
                                next_end_x = berths[next_berth_id].x;
                                next_end_y = berths[next_berth_id].y;
                            }
                        }
                        starEpsilon.SearchToBerth(next_path_2, time,
                                                  make_pair(robots[next_robot_id]->x, robots[next_robot_id]->y),
                                                  make_pair(next_end_x, next_end_y),
                                                  next_node_2->getRobotofConstraint(next_robot_id),
                                                  next_berth_id);
                        next_node_2->ChangePath(next_robot_id, next_path_2);
                        finished_set.insert(next_node_2);
                        open_set.push(next_node_2);
                        if (next_node_2->cost <= min_f * w)
                            focal_set.push(next_node_2);
                        if (next_node_2->clashs.empty()) {
                            flag = true;
                            goal = next_node_2;
                            break;
                        }
                    }
                }
                node->use = true;
                while (!open_set.empty() && open_set.top()->use)open_set.pop();
            }
            if (!flag) {
                return;
            }
            log("ECBS成功");
            for (int i = 0; i < robot_num; i++) {
                robots[i]->path = goal->roads[i];
            }
        }

    private:
        double w;//次优性范围
        int n;//地图大小
        AStarEpsilon starEpsilon;//A*算法
        //机器人
        std::vector<std::vector<char>> map_;//地图
        std::vector<std::vector<std::vector<int>>> berth_dis;//泊位距离
        //节点指针比较函数
        struct AllPathsPtrComparator {
            bool operator()(const AllPaths *a, const AllPaths *b) const {
                return a->cost > b->cost;
            }
        };

        //焦点比较函数
        struct AllPathsCompareNode {
            bool operator()(const AllPaths *lhs, const AllPaths *rhs) const {
                return lhs->cost > rhs->cost;
            }
        };
    };

}