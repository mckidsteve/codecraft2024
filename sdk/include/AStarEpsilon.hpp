#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include "State.hpp"

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
            std::priority_queue<Node> open_set;
            std::unordered_set<State> close_set;
            std::priority_queue<Node, vector<Node>, CompareNode> focal_set;
            focal_set.emplace(State(start, 0), 0, 0, nullptr);
            double min_f = 0;
            bool flag = false;
            while (!focal_set.empty()) {
                Node node = focal_set.top();
                if(node.isGoal(goal)){
                    flag = true;
                    break;
                }
                focal_set.pop();
                if (node.f > min_f) {
                    min_f = node.f;
                    while (!open_set.empty()) {
                        if (open_set.top().f > min_f * w) {
                            break;
                        }
                        focal_set.push(open_set.top());
                        open_set.pop();
                    }
                }
                for (int i = 0; i < 4; i++){
                    int nextx = node.state.x + directions_[i].first;
                    int nexty = node.state.y + directions_[i].second;
                    if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n ||//越界
                    map_[nextx][nexty] == '#' ||map_[nextx][nexty]=='*'||//障碍物
                    obstacles.count(State(nextx, nexty, time)) != 0){//时间约束障碍物
                        continue;
                    }
                    State next_state(nextx, nexty, time + 1);
                    double g = node.g + 1;
                    double h = heuristic(next_state, State(goal.first, goal.second, time));
                    Node next_node(next_state, g, h, &node);
                    if (close_set.count(next_state) == 0){
                        open_set.push(next_node);
                        close_set.insert(next_state);
                    }
                }
            }
            if(!flag){
                return false;
            }
            //回溯路径
            Node node = focal_set.top();
            while (node.parent != nullptr) {
                path.emplace_back(node.state.x, node.state.y);
                node = *node.parent;
            }
            //反转路径
            std::reverse(path.begin(), path.end());
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
            Node* parent;//父节点
            //构造函数
            Node(State state, double g, double h,Node *p) : state(state), g(g), h(h), f(g + h),parent(p) {}

            //判断是否为目标状态
            bool isGoal(const pair<int,int> &goal) const {
                return state.x == goal.first&& state.y == goal.second;
            }
            //重载<运算符，用于优先队列，f值小的优先级高
            bool operator<(const Node &other) const {
                return f > other.f;
            }
        };

        //比较函数
        struct CompareNode {
            bool operator()(const Node &lhs, const Node &rhs) const {
                return lhs.f > rhs.f;
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
