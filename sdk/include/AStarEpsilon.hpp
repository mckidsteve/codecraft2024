#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include "State.hpp"
#include "Queue.hpp"
#include "log.hpp"

namespace Robotlib {
    /**
     * A*ε搜索算法
     * 该类实现了A*ε搜索算法，该算法是A*搜索算法的改进版本。
     * 该算法在搜索过程中使用了次优性范围，以便在搜索过程中
     * 保持次优性。这样，我们可以在搜索过程中找到次优解。
     * */
    class AStarEpsilon {
    public:
        AStarEpsilon(const std::vector<std::vector<char>> &map, double w)
                : map_(map), w(w), n(map.size()) {}

        bool
        Search(std::vector<std::pair<int, int>> &path, int time, std::pair<int, int> start, std::pair<int, int> goal,
               std::unordered_set<State> &obstacles) {
            path.clear();
            std::PriorityQueue<Node *, NodePtrComparator> open_set;//开放集合
            std::priority_queue<Node *, std::vector<Node *>, CompareNode> focal_set;//焦点集合
            std::unordered_set<State> close_set;//关闭集合
            std::unordered_set<Node *> finished_set;//完成集合
            State goal_state(goal.first, goal.second, 0x3f3f3f3f);
            Node *start_node = new Node(State(start, time), 0,
                                        heuristic(State(start, time), goal_state),
                                        nullptr);
            open_set.push(start_node);
            focal_set.push(start_node);
            finished_set.insert(start_node);
            close_set.insert(start_node->state);
            double min_f = start_node->f;
            bool flag = false;
            while (!open_set.empty()) {
                double old_min_f = min_f;
                min_f = open_set.top()->f;
                if (min_f > old_min_f) {
                    for (auto it = open_set.begin(); it != open_set.end(); ++it) {
                        Node *node = *it;
                        if (node->f > old_min_f * w && node->f <= min_f * w)
                            focal_set.push(node);
                    }
                }
                Node *node = focal_set.top();
                //判断是否为目标状态
                if (node->isGoal(goal)) {
                    flag = true;
                    break;
                }
                focal_set.pop();
                //open_set.remove(node);
                node->use = true;
                int z = directions_.size();
                for (int i = 0; i < z; i++) {
                    int nextx = node->state.x + directions_[i].first;
                    int nexty = node->state.y + directions_[i].second;
                    int nexttime = node->state.time + 1;
                    if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n ||//越界
                        map_[nextx][nexty] == '#' || map_[nextx][nexty] == '*' ||//障碍物
                        obstacles.count(State(nextx, nexty, nexttime)) != 0) {//时间约束障碍物
                        continue;
                    }
                    State next_state(nextx, nexty, nexttime);
                    double g = node->g + 1;
                    double h = heuristic(next_state, goal_state);
                    Node *next_node = new Node(next_state, g, h, node);
                    if (close_set.count(next_state) == 0) {
                        finished_set.insert(next_node);
                        open_set.push(next_node);
                        close_set.insert(next_state);
                        if (next_node->f <= min_f * w)
                            focal_set.push(next_node);
                    } else {
                        delete next_node;
                    }
                }
                while (!open_set.empty() && open_set.top()->use)open_set.pop();
            }
            if (!flag) {
                return false;
            }
            //回溯路径
            Node *node = focal_set.top();
            while (node->parent != nullptr) {
                path.emplace_back(node->state.x, node->state.y);
                node = node->parent;
            }
            //反转路径
            std::reverse(path.begin(), path.end());
            //释放内存
            for (auto it: finished_set) {
                delete it;
            }
            return true;//找到路径
        }

    private:
        double w;//次优性范围
        int n;//地图大小
        std::vector<std::vector<char>> map_;//地图
        //节点
        struct Node {
            State state;//状态
            double g;//g值
            double h;//h值
            double f;//f值, f = g + h
            bool use;//是否使用
            Node *parent;//父节点
            //构造函数
            Node(State state, double g, double h, Node *p) : state(state), g(g), h(h), f(g + h), parent(p) {}

            //判断是否为目标状态
            bool isGoal(const pair<int, int> &goal) const {
                return state.x == goal.first && state.y == goal.second;
            }

            //重载<运算符，用于优先队列，f值小的优先级高
            bool operator<(const Node &other) const {
                return f > other.f;
            }

            //重载<运算符，用于优先队列，f值小的优先级高
            bool operator==(const Node &other) const {
                return state == other.state && g == other.g && h == other.h && f == other.f && parent == other.parent;
            }
        };

        //节点指针比较函数
        struct NodePtrComparator {
            bool operator()(const Node *a, const Node *b) const {
                return a->f > b->f;
            }
        };

        //焦点比较函数
        struct CompareNode {
            bool operator()(const Node *lhs, const Node *rhs) const {
                return lhs->f > rhs->f;
            }
        };

        //启发函数
        int heuristic(const State &a, const State &b) {
            return abs(a.x - b.x) + abs(a.y - b.y);
        }

        //状态
        const std::vector<std::pair<int, int>> directions_ = {{0,  1},
                                                              {1,  0},
                                                              {0,  -1},
                                                              {-1, 0},
                                                              {0,  0}};
    };

}  // namespace Robotlib
