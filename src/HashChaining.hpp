#ifndef HASH_CHAINING_HPP
#define HASH_CHAINING_HPP

#include "Dictionary.hpp"

#include <vector>
#include <list>
#include <utility>
#include <functional>
#include <string>
#include <cstddef>

template <typename Key, typename Value>
class HashChaining : public Dictionary<Key, Value> {
private:
    struct Entry {
        Key key;
        Value value;

        Entry(const Key& key, const Value& value)
            : key(key), value(value) {}
    };

    std::vector<std::list<Entry>> table;
    std::size_t elementCount;
    double maxLoadFactor;
    mutable DictionaryMetrics metricsData;

    std::size_t hashIndex(const Key& key) const {
        return std::hash<Key>{}(key) % table.size();
    }

    double currentLoadFactor() const {
        if (table.empty()) {
            return 0.0;
        }

        return static_cast<double>(elementCount) / table.size();
    }

    void rehash(std::size_t newCapacity) {
        std::vector<std::list<Entry>> oldTable = std::move(table);

        table.clear();
        table.resize(newCapacity);
        elementCount = 0;

        for (const auto& bucket : oldTable) {
            for (const auto& entry : bucket) {
                std::size_t index = hashIndex(entry.key);
                table[index].push_back(entry);
                elementCount++;
            }
        }

        metricsData.rehashes++;
    }

    void checkLoadFactor() {
        if (currentLoadFactor() > maxLoadFactor) {
            rehash(table.size() * 2 + 1);
        }
    }

public:
    explicit HashChaining(std::size_t initialCapacity = 101)
        : table(initialCapacity),
          elementCount(0),
          maxLoadFactor(0.75),
          metricsData() {}

    ~HashChaining() override = default;

    bool insert(const Key& key, const Value& value) override {
        std::size_t index = hashIndex(key);
        auto& bucket = table[index];

        for (auto& entry : bucket) {
            metricsData.keyComparisons++;

            if (entry.key == key) {
                return false;
            }
        }

        if (!bucket.empty()) {
            metricsData.collisions++;
        }

        bucket.emplace_back(key, value);
        elementCount++;

        checkLoadFactor();

        return true;
    }

    bool update(const Key& key, const Value& value) override {
        std::size_t index = hashIndex(key);
        auto& bucket = table[index];

        for (auto& entry : bucket) {
            metricsData.keyComparisons++;

            if (entry.key == key) {
                entry.value = value;
                return true;
            }
        }

        return false;
    }

    Value* get(const Key& key) override {
        std::size_t index = hashIndex(key);
        auto& bucket = table[index];

        for (auto& entry : bucket) {
            metricsData.keyComparisons++;

            if (entry.key == key) {
                return &entry.value;
            }
        }

        return nullptr;
    }

    const Value* get(const Key& key) const override {
        std::size_t index = hashIndex(key);
        const auto& bucket = table[index];

        for (const auto& entry : bucket) {
            metricsData.keyComparisons++;

            if (entry.key == key) {
                return &entry.value;
            }
        }

        return nullptr;
    }

    bool contains(const Key& key) override {
        return get(key) != nullptr;
    }

    bool remove(const Key& key) override {
        std::size_t index = hashIndex(key);
        auto& bucket = table[index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            metricsData.keyComparisons++;

            if (it->key == key) {
                bucket.erase(it);
                elementCount--;
                return true;
            }
        }

        return false;
    }

    void clear() override {
        for (auto& bucket : table) {
            bucket.clear();
        }

        elementCount = 0;
    }

    std::size_t size() const override {
        return elementCount;
    }

    std::vector<std::pair<Key, Value>> items() const override {
        std::vector<std::pair<Key, Value>> result;
        result.reserve(elementCount);

        for (const auto& bucket : table) {
            for (const auto& entry : bucket) {
                result.emplace_back(entry.key, entry.value);
            }
        }

        return result;
    }

    std::string name() const override {
        return "Hash Chaining";
    }

    DictionaryMetrics metrics() const override {
        DictionaryMetrics copy = metricsData;
        copy.loadFactor = currentLoadFactor();
        return copy;
    }

    void resetMetrics() override {
        metricsData = DictionaryMetrics();
    }
};

#endif