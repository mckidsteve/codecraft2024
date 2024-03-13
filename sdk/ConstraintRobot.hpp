#pragma once

#include "hashlib.hpp"

namespace Robotlib {
    class ConstraintRobot {
    public:
        ConstraintRobot(int x, int y, int time, int robot_id) : x(x), y(y), time(time), robot_id(robot_id) {}

        ConstraintRobot(std::pair<int, int> s, int time, int robot_id) : x(s.first), y(s.second), time(time),
                                                                         robot_id(robot_id) {}

        bool operator==(const ConstraintRobot &other) const {
            return x == other.x && y == other.y && time == other.time && robot_id == other.robot_id;
        }

        bool operator!=(const ConstraintRobot &other) const {
            return !(*this == other);
        }

        bool operator<(const ConstraintRobot &other) const {
            if (robot_id != other.robot_id) {
                return robot_id < other.robot_id;
            } else if (time != other.time) {
                return time < other.time;
            } else if (x != other.x) {
                return x < other.x;
            } else return y < other.y;
        }

        bool operator>(const ConstraintRobot &other) const {
            if (robot_id != other.robot_id) {
                return robot_id > other.robot_id;
            } else if (time != other.time) {
                return time > other.time;
            } else if (x != other.x) {
                return x > other.x;
            } else return y > other.y;
        }

        int x, y;
        int time;
        int robot_id;
    };
}

namespace std {
    template<>
    struct hash<Robotlib::ConstraintRobot> {
        std::size_t operator()(const Robotlib::ConstraintRobot &constraint) const {
            std::size_t seed = 0;
            hash_combine(seed, constraint.x);
            hash_combine(seed, constraint.y);
            hash_combine(seed, constraint.time);
            hash_combine(seed, constraint.robot_id);
            return seed;
        }
    };
}