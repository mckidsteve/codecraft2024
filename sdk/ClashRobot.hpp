#pragma once

#include "hashlib.hpp"

namespace Robotlib {
    class ClashRobot {
    public:
        ClashRobot(int x, int y, int time, int robot_id_1, int robot_id_2) : x(x), y(y), time(time),
                                                                             robot_id_1(robot_id_1),
                                                                             robot_id_2(robot_id_2), dian(true) {}

        ClashRobot(std::pair<int, int> s, int time, int robot_id_1, int robot_id_2) : x(s.first), y(s.second),
                                                                                      time(time),
                                                                                      robot_id_1(robot_id_1),
                                                                                      robot_id_2(robot_id_2),
                                                                                      dian(true) {}

        ClashRobot(std::pair<int, int> s1, std::pair<int, int> s2, int time, int robot_id_1, int robot_id_2) :
                x(s1.first), y(s1.second),
                x2(s2.first), y2(s2.second),
                time(time),
                robot_id_1(robot_id_1),
                robot_id_2(robot_id_2),
                dian(false) {}

        ClashRobot(int x, int y, int x2, int y2, int time, int robot_id_1, int robot_id_2) :
                x(x), y(y),
                x2(x2), y2(y2),
                time(time),
                robot_id_1(robot_id_1),
                robot_id_2(robot_id_2),
                dian(false) {}

        bool operator==(const ClashRobot &other) const {
            return x == other.x && y == other.y && time == other.time && robot_id_1 == other.robot_id_1 &&
                   robot_id_2 == other.robot_id_2;
        }

        bool operator!=(const ClashRobot &other) const {
            return !(*this == other);
        }

        bool operator<(const ClashRobot &other) const {
            if (robot_id_1 != other.robot_id_1) {
                return robot_id_1 < other.robot_id_1;
            } else if (robot_id_2 != other.robot_id_2) {
                return robot_id_2 < other.robot_id_2;
            } else if (time != other.time) {
                return time < other.time;
            } else if (x != other.x) {
                return x < other.x;
            } else return y < other.y;
        }

        bool operator>(const ClashRobot &other) const {
            if (robot_id_1 != other.robot_id_1) {
                return robot_id_1 > other.robot_id_1;
            } else if (robot_id_2 != other.robot_id_2) {
                return robot_id_2 > other.robot_id_2;
            } else if (time != other.time) {
                return time > other.time;
            } else if (x != other.x) {
                return x > other.x;
            } else return y > other.y;
        }

        int x, y, x2{}, y2{};
        int time;
        int robot_id_1, robot_id_2;
        bool dian;
    };
}

namespace std {
    template<>
    struct hash<Robotlib::ClashRobot> {
        std::size_t operator()(const Robotlib::ClashRobot &constraint) const {
            std::size_t seed = 0;
            hash_combine(seed, constraint.x);
            hash_combine(seed, constraint.y);
            hash_combine(seed, constraint.time);
            hash_combine(seed, constraint.robot_id_1);
            hash_combine(seed, constraint.robot_id_2);
            return seed;
        }
    };
}