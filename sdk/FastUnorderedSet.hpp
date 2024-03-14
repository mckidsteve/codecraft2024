#include <iostream>
#include <vector>
#include <functional>
#include <optional>

namespace std {
    template<typename T, typename Hash = std::hash<T>>
    class FastUnorderedSet {
    private:
        struct Bucket {
            std::optional<T> value;
            bool deleted;
            Bucket() : deleted(false) {}
        };

        std::vector<Bucket> buckets;
        size_t size_;
        Hash hash;

        size_t findBucket(const T& value) const {
            size_t index = hash(value) % buckets.size();
            size_t start_index = index;

            while (buckets[index].value && (buckets[index].value != value || buckets[index].deleted)) {
                index = (index + 1) % buckets.size();
                if (index == start_index) {
                    throw std::runtime_error("Hash table is full");
                }
            }

            return index;
        }

        void rehash() {
            std::vector<Bucket> old_buckets = std::move(buckets);
            buckets.resize(old_buckets.size() * 2);
            size_ = 0;

            for (auto& bucket : old_buckets) {
                if (bucket.value && !bucket.deleted) {
                    insert(*bucket.value);
                }
            }
        }

    public:
        FastUnorderedSet(size_t initial_capacity = 16) : buckets(initial_capacity), size_(0) {}

        bool insert(const T& value) {
            if (size_ >= buckets.size() * 0.7) {
                rehash();
            }

            size_t index = findBucket(value);
            if (buckets[index].value && !buckets[index].deleted) {
                return false;
            }

            buckets[index].value = value;
            buckets[index].deleted = false;
            size_++;
            return true;
        }

        bool contains(const T& value) const {
            size_t index = findBucket(value);
            return buckets[index].value && !buckets[index].deleted;
        }

        bool erase(const T& value) {
            size_t index = findBucket(value);
            if (buckets[index].value && !buckets[index].deleted) {
                buckets[index].deleted = true;
                size_--;
                return true;
            }
            return false;
        }

        size_t size() const {
            return size_;
        }

        // Iterator class
        class Iterator {
        private:
            const FastUnorderedSet& set;
            size_t index;

            void moveToNextValidBucket() {
                while (index < set.buckets.size() && (!set.buckets[index].value || set.buckets[index].deleted)) {
                    ++index;
                }
            }

        public:
            Iterator(const FastUnorderedSet& set, size_t index) : set(set), index(index) {
                moveToNextValidBucket();
            }

            const T& operator*() const {
                return *set.buckets[index].value;
            }

            Iterator& operator++() {
                ++index;
                moveToNextValidBucket();
                return *this;
            }

            bool operator!=(const Iterator& other) const {
                return index != other.index;
            }
        };

        // Begin and end methods for iterator
        Iterator begin() const {
            return Iterator(*this, 0);
        }

        Iterator end() const {
            return Iterator(*this, buckets.size());
        }
    };
}
