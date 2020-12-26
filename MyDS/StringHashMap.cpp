//
// Created by Zhengyi on 2020/12/8.
//

#include "StringHashMap.h"
#include <iostream>

int main() {
    using std::cout;
    using std::endl;
    StringHashMap<int> shmi;
    for (int i = 0; i < 100; ++i) {
        shmi.insert(std::to_string(i), i);
    }
    for (int j = 0; j < 100; ++j) {
        cout << j << ": "<< shmi.hasKey(std::to_string(j)) << endl;
        cout << j << ": "<< shmi.find(std::to_string(j)).second << endl;
    }
}
