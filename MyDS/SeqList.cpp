#include "SeqList.h"

template<class Type>
void SeqList<Type>::resize(int newSize) {
    Type *newData = new(std::nothrow) Type[newSize * sizeof(Type)];
    if (newData == nullptr) {
        std::cerr << "Error Distributing Memory" << std::endl;
        return;
    }
    int len = length();
    memcpy(newData, arrayData, len * sizeof(Type));
    delete[] arrayData;
    arrayData = newData;
}

template<class Type>
SeqList<Type>::SeqList(int size) : maxSize(size), last(0) {
    arrayData = new(std::nothrow) Type[maxSize * sizeof(Type)];
    if (arrayData == nullptr) {
        std::cerr << "Error Distributing Memory" << std::endl;
        maxSize = 0;
    }
}

template<class Type>
SeqList<Type>::SeqList(SeqList<Type> &s) {
    SeqList(s.maxSize);
    if (maxSize != 0 && s.last != 0) {
        memcpy(arrayData, s.arrayData, maxSize * sizeof(Type));
        last = s.last;
    }
}

template<class Type>
SeqList<Type>::~SeqList() {
    delete[] arrayData;
}

template<class Type>
int SeqList<Type>::locate(int i) const {
    if (i >= 0 && i <= last) return i;
    return -1;
}

template<class Type>
int SeqList<Type>::search(const Type &x) const {
    for (int i = 0; i < last; i++) {
        if (arrayData[i] == x) return i;
    }
    return -1;
}

template<class Type>
bool SeqList<Type>::getData(int i, Type &x) const {
    if (locate(i) < 0)
        return false;
    x = arrayData[i];
}

template<class Type>
bool SeqList<Type>::setData(int i, const Type &x) {
    if (locate(i) < 0)
        return false;
    arrayData[i] = x;
}

template<class Type>
bool SeqList<Type>::insert(int i, const Type &x) {
    if (i < 0 || i > last + 1) {
        std::cout << "Inserting Place Illegal" << std::endl;
        return false;
    }
    Type *p = arrayData;
    Type *target = nullptr;
    if (i != last) {
        while (i--) ++p;
        target = p;
        memcpy(p + 1, p, (last - i) * sizeof(Type));
    } else {
        while (i--) ++target;
    }
    *target = x;
    ++last;
    return true;
}

template<class Type>
bool SeqList<Type>::remove(int i, const Type &x) {
    if (locate(i) < 0) {
        std::cout << "Removing Place Illegal" << std::endl;
        return false;
    }
    Type *p = arrayData;
    if (i != last) {
        while (i--) ++p;
        memcpy(p + 1, p, (last - i - 1) * sizeof(Type));
    }
    --last;
    return true;
}

template<class Type>
void SeqList<Type>::sort() {
//empty
}

template<class Type>
void SeqList<Type>::input() {
//empty
}

template<class Type>
void SeqList<Type>::output() {
//empty
}

