#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include "log.hpp"

namespace std {
    template<typename T, typename Compare = less<T>>
    class PriorityQueue {
    public:
        void push(const T &value) {
            data.push_back(value);
            push_heap(data.begin(), data.end(), comp);
        }

        void pop() {
            pop_heap(data.begin(), data.end(), comp);
            data.pop_back();
        }

        const T &top() const {
            return data.front();
        }

        bool empty() const {
            return data.empty();
        }

        size_t size() const {
            return data.size();
        }

        bool remove(const T &x) {
            auto it = std::find(data.begin(), data.end(), x);
            if (it == data.end()) {
                return false; // 未找到元素
            }

            // 将要删除的元素与最后一个元素交换
            std::iter_swap(it, data.end() - 1);
            data.pop_back(); // 移除最后一个元素

            // 对交换后的元素进行调整
            std::make_heap(data.begin(), data.end(), comp);

            return true;
        }

        // 提供遍历功能
        typename vector<T>::iterator begin() {
            return data.begin();
        }

        typename vector<T>::iterator end() {
            return data.end();
        }

    private:
        vector<T> data;
        Compare comp;
    };
}