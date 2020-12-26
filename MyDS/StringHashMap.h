//
// Created by Zhengyi on 2020/12/8.
//

#ifndef DATASTRUCTURE_STRINGHASHMAP_H
#define DATASTRUCTURE_STRINGHASHMAP_H

#include <exception>
#include "HashTable.h"

using std::string;

class SHMApException : public std::exception {
public:
    explicit SHMApException(const char *s) : content(s) {}

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

// class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey

template<class Value>
class StringHashMap {
public:
    typedef Pair<string, Value> MapPair;

    StringHashMap() : ht(50) { }

    size_t size() const { return ht.bucketCount(); }
    size_t maxSize() const { return ht.maxBucketNum(); }
    bool empty() const { return ht.empty(); }

    inline Value &operator[](const string &s);
    MapPair &find(const string &s) { return ht.find(s); }
    const MapPair &find(const string &s) const { return ht.find(s); }
    bool hasKey(const string &s) { return ht.hasKey(s); }
    size_t erase(const string& key) { return ht.erase(key); }
    void resize(size_t hint) { ht.resize(hint); }
    inline void insert(const string& s, const Value& val);
    inline void insert(const MapPair& pair);


private:
    HashTable<
        MapPair,
        string,
        Hash<string>,
        GetPairKey<MapPair, string>,
        Equals<string> > ht;
};

template<class Value>
Value &StringHashMap<Value>::operator[](const string &s) {
    return ht.findOrInsert(MapPair(s, Value())).second;
}

template<class Value>
void StringHashMap<Value>::insert(const StringHashMap::MapPair &pair) {
    try {
        ht.insertUnique(pair);
    } catch (HashTableException &e) {
        throw SHMApException(e.what());
    }
}

template<class Value>
void StringHashMap<Value>::insert(const string &s, const Value &val) {
    insert(MapPair(s, val));
}


#endif //DATASTRUCTURE_STRINGHASHMAP_H
