//
// Created by Zhengyi on 2020/12/7.
//

#include "HashTable.h"
#include <iostream>

template <class T>
struct Self {
    const T& operator()(const T &t) { return t; }
};

template <class T>
struct Equals {
    bool operator()(const T& t1, const T& t2) {
        return t1 == t2;
    }
};

// class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey

int main() {
    using std::string;
    using std::cout;
    using std::endl;
    HashTable<string, string, Hash<string>, Self<string>, Equals<string>> ht(50);
    cout << ht.bucketCount() << endl;
    ht.insertUnique(string("I'm first"));
    ht.insertUnique(string("Im second"));
    cout << ht.hasKey(string("Im second")) << endl;
    cout << ht.hasKey(string("1")) << endl;
    for (int i = 0; i < 100; ++i) {
        ht.insertUnique(std::to_string(i));
    }
    cout << ht.bucketCount() << endl;
    cout << ht.hasKey("1") << endl;
    cout << ht.erase(string("1")) << endl;
    cout << ht.hasKey("1") << endl;
    return 0;
}