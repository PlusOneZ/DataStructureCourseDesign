//
// Created by Zhengyi on 2020/12/15.
//

#include "HuffmanTree.h"

int main() {
    MinHeap<int, Less<int> > maxHeap;
    int a[] = {1, 1, 4, 5, 1, 2, 1, 3};
    for (int i : a) {
        maxHeap.push(i);
    }
    int cost = 0;
    for (int j = 0; j < 7; ++j) {
        int f = maxHeap.peek();
        maxHeap.pop();
        int s = maxHeap.peek();
        maxHeap.pop();
        int ncost = f + s;
        cost += ncost;
        std::cout << f <<" "<<s<<" "<< ncost << std::endl;
        maxHeap.push(ncost);
    }
    std::cout << cost << std::endl;
}
