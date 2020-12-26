//
// Created by Zhengyi on 2020/12/15.
//

#include "MinHeap.h"

int main() {
    MinHeap<int> a;
    a.size();
    a.isEmpty();
    for (int i = 0; i < 100; ++i) {
        a.push(100 - i);
    }
    for (int i = 0; i < 100; ++i) {
        std::cout << a.peek() << " ";
        a.pop();
    }
//    a.pop();
}