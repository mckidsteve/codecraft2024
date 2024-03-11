#pragma once

#include "hashlib.hpp"

namespace Robotlib {
    class State {
    public:
        State(int x, int y, int time) : x(x), y(y), time(time) {}

        bool operator==(const State &other) const {
            return x == other.x && y == other.y && time == other.time;
        }

        bool operator!=(const State &other) const {
            return !(*this == other);
        }

        bool operator<(const State &other) const {
            if (time != other.time) {
                return time < other.time;
            } else if (x != other.x) {
                return x < other.x;
            } else return y < other.y;
        }

        bool operator>(const State &other) const {
            if (time != other.time) {
                return time > other.time;
            } else if (x != other.x) {
                return x > other.x;
            } else return y > other.y;
        }

        int x;
        int y;
        int time;
    };
}
namespace std {
    template<>
    struct hash<Robotlib::State> {
        std::size_t operator()(const Robotlib::State &state) const {
            std::size_t seed = 0;
            hash_combine(seed, state.x);
            hash_combine(seed, state.y);
            hash_combine(seed, state.time);
            return seed;
        }
    };
}