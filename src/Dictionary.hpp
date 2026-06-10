#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <vector>
#include <utility>
#include <string>
#include <cstddef>

struct DictionaryMetrics {
    long long keyComparisons;
    long long collisions;
    long long probes;
    long long rotations;
    long long rehashes;
    double loadFactor;

    DictionaryMetrics()
        : keyComparisons(0),
          collisions(0),
          probes(0),
          rotations(0),
          rehashes(0),
          loadFactor(0.0) {}
};

template <typename Key, typename Value>
class Dictionary {
public:
    virtual ~Dictionary() {}

    virtual bool insert(const Key& key, const Value& value) = 0;

    virtual bool update(const Key& key, const Value& value) = 0;

    virtual Value* get(const Key& key) = 0;

    virtual const Value* get(const Key& key) const = 0;

    virtual bool contains(const Key& key) = 0;

    virtual bool remove(const Key& key) = 0;

    virtual void clear() = 0;

    virtual std::size_t size() const = 0;

    virtual bool empty() const {
        return size() == 0;
    }

    virtual std::vector<std::pair<Key, Value> > items() const = 0;

    virtual std::string name() const = 0;

    virtual DictionaryMetrics metrics() const = 0;

    virtual void resetMetrics() = 0;
};

#endif