//
// Created by Zhengyi on 2020/12/1.
//

#include "Queue.h"

using namespace std;
int main() {
    Queue<int> qi;
    for (int i = 0; i < 5; ++i) {
        qi.enqueue(i);
    }
    qi.show(cout);
    for (int j = 0; j < 4; ++j) {
        int m;
        qi.dequeue(m);
//        cout << m << " ";
    }
    qi.show(cout);
    cout << endl;
    for (int i = 0; i < 20; ++i) {
        qi.enqueue(i);
    }
    qi.show(cout);
    for (int j = 0; j < 10; ++j) {
        int m;
        qi.dequeue(m);
//        cout << m << " ";
    }
    qi.show(cout);
    return 0;
}