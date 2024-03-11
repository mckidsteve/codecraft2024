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
            std::priority_queue<Node> open_set;
            std::unordered_set<State> close_set;
        }

    private:
        double w;//次优性范围
        int n;//地图大小
        std::vector<std::vector<char>> map_;//地图
        struct Node {
            State state;//状态
            double g;//g值
            double h;//h值
            double f;//f值, f = g + h

            //构造函数
            Node(State state, double g, double h) : state(state), g(g), h(h), f(g + h) {}

            //重载<运算符
            bool operator<(const Node &other) const {
                return f > other.f;
            }
        };

        //状态
        const std::vector<std::pair<int, int>> directions_ = {{0,  1},
                                                              {1,  0},
                                                              {0,  -1},
                                                              {-1, 0},
                                                              {0,  0}};
    };

}  // namespace Robotlib
