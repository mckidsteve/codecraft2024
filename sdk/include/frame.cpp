
// 初始化
void Init() {
    for (int i = 0; i < n; i++)
        scanf("%s", game_map[i]);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (game_map[i][j] == 'A')game_map[i][j] = '.';
        }
    }
    for (int i = 0; i < robot_num; i++) {
        robots[i].id = i;
    }
    for (int i = 0; i < boat_num; i++) {
        boats[i].id = i;
    }
    for (int i = 0; i < berth_num; i++) {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berths[id].x, &berths[id].y, &berths[id].transport_time, &berths[id].loading_speed);
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
    memset(berth_dis, -1, sizeof(berth_dis));
    int dir[4][2] = { {0,  1},
                     {0,  -1},
                     {1,  0},
                     {-1, 0} };
    for (int i = 0; i < berth_num; i++) {
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
                if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || berth_dis[nextx][nexty][i] != -1 ||
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
    bool vis[n][n];
    memset(vis, false, sizeof(vis));
    for (auto& s : game_map) {
        for (char c : s) {
            if (c == '#' || c == '*')continue;
            sum++;
        }
    }
    sum = sum / random_bfs_point;
    for (int ii = 0; ii < n; ii++) {
        if (random_point.size() >= random_bfs_point)break;
        for (int jj = 0; jj < n; jj++) {
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
                    if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || vis[nextx][nexty] ||
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
    memset(random_dis, -1, sizeof(random_dis));
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
                if (nextx < 0 || nextx >= n || nexty < 0 || nexty >= n || random_dis[nextx][nexty][i] != -1 ||
                    game_map[nextx][nexty] == '#' || game_map[nextx][nexty] == '*')
                    continue;
                // 标记已经访问
                random_dis[nextx][nexty][i] = nowg + 1;
                // 加入优先队列
                q.emplace(nowg + 1, make_pair(nextx, nexty));
            }
        }
    }
    starEpsilon = new AStarEpsilon(convertToVector(n, game_map), 1.5);
    printf("OK\n");
    fflush(stdout);
}
// 每帧的输入
void PerframeInput() {
    scanf("%d%d", &id, &money);
    log("第" + to_string(id) + "帧");
    int num;
    scanf("%d", &num);
    for (int i = 1; i <= num; i++) {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        new_cargos.emplace_back(x, y, val, id);
    }
    for (int i = 0; i < robot_num; i++) {
        scanf("%d%d%d%d", &robots[i].goods, &robots[i].x, &robots[i].y, &robots[i].status);
    }
    for (int i = 0; i < 5; i++)
        scanf("%d%d\n", &boats[i].status, &boats[i].berthid);
    char okk[100];
    scanf("%s", okk);
}

// 每帧的更新
void PerframeUpdate() {
    queue<Cargo> c;
    // 当大于1000帧的时候物品消失
    for (int i = 0; i < cargos.size(); i++) {
        Cargo cargo = cargos.front();
        cargos.pop();
        if (cargo.time - id > 1000)
            continue;
        c.push(cargo);
    }
    // 每帧处理一个新货物
    bool have_robot = false;
    for (int i = 0; i < robot_num; i++) {
        if (robots[i].status == 0)continue;
        if (robots[i].goods == 0) {
            have_robot = true;
            break;
        }
    }
    while (have_robot && !new_cargos.empty()) {
        Cargo cargo = new_cargos.front();
        new_cargos.pop_front();
        int berth_id = CargotoBerth(cargo);
        if (berth_id == -1)
            continue;
        double berth_time = berth_dis[cargo.x][cargo.y][berth_id] + berths[berth_id].transport_time_value();
        double max_value = 0.0;
        int robot_id = -1;
        //找到最优的机器人
        for (int i = 0; i < robot_num; i++) {
            if (robots[i].status == 0)continue;
            //如果机器人携带货物就跳过
            if (robots[i].goods == 1)continue;
            //如果机器人到不了对应的泊位
            if (berth_dis[robots[i].x][robots[i].y][berth_id] == -1)continue;
            int robot_time = CargotoRobot(cargo, robots[i]);
            if (robot_time == MAXNUM)
                continue;
            double value = cargo.val * 1.0 / (berth_time + robot_time);
            double robot_value =
                robots[i].cargo.val * 1.0 /
                max(1.0, CargotoRobot(robots[i].cargo, robots[i]) + robots[i].cargotoberth);
            //            double robot_value =
            //                    robots[i].cargo.val * 1.0 /
            //                    max(1.0, (int) robots[i].road.size() + robots[i].cargotoberth);
            if (robot_value > value)
                continue;
            if (value - robot_value > max_value) {
                max_value = value - robot_value;
                robot_id = i;
            }
        }
        // 如果没有找到机器人就把货物重新放入队列
        if (robot_id == -1) {
            cargos.push(cargo);
            continue;
        }
        vector<pair<int, int>> road = getRoadtoCargo(robots[robot_id].x, robots[robot_id].y, cargo.x, cargo.y);
        // 如果没有找到路径就把货物重新放入队列
        if (road.empty()) {
            cargos.push(cargo);
            continue;
        }
        // 如果机器人在运输其他货物，就先把货物重新放入队列
        if (robots[robot_id].cargo.val != 0) {
            new_cargos.push_front(robots[robot_id].cargo);
        }
        robots[robot_id].setGoal(cargo, berth_time, berth_id, road);
        break;
    }
    //没有任务目标的机器人获得新的任务目标,有目标但是失败的机器人重新获取新的泊位
    for (int i = 0; i < robot_num; i++) {
        if (robots[i].status == 0)continue;
        if (robots[i].goods == 1 &&
            robots[i].road.empty()) {
            int berth_id = RobottoBerth(robots[i]);
            robots[i].road = getRoadtoBerth(robots[i].x, robots[i].y, berths[berth_id].x,
                berths[berth_id].y, berth_id);
        }
        if (robots[i].cargo.val == 0 && robots[i].goods == 0) {
            RobotFindNewGoal(cargos, robots[i]);
        }
        if (robots[i].cargo.val != 0 && (robots[i].road.size() + id - robots[i].cargo.time > 1000)) {
            robots[i].Reset(false);
        }
    }
}

// 每帧的输出
void PerframeOutput() {
    //    log("第" + to_string(id) + "帧输出");
    //    for (int i = 0; i < berth_num; i++) {
    //        if (berths[i].things.empty())continue;
    //        log("第" + to_string(i) + "号泊位");
    //        log("物品数量:" + to_string(berths[i].things.size()));
    //    }
    for (int i = 0; i < robot_num; i++) {
        // 如果机器人路径不为空就继续走
        if (!robots[i].road.empty()) {
            pair<int, int> next = robots[i].road.front();
            robots[i].road.erase(robots[i].road.begin());
            int dis = abs(robots[i].x - next.first) + abs(robots[i].y - next.second);
            if (dis > 1) {
                robots[i].Reset(false);
            }
            else {
                // 机器人移动
                robots[i].move(next.first, next.second);
            }
            if (robots[i].road.empty()) {
                if (robots[i].goods == 0) {
                    // 机器人没有货物,就去拿货物
                    robots[i].getThings(next.first, next.second);
                }
                else {
                    // 机器人有货物,就去放货物
                    robots[i].putThings(next.first, next.second);
                }
            }
        }
        else if (robots[i].goods == 1) { // 机器人正好在港口上的时候
            robots[i].putThings(robots[i].x, robots[i].y);
        }
    }
    for (int i = 0; i < boat_num; i++) {
        if (boats[i].status == 0)
            continue;
        if (boats[i].num == boat_capacity || // 当装满的时候直接出发开始运输
            boats[i].berthid != -1 && berths[boats[i].berthid].transport_time + id > 14950 &&
            boats[i].num != 0) { // 当时间快到的时候也直接开始运输
            boats[i].go();
            continue;
        }
        else if (boats[i].berthid == -1) {
            int max = 0, goal = -1;
            for (int j = 0; j < berth_num; j++) {
                if (berths[j].boatid != -1)
                    continue;
                int z = berths[j].things.size();
                if (z > max) {
                    max = z;
                    goal = j;
                }
            }
            if (goal == -1) {
                for (int j = 0; j < berth_num; j++) {
                    if (berths[j].boatid == -1) {
                        goal = j;
                        break;
                    }
                }
            }
            boats[i].ship(goal);
            continue;
        }
        else if (!berths[boats[i].berthid].things.empty()) {
            continue;
        }
        else {
            int max = 0, goal = -1;
            for (int j = 0; j < berth_num; j++) {
                if (berths[j].boatid != -1)
                    continue;
                if (id + 500 + berths[j].transport_time > 14950)
                    continue;
                int z = berths[j].things.size();
                if (z > max) {
                    max = z;
                    goal = j;
                }
            }
            boats[i].ship(goal);
        }
    }
    for (int i = 0; i < berth_num; i++) {
        berths[i].stowage();
    }
    puts("OK");
    fflush(stdout);
}
