#pragma once

vector<pair<int, int>> Astar(int x, int y, int x1, int y1) {
    vector<pair<int, int>> road;
    int dir[4][2] = {{0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0}};
    int dis[n][n];
    memset(dis, -1, sizeof(dis));
    // 优先队列
    priority_queue<pair<int, pair<int, pair<int, int>>>> q;
    q.emplace(0, make_pair(0, make_pair(x, y)));
    dis[x][y] = 0;
    int flag = 0;
    // A*算法
    while (!q.empty()) {
        int nowx = q.top().second.second.first;
        int nowy = q.top().second.second.second;
        int nowg = q.top().second.first;
        q.pop();
        if (nowx == x1 && nowy == y1) {
            flag = 1;
            break;
        }
        for (int i = 0; i < 4; i++) {
            int nextx = nowx + dir[i][0];
            int nexty = nowy + dir[i][1];
            if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || dis[nextx][nexty] != -1 ||
                game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                continue;
            // 标记已经访问
            dis[nextx][nexty] = nowg + 1;
            // 估价函数
            int nextg = nowg + 1;
            // 曼哈顿距离
            int h = abs(nextx - x1) + abs(nexty - y1);
            // 加入优先队列
            q.emplace(-(nextg + h), make_pair(nextg, make_pair(nextx, nexty)));
        }
    }
    if (flag == 0)
        return road;
    int nowx = x1, nowy = y1;
    // 获取路径
    while (nowx != x || nowy != y) {
        road.emplace_back(nowx, nowy);
        for (int i = 0; i < 4; i++) {
            int nextx = nowx + dir[i][0];
            int nexty = nowy + dir[i][1];
            if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || dis[nextx][nexty] == -1 ||
                game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                continue;
            if (dis[nextx][nexty] == dis[nowx][nowy] - 1) {
                nowx = nextx;
                nowy = nexty;
                break;
            }
        }
    }
    // 翻转路径
    reverse(road.begin(), road.end());
    return road;
}

vector<pair<int, int>> Astar(int x, int y, int x1, int y1, int berthid) {
    vector<pair<int, int>> road;
    int dir[4][2] = {{0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0}};
    int dis[n][n];
    memset(dis, -1, sizeof(dis));
    // 优先队列
    priority_queue<pair<int, pair<int, pair<int, int>>>> q;
    q.emplace(0, make_pair(0, make_pair(x, y)));
    dis[x][y] = 0;
    int flag = 0;
    int fx, fy; // 终点坐标
    // A*算法
    while (!q.empty()) {
        int nowx = q.top().second.second.first;
        int nowy = q.top().second.second.second;
        int nowg = q.top().second.first;
        q.pop();
        if (nowx == x1 && nowy == y1 || game_map[nowx][nowy] == '0' + berthid) {
            flag = 1;
            fx = nowx;
            fy = nowy;
            break;
        }
        for (int i = 0; i < 4; i++) {
            int nextx = nowx + dir[i][0];
            int nexty = nowy + dir[i][1];
            if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || dis[nextx][nexty] != -1 ||
                game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                continue;
            // 标记已经访问
            dis[nextx][nexty] = nowg + 1;
            // 估价函数
            int nextg = nowg + 1;
            // 曼哈顿距离
            int h = berth_dis[nextx][nexty][berthid];
            // 加入优先队列
            q.emplace(-(nextg + h), make_pair(nextg, make_pair(nextx, nexty)));
        }
    }
    if (flag == 0)
        return road;
    int nowx = fx, nowy = fy;
    // 获取路径
    while (nowx != x || nowy != y) {
        road.emplace_back(nowx, nowy);
        for (int i = 0; i < 4; i++) {
            int nextx = nowx + dir[i][0];
            int nexty = nowy + dir[i][1];
            if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || dis[nextx][nexty] == -1 ||
                game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                continue;
            if (dis[nextx][nexty] == dis[nowx][nowy] - 1) {
                nowx = nextx;
                nowy = nexty;
                break;
            }
        }
    }
    // 翻转路径
    reverse(road.begin(), road.end());
    return road;
}
