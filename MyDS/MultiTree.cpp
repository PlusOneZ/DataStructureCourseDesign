//
// Created by Zhengyi on 2020/12/6.
//

#include "MultiTree.h"

int main() {
    MultiTree<int> mt(4);
    MultiTree<int>::Node* n = mt.trivialFind(4);
    MultiTree<int>::insertChild(n, 20);
    auto sib = MultiTree<int>::insertSibling(n, 50);
    MultiTree<int>::insertSibling(n, 100);
    MultiTree<int>::Node* n2 = mt.trivialFind(100);
    MultiTree<int>::insertChild(n2, 700);
    auto ci = MultiTree<int>::insertChild(sib, 635);
    MultiTree<int>::insertChild(ci, 693);
    MultiTree<int>::insertSibling(ci, 687);
    mt.show(std::cout);
    Vector<int> temp;
    MultiTree<int>::dismissSubTree(sib, temp);
    std::cout << std::endl;
    temp.show(std::cout);
    std::cout << std::endl;
    mt.show(std::cout);
    return 0;
}