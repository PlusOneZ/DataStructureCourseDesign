//
// Created by Zhengyi on 2020/12/21.
//

#ifndef DATASTRUCTURE_HASHMAP_H
#define DATASTRUCTURE_HASHMAP_H

#include "HashTable.h"

class HashMapException : public std::exception {
public:
    explicit HashMapException(const char *s) : content(s) {}
    const char *what() const _NOEXCEPT override {
        return content;
    };
    const char *content;
};

template<class First, class Second>
struct Pair {
    typedef First firstType;
    typedef Second SecondType;

    First first;
    Second second;

    Pair() : first(First()), second(Second()) {}
    Pair(const First &t1, const Second &t2) : first(t1), second(t2) {}
    Pair(const Pair &p) : first(p.first), second(p.second) { }

    Pair &operator=(const Pair& p) {
        if (&p == this)
            return *this;
        first = p.first; second = p.second;
        return *this;
    }
};

template<class P, class KeyType>
struct GetPairKey {
    const KeyType &operator()(const P &pair) const {
        return pair.first;
    }
};

template <class T>
struct Equals {
    bool operator()(const T& t1, const T& t2) {
        return t1 == t2;
    }
};


template <class Key, class Value>
class HashMap {
public:
    typedef Pair<Key, Value> MapPair;
    HashMap() : ht(50) { }
    size_t size() const { return ht.bucketCount(); }
    size_t maxSize() const { return ht.maxBucketNum(); }
    bool empty() const { return ht.empty(); }

    inline Value &operator[](const Key &key);
    MapPair &find(const Key &key) { return ht.find(key); }
    const MapPair &find(const Key &key) const { return ht.find(key); }
    bool hasKey(const Key &key) { return ht.hasKey(key); }
    size_t erase(const Key& key) { return ht.erase(key); }
    void resize(size_t hint) { ht.resize(hint); }
    inline void insert(const Key& s, const Value& val);
    inline void insert(const MapPair& pair);
    inline size_t numElements() { return ht.numElements(); };

private:
    HashTable<
            MapPair,
            Key,
            Hash<Key>,
            GetPairKey<MapPair, Key>,
            Equals<Key> > ht;
};

template<class Key, class Value>
Value &HashMap<Key, Value>::operator[](const Key &key) {
    return ht.findOrInsert(MapPair(key, Value())).second;
}

template<class Key, class Value>
void HashMap<Key, Value>::insert(const Key &s, const Value &val) {
    insert(MapPair(s, val));
}

template<class Key, class Value>
void HashMap<Key, Value>::insert(const HashMap::MapPair &pair) {
    try {
        ht.insertUnique(pair);
    } catch (HashTableException &e) {
        throw HashMapException(e.what());
    }
}


#endif //DATASTRUCTURE_HASHMAP_H
