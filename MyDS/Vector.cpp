//
// Created by Zhengyi on 2020/11/17.
//

#include "Vector.h"

int main() {
    Vector<int> veci;
    for (int i = 0; i < 10; i++) {
        veci.pushBack(i);
    }
    for (int j = 0; j < 10; ++j) {
        std::cout << veci.pop() << "\t";
        veci.popBack();
    }
    std::cout << "\n" << veci.isEmpty() << "\n";
    veci.reSize(-1);
    veci.pushBack(1);
    std::cout << veci[0] << "\n";
    veci.clear();
    try {
        std::cout << veci[0] << "\n";
    } catch (std::exception &e) {
        std::cout <<  "failed\n";
    }

    return 0;
}