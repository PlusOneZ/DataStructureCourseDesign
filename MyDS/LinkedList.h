//
// Created by Zhengyi on 2020/10/1.
//

#ifndef DATASTRUCTURE_LINKEDLIST_H
#define DATASTRUCTURE_LINKEDLIST_H

#include <iostream>
// This is a doubly linked list

template <class Type>
class LinkedList {
public:
    struct LinkNode {
        LinkNode *prev = nullptr;
        LinkNode *next = nullptr;
        Type data;

        LinkNode() = default;
        explicit LinkNode(const Type &);
        LinkNode operator++(int);
        LinkNode &operator++();
        LinkNode operator--(int);
        LinkNode &operator--();
    };

    typedef LinkNode* Link;

    LinkedList();
    LinkedList(LinkedList const &);
    ~LinkedList();

    int length() const;
    bool isEmpty() const;
    Link getHead() const;
    Link getTail() const;
    Link search(const Type& x) const;
    Link locate(int i) const;
    bool insert(int i, const Type& d);
    bool insert(int i, Link link);
    bool remove(int i, Type& d);
    bool remove(int i, Link link = nullptr);
    bool insertBack(const Type& d);


protected:
    Link _head;
    Link _tail;
    int _length = 0;

    bool makeNode(Link &) const;
    bool makeNode(Link &, const Type& x) const;
    bool insertTarget(Link tar, Link link);
};




#endif //DATASTRUCTURE_LINKEDLIST_H
