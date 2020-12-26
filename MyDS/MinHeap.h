//
// Created by Zhengyi on 2020/12/15.
//

#ifndef DATASTRUCTURE_MINHEAP_H
#define DATASTRUCTURE_MINHEAP_H

#include "Vector.h"

class MinHeapException : public std::exception {
public:
    explicit MinHeapException(const char* s) : content(s) { }
    const char *what() const _NOEXCEPT override {
        return content;
    };

    const char* content;
};

template<class T>
struct Less {
    bool operator()(const T &left, const T &right) const {
        return left < right;
    }
};

template<class T>
struct Greater {
    bool operator()(const T &left, const T &right) const {
        return left > right;
    }
};

template<class T>
void swap(T &a, T &b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

template<class ElemType, class Compare = Less<ElemType> >
class MinHeap {
public:
    MinHeap() : data() { }
    explicit MinHeap(const Vector<ElemType> &v);

    void sink(int root);
    void swim(int child);
    void push(const ElemType &x);
    void pop();
    void clear();

    ElemType &peek();
    size_t size();
    bool isEmpty();

private:
    Vector<ElemType> data;
};

template<class ElemType, class Compare>
MinHeap<ElemType, Compare>::MinHeap(const Vector <ElemType> &v) {
    data.reserve(v.length());
    for (size_t i = 0; i < v.length(); ++i) {
        data.pushBack(v[i]);
    }
    for (int i = (data.length() - 2) / 2; i >= 0; --i) {
        sink(i);
    }
}

template<class ElemType, class Compare>
void MinHeap<ElemType, Compare>::sink(int root) {
    Compare com;
    int parent = root;
    size_t child = parent * 2 + 1;
    while (child < data.length()) {
        if (child + 1 < data.length() && com(data[child + 1], data[child])) {
            ++child;
        }

        if (com(data[child], data[parent])) {
            swap(data[child], data[parent]);
            parent = child;
            child = parent * 2 + 1;
        } else {
            break;
        }
    }
}

template<class ElemType, class Compare>
void MinHeap<ElemType, Compare>::swim(int child) {
    Compare com;
    int parent = (child - 1) / 2;
    while (parent >= 0) {
        if (com(data[child], data[parent])) {
            swap(data[parent], data[child]);
            child = parent;
            parent = (child - 1) / 2;
        } else {
            break;
        }
    }
}

template<class ElemType, class Compare>
void MinHeap<ElemType, Compare>::push(const ElemType &x) {
    data.pushBack(x);
    swim(data.length() - 1);
}

template<class ElemType, class Compare>
void MinHeap<ElemType, Compare>::pop() {
    if (data.isEmpty())
        throw MinHeapException("Popping from empty heap.");
    swap(data[0], data[data.length() - 1]);
    data.popBack();
    sink(0);

}

template<class ElemType, class Compare>
ElemType &MinHeap<ElemType, Compare>::peek() {
    if (data.isEmpty())
        throw MinHeapException("Peeking from empty heap.");
    return data[0];
}

template<class ElemType, class Compare>
inline
size_t MinHeap<ElemType, Compare>::size() {
    return data.length();
}

template<class ElemType, class Compare>
inline
bool MinHeap<ElemType, Compare>::isEmpty() {
    return data.isEmpty();
}

template<class ElemType, class Compare>
void MinHeap<ElemType, Compare>::clear() {
    data.clear();
}

#endif //DATASTRUCTURE_MINHEAP_H
