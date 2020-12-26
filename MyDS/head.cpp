//
// Created by Zhengyi on 2020/10/9.
//

#include "head.h"
#pragma once
#include"head.h"
template<class Type>
void A<Type>::abc() {
    std::cout << "abc";
    return;
}

template<class Type>
void A<Type>::bcd() {
    std::cout << "bcd";
}

template<class Type>
void A<Type>::aaa() {
    A<double> d;
    d.abc();
    d.bcd();
}

class B {
public:
        int s;
    };
//void func() {
//    A<int> a;
//    a.aaa();
//    a.bcd();
//    a.abc();
//    A<long double> b;
//}

