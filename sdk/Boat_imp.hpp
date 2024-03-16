#pragma once

void Boat::go() {
    //log("第" + to_string(zhen) + "号船开向虚拟点");
    printf("go %d\n", id);
    if (berthid != -1) {
        //删除id
        for (auto it = berths[berthid].boatids.begin(); it != berths[berthid].boatids.end(); it++) {
            if (*it == id) {
                berths[berthid].boatids.erase(it);
                break;
            }
        }
    }
    num = 0;
    berthid = -1;
    status = 0;
}

void Boat::ship(int goal) {
    if (goal == -1)
        return;
    //log("第" + to_string(id) + "号船开向第" + to_string(goal) + "号泊位");
    printf("ship %d %d\n", id, goal);
    if (berthid != -1) {
        for (auto it = berths[berthid].boatids.begin(); it != berths[berthid].boatids.end(); it++) {
            if (*it == id) {
                berths[berthid].boatids.erase(it);
                break;
            }
        }
    }
    berths[goal].boatids.push_back(id);
    berthid = goal;
    status = 0;
}