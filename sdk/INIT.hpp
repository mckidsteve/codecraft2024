#pragma once

void Init() {
    for (int i = 0; i < N; i++) {
        char s[N];
        scanf("%s", s);
        for (int j = 0; j < N; j++) {
            game_map[i][j] = s[j];
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (game_map[i][j] == 'A')game_map[i][j] = '.';
        }
    }
    for (int i = 0; i < ROBOT_NUM; i++) {
        robots[i].id = i;
    }
    for (int i = 0; i < BOAT_NUM; i++) {
        boats[i].id = i;
    }
    for (int i = 0; i < BERTH_NUM; i++) {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berths[id].x, &berths[id].y, &berths[id].transport_time, &berths[id].loading_speed);
        log(to_string(id) + " " + to_string(berths[id].x) + " " + to_string(berths[id].y) + " " +
            to_string(berths[id].transport_time) + " " + to_string(berths[id].loading_speed));
        berths[id].id = id;
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                game_map[berths[id].x + j][berths[id].y + k] = '0' + i;
            }
        }
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    //初始化所有泊位到所有位置的距离
    int dir[4][2] = {{0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0}};
    for (int i = 0; i < BERTH_NUM; i++) {
        queue<pair<int, pair<int, int>>> q;
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                q.emplace(0, make_pair(berths[i].x + j, berths[i].y + k));
                berth_dis[berths[i].x + j][berths[i].y + k][i] = 0;
            }
        }
        while (!q.empty()) {
            int nowx = q.front().second.first;
            int nowy = q.front().second.second;
            int nowg = q.front().first;
            q.pop();
            for (int j = 0; j < 4; j++) {
                int nextx = nowx + dir[j][0];
                int nexty = nowy + dir[j][1];
                if (nextx < 0 || nextx >= N || nexty < 0 || nexty >= N || berth_dis[nextx][nexty][i] != -1 ||
                    game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                    continue;
                // 标记已经访问
                berth_dis[nextx][nexty][i] = nowg + 1;
                // 加入优先队列
                q.emplace(nowg + 1, make_pair(nextx, nexty));
            }
        }
    }
    int sum = 0, f = 0;
    bool vis[N][N];
    memset(vis, false, sizeof(vis));
    for (auto &s: game_map) {
        for (char c: s) {
            if (c == '#' || c == '*')continue;
            sum++;
        }
    }
    sum = sum / random_bfs_point;
    for (int ii = 0; ii < N; ii++) {
        if (random_point.size() >= random_bfs_point)break;
        for (int jj = 0; jj < N; jj++) {
            if (random_point.size() >= random_bfs_point)break;
            if (game_map[ii][jj] == '#' || game_map[ii][jj] == '*' || vis[ii][jj])continue;
            queue<pair<int, int>> q;
            q.emplace(ii, jj);
            vis[ii][jj] = true;
            while (!q.empty()) {
                int nowx = q.front().first;
                int nowy = q.front().second;
                q.pop();
                f++;
                if (f == sum) {
                    f = 0;
                    random_point.emplace_back(nowx, nowy);
                }
                for (int j = 0; j < 4; j++) {
                    int nextx = nowx + dir[j][0];
                    int nexty = nowy + dir[j][1];
                    if (nextx < 0 || nextx >= N || nexty < 0 || nexty >= N || vis[nextx][nexty] ||
                        game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                        continue;
                    //标记已经访问
                    vis[nextx][nexty] = true;
                    //加入优先队列
                    q.emplace(nextx, nexty);
                }
            }
        }
    }
    int size = random_point.size();
    log("初始化随机点的个数:" + to_string(random_point.size()));
    for (int i = 0; i < size; i++) {
        pair<int, int> st = random_point[i];
        queue<pair<int, pair<int, int>>> q;
        q.emplace(0, make_pair(st.first, st.second));
        random_dis[st.first][st.second][i] = 0;
        while (!q.empty()) {
            int nowx = q.front().second.first;
            int nowy = q.front().second.second;
            int nowg = q.front().first;
            q.pop();
            for (int j = 0; j < 4; j++) {
                int nextx = nowx + dir[j][0];
                int nexty = nowy + dir[j][1];
                if (nextx < 0 || nextx >= N || nexty < 0 || nexty >= N || random_dis[nextx][nexty][i] != -1 ||
                    game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                    continue;
                // 标记已经访问
                random_dis[nextx][nexty][i] = nowg + 1;
                // 加入优先队列
                q.emplace(nowg + 1, make_pair(nextx, nexty));
            }
        }
    }
    ecbs = new ECBS(game_map, 1.5, berth_dis, random_dis);
    for (int i = 0; i < ROBOT_NUM; i++) {
        ecbs->robots[i] = &robots[i];
    }
    //输出地图情况
#ifdef _WIN32
    ofstream mapfile;
//    mapfile.open("finish_map.txt", ios::out | ios::trunc);
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++) {
//            mapfile << game_map[i][j];
//        }
//        mapfile << endl;
//    }
//    mapfile.close();
//    for (int k = 0; k < berth_num; k++) {
//        mapfile.open("berth_map_" + to_string(k) + ".txt", ios::out | ios::trunc);
//        for (int i = 0; i < n; i++) {
//            for (int j = 0; j < n; j++) {
//                if (berth_dis[i][j][k] == -1)mapfile << "#";
//                else if (berth_dis[i][j][k] == 0)mapfile << "0";
//                else mapfile << ".";
//            }
//            mapfile << endl;
//        }
//        mapfile.close();
//    }
//    unsigned long long int size1 = random_point.size();
//    for (int k = 0; k < size1; k++) {
//        mapfile.open("./random_point/random_map_" + to_string(k) + ".txt", ios::out | ios::trunc);
//        for (int i = 0; i < n; i++) {
//            for (int j = 0; j < n; j++) {
//                if (random_dis[i][j][k] == -1)mapfile << "#";
//                else if (random_dis[i][j][k] == 0 && random_point[k].first == i && random_point[k].second == j)
//                    mapfile << "0";
//                else mapfile << ".";
//            }
//            mapfile << endl;
//        }
//        mapfile.close();
//    }
#endif
    log("轮船运载能力" + to_string(boat_capacity));
    printf("OK\n");
    fflush(stdout);
}