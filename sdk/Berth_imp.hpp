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
            boats[boatid].val += things.begin()->val;
            boats[boatid].num++;
            things.erase(things.begin());
        } else
            return;
    }
}

double Berth::transport_time_value() const {
//    int z = 0;
//    for (int boatid: boatids) {
//        z += (boat_capacity - boats[boatid].num);
//    }
//    double empty = (boat_capacity - (max((int) things.size() - z, 0) % boat_capacity)) * 1.0 / boat_capacity;
//    return 5 * empty * transport_time / boat_capacity;
//    return (1 * (boat_num - free_boats) / boat_num + 1) * (boat_capacity - ((int) things.size() % boat_capacity)) /
//           boat_capacity * transport_time / boat_capacity;
    //return cargo_dis != 0 ? 10 - carge_value * 1.0 / cargo_dis : 0;
    return 0;
//    return transport_time * 1.0 * (boat_capacity - ((int) things.size() % boat_capacity)) / boat_capacity;
//    return transport_time * 1.0 / boat_capacity;
}
