//
// Created by Zhengyi on 2020/12/7.
//

#ifndef DATASTRUCTURE_HASHTABLE_H
#define DATASTRUCTURE_HASHTABLE_H

#include <string>
#include <exception>
#include <cstdio>
#include "Vector.h"

class HashTableException : public std::exception {
public:
    explicit HashTableException(const char* s) : content(s) { }
    const char *what() const _NOEXCEPT override {
        return content;
    };

    const char* content;
};

size_t hashString(const char *s) {
    size_t h = 0;
    for (; *s; ++s) {
        h = 5*h + *s;
    }
    return h;
}

template <class Key> struct Hash { };

template<> struct Hash<char *> {
    size_t operator()(const char *s) const { return hashString(s); }
};

template<> struct Hash<const char *> {
    size_t operator()(const char *s) const { return hashString(s); }
};

template<> struct Hash<std::string> {
    size_t operator()(const std::string &s) const { return hashString(s.c_str()); }
};

template<> struct Hash<int> {
    size_t operator()(int i) const { return i; }
};

template <class ValueType>
struct HashNode {
    explicit HashNode(const ValueType &v) : next(nullptr), value(v) { }
    HashNode * next;
    ValueType value;
};

template<class ValueType, class Key, class HashFunction,
        class ExtractKey, class EqualKey>
class HashTable {
public:
    explicit HashTable(size_t n);

    inline size_t maxBucketNum() const;
    inline size_t bucketCount() const;
    inline size_t numElements( ) const;
    bool insertUnique(const ValueType& obj);
    void clear();
    ValueType & find(const Key&);
    ValueType & findOrInsert(const ValueType& obj);
    bool hasKey(const Key& k);
    inline bool empty() const;
    size_t erase(const Key& key);
    void resize(size_t hint);

private:
    typedef HashNode<ValueType> node;
    typedef HashFunction hasher;
    typedef EqualKey equal_key;

    hasher hash;
    ExtractKey getKey;
    equal_key equals;

    Vector<node *> buckets;
    size_t num_elements;
    static constexpr int num_primes = 28;
    static const unsigned long prime_list[num_primes];

    inline unsigned long next_prime(unsigned long n);
    inline size_t findBucketKey(const Key &key, size_t size) const ;
    inline size_t findBucket(const ValueType &obj) const ;
    inline size_t findBucket(const ValueType &obj, size_t size) const ;
};

template<class ValueType, class Key, class HashFunction,
        class ExtractKey, class EqualKey>
const unsigned long
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
prime_list[HashTable::num_primes] = {
    53,         97,           193,         389,       769,
    1543,       3079,         6151,        12289,     24593,
    49157,      98317,        196613,      393241,    786433,
    1572869,    3145739,      6291469,     12582917,  25165843,
    50331653,   100663319,    201326611,   402653189, 805306457,
    1610612741, 3221225473ul, 4294967291ul
};

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
unsigned long
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        next_prime(unsigned long n) {
    int i;
    for (i = 0; prime_list[i] < n; ++i) { }
    return prime_list[i];
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        maxBucketNum() const {
    return prime_list[num_primes - 1];
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        bucketCount() const {
    return buckets.length();
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
HashTable<ValueType, Key, HashFunction,ExtractKey, EqualKey>::
        HashTable(size_t n) : num_elements(0) {
    const size_t n_buckets = next_prime(n);
    buckets.reserve(n_buckets);
    for (size_t i = 0; i < n_buckets; ++i) {
        buckets.pushBack(nullptr);
    }
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
void
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        resize(size_t hint) {
    const size_t previous = buckets.length();
    if (hint > previous) {
        const auto n = next_prime(hint);
        Vector<node *> temp(n, nullptr);
        for (int bucket = 0; bucket < previous; ++bucket) {
            node * first = buckets[bucket];
            while (first) {
                size_t new_bucket = findBucket(first->value, n);
                buckets[bucket] = first->next;
                first->next = temp[new_bucket];
                temp[new_bucket] = first;
                first = buckets[bucket];
            }
        }
        buckets.swap(temp);
    }
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        findBucketKey(const Key &key, size_t size) const {
    return hash(key) % size;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        findBucket(const ValueType &obj) const {
    return findBucketKey(getKey(obj), buckets.length());
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        findBucket(const ValueType &obj, size_t size) const  {
    return findBucketKey(getKey(obj), size);
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
bool
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        insertUnique(const ValueType &obj) {
    resize(num_elements + 1);
    const size_t pos = findBucket(obj);
    node *first = buckets[pos];

    for(node *cur = first; cur; cur = cur->next) {
        if (equals(getKey(obj), getKey(cur->value)))
            return false;
    }

    auto temp = new node(obj);
    temp->next = first;
    buckets[pos] = temp;
    ++num_elements;
    return true;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
void
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        clear() {
    for (int i = 0; i < buckets.length(); ++i) {
        node * cur = buckets[i];
        while (cur) {
            auto next = cur->next;
            delete cur;
            cur = next;
        }
        buckets[i] = nullptr;
    }
    num_elements = 0;
    buckets.resize(prime_list[0]);
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
ValueType &
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        find(const Key &k) {
    size_t n = findBucketKey(k, buckets.length());
    node *first = buckets[n];
    if (first == nullptr) throw HashTableException("Key Not Exist");
    for (node *cur = first; cur; cur = cur->next) {
        if (equals(getKey(cur->value), k)) {
            return cur->value;
        }
    }
    throw HashTableException("Key Not Exist");
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
bool
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        hasKey(const Key &k) {
    size_t n = findBucketKey(k, buckets.length());
    node *first = buckets[n];
    if (first == nullptr)
        return false;
    for (node *cur = first; cur; cur = cur->next) {
        if (equals(getKey(cur->value), k)) {
            return true;
        }
    }
    return false;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
bool
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        empty() const {
    return num_elements == 0;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
ValueType &
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        findOrInsert(const ValueType& obj) {
    size_t n = findBucketKey(getKey(obj), buckets.length());
    node *first = buckets[n];

    node *cur;
    for (cur = first; cur; cur = cur->next) {
        if (equals(getKey(cur->value), getKey(obj))) {
            return cur->value;
        }
    }
    // Not found, try inserting
    resize(num_elements + 1);
    // May be updated
    n = findBucketKey(getKey(obj), buckets.length());
    first = buckets[n];
    auto temp = new node(obj);
    temp->next = first;
    buckets[n] = temp;
    ++num_elements;
    return temp->value;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        erase(const Key &key) {
    size_t count = 0;
    size_t n = findBucketKey(key, buckets.length());
    node *first = buckets[n];
    for (node *cur = first, *p = cur; cur;) {
        if (equals(getKey(cur->value), key)) {
            ++count;
            if (cur == buckets[n]) {
                buckets[n] = cur->next;
                delete cur;
                cur = buckets[n];
                continue;
            } else {
                p->next = cur->next;
                delete cur;
                cur = p;
            }
            p = cur;
            cur = cur->next;
        }
    }
    return count;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::numElements() const {
    return num_elements;
}


#endif //DATASTRUCTURE_HASHTABLE_H
