//
// Created by Zhengyi on 2020/10/1.
//

#include "LinkedList.h"
using namespace std;

template<class Type>
LinkedList<Type>::LinkedList() {
    if (!(makeNode(_head) && makeNode(_tail))) {
        throw runtime_error("Error Creating List");
    }
    _head->next = _tail;
    _head->prev = nullptr;
    _tail->prev = _head;
    _tail->next = nullptr;
    _length = 0;
}

template<class Type>
LinkedList<Type>::LinkedList(LinkedList const &) {
    LinkedList<Type>();

}

template<class Type>
LinkedList<Type>::~LinkedList() {
    Link p = _head;
    Link q = p->next;
    while (q != nullptr) {
        delete p;
        p = q;
        q = p->next;
    }
    delete p;
}


/**
 * @brief Assistant func generates one single node.
 * @param link：return value passed by ref.
 * @return true if success.
 */
template<class Type>
bool LinkedList<Type>::makeNode(Link &link) const {
    link = new(std::nothrow) LinkNode;
    return (link != nullptr);
}

template<class Type>
LinkedList<Type>::LinkNode::LinkNode(const Type &x)
        :prev(nullptr), next(nullptr), data(x) {
    //empty
}

template<class Type>
bool LinkedList<Type>::makeNode(LinkedList::Link &link, const Type &x) const {
    link = new(std::nothrow) LinkNode(x);
    return (link != nullptr);
}

template<class Type>
typename LinkedList<Type>::LinkNode
LinkedList<Type>::LinkNode::operator++(int) {
    auto ret = this;
    ++this;
    return ret;
}

template<class Type>
typename LinkedList<Type>::LinkNode &
LinkedList<Type>::LinkNode::operator++() {
    if (next == nullptr) {
        cerr << "Cannot advance this node" << endl;
        return this;
    }
    return (this = this->next);
}

template<class Type>
typename LinkedList<Type>::LinkNode
LinkedList<Type>::LinkNode::operator--(int) {
    auto ret = this;
    --this;
    return (ret);
}

template<class Type>
typename LinkedList<Type>::LinkNode &
LinkedList<Type>::LinkNode::operator--() {
    if (prev == nullptr) {
        cerr << "Cannot advance this node" << endl;
        return this;
    }
    return (this = this->prev);
}


///
template<class Type>
int LinkedList<Type>::length() const {
    return _length;
}

template<class Type>
bool LinkedList<Type>::isEmpty() const {
    return (_length == 0);
}

template<class Type>
typename LinkedList<Type>::Link LinkedList<Type>::getHead() const {
    return _head;
}

template<class Type>
typename LinkedList<Type>::Link LinkedList<Type>::getTail() const {
    return _tail;
}

template<class Type>
typename LinkedList<Type>::Link LinkedList<Type>::search(const Type &x) const {
    Link p = _head;
    while (p != nullptr) {
        if (p->data == x) {
            return p;
        }
        ++p;
    }
    return nullptr;
}

template<class Type>
typename LinkedList<Type>::Link LinkedList<Type>::locate(int i) const {
    if (i < 0 || i > _length)  {
        cerr << "Invalid Index" << endl;
        return nullptr;
    }
    Link p = _head;
    if (i == 0) {return _head;}
    while (i--) {
        ++p;
    }
    return p;
}

template<class Type>
bool
LinkedList<Type>::insertTarget(LinkedList::Link target, LinkedList::Link link) {
    if (target == nullptr){
        return false;
    }
    link->next = target->next;
    link->next->prev = link;
    target->next = link;
    link->prev = target;
    ++_length;
}

template<class Type>
bool LinkedList<Type>::insert(int i, const Type &d) {
    Link newNode;
    if (!makeNode(newNode, d)) {
        cerr << "Error Distributing Memory in insert()" << endl;
        return false;
    }

    return insert(i, newNode);
}

template<class Type>
bool LinkedList<Type>::insert(int i, LinkedList::Link link) {
    Link target = nullptr;
    if (!(target = locate(i))) {
        return false;
    }
    insertTarget(target, link);
    return true;
}

template<class Type>
bool LinkedList<Type>::remove(int i, Type &d) {
    if (isEmpty()) {
        cerr << "Cannot remove from empty list" << endl;
        return false;
    }
    if (i == 0) {
        cerr << "Cannot remove head" << endl;
        return false;
    }
    Link target;
    if (!(target = locate(i))) {
        return false;
    }
    d = target->data;
    target->prev->next = target->next;
    target->next->prev = target->prev;
    --_length;
    delete target;
    return true;
}

template<class Type>
bool LinkedList<Type>::remove(int i, LinkedList::Link link) {
    if (link == nullptr) {
        Type s;
        return remove(i, s);
    }
    return remove(i, link->data);
}

template<class Type>
bool LinkedList<Type>::insertBack(const Type &d) {
    Link target = _tail->prev;
    Link newNode;
    if (!makeNode(newNode, d)) {
        cerr << "Error Distributing Memory in insert()" << endl;
        return false;
    }
    insertTarget(target, newNode);
    return false;
}





