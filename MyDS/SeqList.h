
#ifndef DATASTRUCTURE_SEQLIST_H
#define DATASTRUCTURE_SEQLIST_H

#include "LinearList.h"

static constexpr int defaultSize = 32;

template<class Type>
class SeqList : public LinearList<Type> {
protected:
    Type *arrayData;
    int maxSize;
    int last;

    void resize(int newSize);

public:
    explicit SeqList(int size = 32);

    SeqList(SeqList<Type> &s);

    ~SeqList();

    int capacity() const override { return maxSize; };

    int length() const override { return last; };

    int search(const Type &x) const override;

    int locate(int i) const override;

    bool getData(int i, Type &x) const override;

    bool setData(int i, const Type &x) override;

    bool insert(int i, const Type &x) override;

    bool remove(int i, const Type &x) override;

    bool isEmpty() override { return last == 0; };

    bool isFull() override { return last == maxSize; };

    void sort() override;

    void input() override;

    void output() override;
};


#endif //DATASTRUCTURE_SEQLIST_H
