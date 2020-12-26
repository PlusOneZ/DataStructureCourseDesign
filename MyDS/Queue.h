//
// Created by Zhengyi on 2020/12/1.
//

#ifndef DATASTRUCTURE_QUEUE_H
#define DATASTRUCTURE_QUEUE_H

#include <iostream>

// Cyclic queue
template <class Type>
class Queue {
public:
    Queue();
    ~Queue();
    bool enqueue(const Type &d);
    bool dequeue(Type &d);
    bool peekFront(Type &d);
    bool isEmpty();
    int length();
    void clear();
    std::ostream &show(std::ostream &os);

private:
    static constexpr int InitSize = 10;
    int size;
    int capacity;
    int front;
    int rear;
    Type * data;

    int resize(int n);
    void tryExtend();
    void tryShrink();
};

template<class Type>
Queue<Type>::Queue() : size(0), capacity(InitSize), front(0), rear(0){
    data = new Type[capacity];
    // Compiler will throw failure message.
}

template<class Type>
Queue<Type>::~Queue() {
    delete[] data;
}

template<class Type>
bool Queue<Type>::enqueue(const Type &d) {
    tryExtend();
    data[rear] = d;
    rear = (rear+1) % capacity;
    ++size;
    return true;
}

template<class Type>
bool Queue<Type>::dequeue(Type &d) {
    if (isEmpty())
        return false;
    d = data[front];
    front = (front+1) % capacity;
    --size;
    tryShrink();
    return true;
}

template<class Type>
bool Queue<Type>::peekFront(Type &d) {
    if (isEmpty())
        return false;
    d = data[front];
    return true;
}

template<class Type>
bool Queue<Type>::isEmpty() {
    return size == 0;
}

template<class Type>
int Queue<Type>::length() {
    return size;
}

template<class Type>
int Queue<Type>::resize(int n) {
    if (n < size) {
        return -1;                  // data loss is not permitted
    }
    if (n == capacity) {
        return 1;                   // not resize
    }
    auto temp = new Type[n]; // Compiler will throw failure message.
    if (!isEmpty()) {
        int j = 0;
        for (int i = front; i != rear; i = (i+1)%capacity) {
            temp[j++] = data[i];
        }
    }
    capacity = n;
    front = 0;
    rear = size;
    delete [] data;
    data = temp;
    return 1;
}

template<class Type>
void Queue<Type>::tryExtend() {
    if (capacity - size <= 1)
        resize(capacity * 2);
}

template<class Type>
void Queue<Type>::tryShrink() {
    if (isEmpty())
        resize(InitSize);
    else if (capacity > InitSize && capacity / 4 > size)
        resize(capacity / 2);
}

template<class Type>
std::ostream &Queue<Type>::show(std::ostream &os) {
//    for (int i = 0; i < capacity; ++i) {
//        os << data[i] << "\t";
//    }
//    os << std::endl;
    for (int t = front; t != rear; t = (t+1) % capacity) {
        os << data[t] << "\t";
    }
    os << std::endl;
    return os;
}

template<class Type>
void Queue<Type>::clear() {
    front = 0;
    rear = 0;
    size = 0;
    tryShrink();
}


#endif //DATASTRUCTURE_QUEUE_H
