//
// Created by Zhengyi on 2020/11/17.
//

#ifndef DATASTRUCTURE_VECTOR_H
#define DATASTRUCTURE_VECTOR_H

#include <cstring>
#include <exception>
#include <iostream>

class IllegalVectorAccessing : public std::exception {
    const char *what() const _NOEXCEPT override {
        return "Accessing index not exist";
    }
};

/// One-direction extending vector
template<class ElemType>
class Vector {
public:
    Vector();
    Vector(size_t num, const ElemType& d);
    Vector(const Vector<ElemType> &vec);
    ~Vector();

    void resize(size_t new_size);
    void reserve(size_t n);

    const ElemType &pop() const;
    void popBack();
    void pushBack(const ElemType &e);

    size_t length() const;
    bool isEmpty() const;

    void clear();
    void fill(const ElemType& d);
    void swap(Vector<ElemType> &another);

    ElemType &operator[](size_t index);
    const ElemType &operator[](size_t index) const;
    Vector &operator=(Vector<ElemType> const &another);

    std::ostream &show(std::ostream& os) const;

protected:
    size_t size = 0;
    int capacity = 10;
    ElemType *data;
    static constexpr int LowerBound = 8;

    void extend();
    void tryShrink();
    void reallocate(size_t new_size);
};

template<class ElemType>
Vector<ElemType>::Vector() {
    data = new ElemType[capacity];
}

template<class ElemType>
Vector<ElemType>::Vector(size_t num, const ElemType &d) {
    if (num > 0) {
        capacity = num;
        size = num;
        data = new ElemType[capacity];
        for (int i = 0; i < capacity; ++i) {
            data[i] = d;
        }
    } else {
        Vector();
    }
}


template<class ElemType>
Vector<ElemType>::~Vector() {
    if (capacity > 0) {
        delete[] data;
    }
}

template<class ElemType>
Vector<ElemType> &Vector<ElemType>::operator=(Vector<ElemType> const &another) {
    if (&another == this) {
        return *this;
    }
    if (capacity != another.capacity) {
        delete[] data;
        capacity = another.capacity;
        data = new ElemType[capacity];
    }
    size = another.size;
    if (size > 0) {
        memcpy(data, another.data, sizeof(ElemType) * size);
    }
    return *this;
}

template<class ElemType>
Vector<ElemType>::Vector(const Vector<ElemType> &vec) {
    data = new ElemType[LowerBound];
    *this = vec;
}

template<class ElemType>
void Vector<ElemType>::resize(size_t new_size) {
    if (new_size > capacity)
        reallocate(new_size);
    size = new_size;
    tryShrink();
}

template<class ElemType>
void Vector<ElemType>::reallocate(size_t new_size) {
    if (new_size < 0) {
        std::cerr << "Cannot resize to negative or zero!" << std::endl;
        return;
    }
    if (new_size == capacity)
        return;
    auto new_data = new ElemType[new_size];
    if (new_size > capacity) {
        if (!isEmpty())
            memcpy(new_data, data, sizeof(ElemType) * size);
    } else {
        if (new_size < size) {
            size = new_size;
        }
        memcpy(new_data, data, sizeof(ElemType) * size);
    }
    delete[] data;
    data = new_data;
    capacity = new_size;
}


template<class ElemType>
void Vector<ElemType>::extend() {
    if (capacity == 0) {
        capacity = 2;
        data = new ElemType[capacity];
    } else {
        reallocate(2 * capacity);
    }
}

template<class ElemType>
const ElemType &Vector<ElemType>::pop() const {
    if (!isEmpty())
        return data[size - 1];
    throw IllegalVectorAccessing();
}

template<class ElemType>
void Vector<ElemType>::popBack() {
    if (!isEmpty()) {
        size--;
        tryShrink();
    } else {
        std::cerr << "Popping Back an empty vector!";
    }
}

template<class ElemType>
void Vector<ElemType>::pushBack(const ElemType &e) {
    if (size == capacity) {
        extend();
    }
    data[size++] = e;
}

template<class ElemType>
inline
size_t Vector<ElemType>::length() const {
    return size;
}

template<class ElemType>
inline
bool Vector<ElemType>::isEmpty() const {
    return (size == 0);
}

template<class ElemType>
void Vector<ElemType>::clear() {
    size = 0;
    tryShrink();
}

template<class ElemType>
ElemType &Vector<ElemType>::operator[](size_t index) {
    if (index < 0 || index >= size) {
        throw IllegalVectorAccessing();
    }
    return data[index];
}

template<class ElemType>
const ElemType &Vector<ElemType>::operator[](size_t index) const {
    if (index < 0 || index >= size) {
        throw IllegalVectorAccessing();
    }
    return data[index];
}

template<class ElemType>
void Vector<ElemType>::tryShrink() {
    if (size + 1 <= capacity / 4 && capacity >= LowerBound) {
        reallocate((size + 1) * 2);                   // reduce storage
    }
}

template<class ElemType>
void Vector<ElemType>::reserve(size_t n) {
    if (size + n > capacity)
        reallocate(size + n);
}

template<class ElemType>
void Vector<ElemType>::swap(Vector<ElemType> &another) {
    auto temp_data = data;
    auto temp_cap = capacity;
    auto temp_size = size;
    data = another.data;
    capacity = another.capacity;
    size = another.size;
    another.data = temp_data;
    another.capacity = temp_cap;
    another.size = temp_size;
}

template<class ElemType>
std::ostream &Vector<ElemType>::show(std::ostream &os) const {
    for (int i = 0; i < size; ++i) {
        os << data[i] << '\t';
    }
    os << "\n";
    return os;
}

template<class ElemType>
void Vector<ElemType>::fill(const ElemType &d) {
    for (int i = 0; i < size; ++i) {
        data[i] = d;
    }
}


#endif //DATASTRUCTURE_VECTOR_H
