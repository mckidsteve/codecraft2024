#pragma once

#include<vector>

namespace Robotlib {
    struct Path {
        std::vector<std::pair<int, int>> road;
        double min_f{};

        void clear() {
            road.clear();
        }
    };
}