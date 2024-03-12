#pragma once
#include <vector>

using namespace std;
vector<vector<char>> convertToVector(int map_size, char map[n][n]) {
    std::vector<std::vector<char>> m(map_size, std::vector<char>(map_size));
    for (int i = 0; i < map_size; ++i) {
        for (int j = 0; j < map_size; ++j) {
            m[i][j] = map[i][j];
        }
    }
    return m;
}
