#ifndef DATASTRUCTURE_LINEARLIST_H
#define DATASTRUCTURE_LINEARLIST_H

#include <iostream>
#include <cstdlib>

template<class Type>
class LinearList {
public:
    LinearList() = default;

    virtual ~LinearList() = default;

    virtual int capacity() const = 0;

    virtual int length() const = 0;

    virtual int search(const Type &x) const = 0;

    virtual int locate(int i) const = 0;

    virtual bool getData(int i, Type &x) const = 0;

    virtual bool setData(int i, const Type &x) = 0;

    virtual bool insert(int i, const Type &x) = 0;

    virtual bool remove(int i, const Type &x) = 0;

    virtual bool isEmpty() = 0;

    virtual bool isFull() = 0;

    virtual void sort() = 0;

    virtual void input() = 0;

    virtual void output() = 0;
};


#endif //DATASTRUCTURE_LINEARLIST_H
