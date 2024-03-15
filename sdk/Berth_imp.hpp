#pragma once

void Berth::stowage() {
    if (boatids.empty())
        return;
    int boatid = boatids.front();
    if (boats[boatid].status == 0)
        return;
    if (things.empty())
        return;
    // log("第" + to_string(zhen) + "号泊位向第" + to_string(boatid) + "号船装载货物");
    for (int i = 0; i < loading_speed; i++) {
        if (boats[boatid].num < boat_capacity && !things.empty()) {
            // pair<int, int> a = things.front();
            things.pop();
            boats[boatid].num++;
        } else
            return;
    }
}

double Berth::transport_time_value() {
    return 0;
//    return transport_time * 1.0 * (boat_capacity - ((int) things.size() % boat_capacity)) / boat_capacity;
//    return transport_time;
}
