#pragma once

#include<vector>

namespace Robotlib {
    struct Path {
        std::vector<std::pair<int, int>> road;

        void clear() {
            road.clear();
        }
    };
}