#ifndef HASH_OPEN_ADDRESSING_HPP
#define HASH_OPEN_ADDRESSING_HPP

#include "Dictionary.hpp"

#include <vector>
#include <utility>
#include <functional>
#include <string>
#include <cstddef>

template <typename Key, typename Value>
class HashOpenAddressing : public Dictionary<Key, Value> {
private:
    enum class State {
        EMPTY,
        OCCUPIED,
        DELETED
    };

    struct Entry {
        Key key;
        Value value;
        State state;

        Entry() : key(), value(), state(State::EMPTY) {}
    };

    std::vector<Entry> table;
    std::size_t elementCount;
    std::size_t deletedCount;
    std::size_t initialCapacity;
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

    double usedLoadFactorAfterOneMore() const {
        if (table.empty()) {
            return 1.0;
        }

        return static_cast<double>(elementCount + deletedCount + 1) / table.size();
    }

    void placeWithoutMetrics(const Key& key, const Value& value) {
        std::size_t index = hashIndex(key);

        for (std::size_t i = 0; i < table.size(); ++i) {
            std::size_t current = (index + i) % table.size();

            if (table[current].state != State::OCCUPIED) {
                table[current].key = key;
                table[current].value = value;
                table[current].state = State::OCCUPIED;
                elementCount++;
                return;
            }
        }
    }

    void rehash(std::size_t newCapacity) {
        std::vector<Entry> oldTable = table;

        table.clear();
        table.resize(newCapacity);

        elementCount = 0;
        deletedCount = 0;

        for (const auto& entry : oldTable) {
            if (entry.state == State::OCCUPIED) {
                placeWithoutMetrics(entry.key, entry.value);
            }
        }

        metricsData.rehashes++;
    }

    void ensureCapacity() {
        if (table.empty()) {
            table.resize(initialCapacity);
        }

        if (usedLoadFactorAfterOneMore() > maxLoadFactor) {
            rehash(table.size() * 2 + 1);
        }
    }

public:
    explicit HashOpenAddressing(std::size_t capacity = 101)
        : table(capacity < 8 ? 8 : capacity),
          elementCount(0),
          deletedCount(0),
          initialCapacity(capacity < 8 ? 8 : capacity),
          maxLoadFactor(0.60),
          metricsData() {}

    ~HashOpenAddressing() override = default;

    bool insert(const Key& key, const Value& value) override {
        ensureCapacity();

        std::size_t index = hashIndex(key);

        bool hasDeletedIndex = false;
        std::size_t firstDeletedIndex = 0;
        bool collisionCounted = false;

        for (std::size_t i = 0; i < table.size(); ++i) {
            std::size_t current = (index + i) % table.size();
            metricsData.probes++;

            if (table[current].state == State::OCCUPIED) {
                metricsData.keyComparisons++;

                if (table[current].key == key) {
                    return false;
                }

                if (!collisionCounted) {
                    metricsData.collisions++;
                    collisionCounted = true;
                }
            } else if (table[current].state == State::DELETED) {
                if (!hasDeletedIndex) {
                    hasDeletedIndex = true;
                    firstDeletedIndex = current;
                }
            } else {
                std::size_t target = hasDeletedIndex ? firstDeletedIndex : current;

                if (table[target].state == State::DELETED) {
                    deletedCount--;
                }

                table[target].key = key;
                table[target].value = value;
                table[target].state = State::OCCUPIED;
                elementCount++;

                return true;
            }
        }

        if (hasDeletedIndex) {
            table[firstDeletedIndex].key = key;
            table[firstDeletedIndex].value = value;
            table[firstDeletedIndex].state = State::OCCUPIED;

            elementCount++;
            deletedCount--;

            return true;
        }

        rehash(table.size() * 2 + 1);
        return insert(key, value);
    }

    bool update(const Key& key, const Value& value) override {
        std::size_t index = hashIndex(key);

        for (std::size_t i = 0; i < table.size(); ++i) {
            std::size_t current = (index + i) % table.size();
            metricsData.probes++;

            if (table[current].state == State::EMPTY) {
                return false;
            }

            if (table[current].state == State::OCCUPIED) {
                metricsData.keyComparisons++;

                if (table[current].key == key) {
                    table[current].value = value;
                    return true;
                }
            }
        }

        return false;
    }

    Value* get(const Key& key) override {
        std::size_t index = hashIndex(key);

        for (std::size_t i = 0; i < table.size(); ++i) {
            std::size_t current = (index + i) % table.size();
            metricsData.probes++;

            if (table[current].state == State::EMPTY) {
                return nullptr;
            }

            if (table[current].state == State::OCCUPIED) {
                metricsData.keyComparisons++;

                if (table[current].key == key) {
                    return &table[current].value;
                }
            }
        }

        return nullptr;
    }

    const Value* get(const Key& key) const override {
        std::size_t index = hashIndex(key);

        for (std::size_t i = 0; i < table.size(); ++i) {
            std::size_t current = (index + i) % table.size();
            metricsData.probes++;

            if (table[current].state == State::EMPTY) {
                return nullptr;
            }

            if (table[current].state == State::OCCUPIED) {
                metricsData.keyComparisons++;

                if (table[current].key == key) {
                    return &table[current].value;
                }
            }
        }

        return nullptr;
    }

    bool contains(const Key& key) override {
        return get(key) != nullptr;
    }

    bool remove(const Key& key) override {
        std::size_t index = hashIndex(key);

        for (std::size_t i = 0; i < table.size(); ++i) {
            std::size_t current = (index + i) % table.size();
            metricsData.probes++;

            if (table[current].state == State::EMPTY) {
                return false;
            }

            if (table[current].state == State::OCCUPIED) {
                metricsData.keyComparisons++;

                if (table[current].key == key) {
                    table[current].state = State::DELETED;
                    elementCount--;
                    deletedCount++;
                    return true;
                }
            }
        }

        return false;
    }

    void clear() override {
        table.clear();
        table.resize(initialCapacity);

        elementCount = 0;
        deletedCount = 0;
    }

    std::size_t size() const override {
        return elementCount;
    }

    std::vector<std::pair<Key, Value>> items() const override {
        std::vector<std::pair<Key, Value>> result;
        result.reserve(elementCount);

        for (const auto& entry : table) {
            if (entry.state == State::OCCUPIED) {
                result.emplace_back(entry.key, entry.value);
            }
        }

        return result;
    }

    std::string name() const override {
        return "Hash Open Addressing";
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