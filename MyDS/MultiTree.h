//
// Created by Zhengyi on 2020/12/6.
//

#ifndef DATASTRUCTURE_MULTITREE_H
#define DATASTRUCTURE_MULTITREE_H

#include <exception>
#include <stdexcept>
#include <iostream>
#include "Vector.h"

using std::exception;

class MultiTreeException : public exception {
public:
    explicit MultiTreeException(const char* s) : content(s) { }
    const char *what() const _NOEXCEPT override {
        return content;
    };

    const char* content;
};

template <class Type>
class MultiTree {
public:
    struct Node {
        Node(const Type& d, Node* par,
                Node * child = nullptr, Node * sib = nullptr) :
            data(d), firstChild(child), sibling(sib), parent(par) {
            // Empty
        }
        Type data;
        Node * firstChild;
        Node * sibling;
        Node * parent;
        Node * insertChild(const Type &d);
        Node * insertSibling(const Type &d);
        Node *leftMostOf();
        Node *rightMostOf();
    };

public:
    explicit MultiTree(const Type &d) {
        root = new Node(d, nullptr);
    }
    ~MultiTree();
    static Node *insertChild(Node *node, const Type &d);
    static Node *insertSibling(Node *node, const Type &d);
    Node *trivialFind(const Type& d);
    std::ostream &show(std::ostream &os);
    int dismissSubTree(Node * node);
    int dismissSubTree(Node * node, Vector<Type> &valList);
    Node *getRoot() const;

private:
    Node * root;
    static bool deleteLeave(Node * node, Node *&parent);
    static Node *recursiveFind(Node *, const Type &d);
    static void showNode(Node *, std::ostream & os);
};

template<class Type>
int
MultiTree<Type>::dismissSubTree(MultiTree::Node *node, Vector<Type> &valList) {
    if (node == nullptr)
        return 0;
    if (node == root && root->sibling == nullptr) {
        throw MultiTreeException("Cannot delete only root");
    }
    auto *child = node->firstChild;
    auto *p = child;
    Node *temp;
    int count = 0;
    if (child != nullptr) {
        while (child->sibling || child->firstChild) {
            if (p->sibling) {
                p = p->sibling;
                continue;
            } else if (p->firstChild) {
                p = p->firstChild;
                continue;
            } else {
                Node* parent;
                valList.pushBack(p->data);
                if (!deleteLeave(p, parent)) {
                    throw MultiTreeException("Fail deleting leave.");
                }
                ++count;
                p = parent;
            }
        }
        valList.pushBack(child->data);
        deleteLeave(child, temp);
        ++count;
    }
    if (node->sibling) {
        if (node->parent)
            node->parent->sibling = node->sibling;
        else
            root = node->sibling;
        node->sibling = nullptr;
    }
    valList.pushBack(node->data);
    node->parent = nullptr;
    deleteLeave(node, temp);
    ++count;

    return count;
}

template<class Type>
int MultiTree<Type>::dismissSubTree(MultiTree::Node *node) {
    Vector<Type> temp;
    return dismissSubTree(node, temp);
}

template<class Type>
bool MultiTree<Type>::deleteLeave(MultiTree::Node * node, Node *&parent) {
    if (node->firstChild == nullptr && node->sibling == nullptr) {
        parent = node->parent;
        if (parent) {
            if (parent->firstChild == node) parent->firstChild = nullptr;
            else parent->sibling = nullptr;
        }
        delete node;
        return true;
    }
    return false;
}

template<class Type>
typename MultiTree<Type>::Node *
MultiTree<Type>::insertChild(MultiTree::Node *node, const Type &d) {
    auto child = node->firstChild;
    Node* ret;
    if (!child) {
        ret = node->insertChild(d);
    } else {
        child = child->rightMostOf();
        ret = child->insertSibling(d);
    }
    if (ret == nullptr)
        throw MultiTreeException("Cannot insert child");
    return ret;
}

template<class Type>
typename MultiTree<Type>::Node *
MultiTree<Type>::insertSibling(MultiTree::Node *node, const Type &d) {
    auto sib = node->rightMostOf();
    auto ret = sib->insertSibling(d);
    if (ret == nullptr)
        throw MultiTreeException("Cannot insert sibling");
    return ret;
}

template<class Type>
typename MultiTree<Type>::Node *MultiTree<Type>::trivialFind(const Type &d) {
    return recursiveFind(root, d);
}

template<class Type>
typename MultiTree<Type>::Node *
MultiTree<Type>::recursiveFind(MultiTree::Node *node, const Type &d) {
    if (node->data == d) return node;
    Node *ret = nullptr;
    if (node->sibling)
        ret = recursiveFind(node->sibling, d);
    if (!ret && node->firstChild)
        ret = recursiveFind(node->firstChild, d);
    return ret;
}

template<class Type>
std::ostream &MultiTree<Type>::show(std::ostream &os) {
    showNode(root, os);
    return os;
}

template<class Type>
void MultiTree<Type>::showNode(MultiTree::Node *node, std::ostream &os) {
    os << node->data << '\t';
    if (node->sibling) {
        showNode(node->sibling, os);
    } else {
        os << '\n';
    }
    if (node->firstChild) {
        showNode(node->firstChild, os);
    }
}

template<class Type>
MultiTree<Type>::~MultiTree() {
    dismissSubTree(root);
    root = nullptr;
}

template<class Type>
typename MultiTree<Type>::Node *MultiTree<Type>::getRoot() const {
    if (root == nullptr)
        throw MultiTreeException("Root deleted.");
    return root;
}


template<class Type>
typename MultiTree<Type>::Node *
MultiTree<Type>::Node::insertChild(const Type &d) {
    if (firstChild != nullptr) {
        return nullptr;
    }
    auto node = new Node(d, this);
    firstChild = node;
    return node;
}

template<class Type>
typename MultiTree<Type>::Node *
MultiTree<Type>::Node::insertSibling(const Type &d) {
    if (sibling != nullptr) {
        return nullptr;
    }
    auto node = new Node(d, this);
    sibling = node;
    return node;
}

template<class Type>
typename MultiTree<Type>::Node *MultiTree<Type>::Node::leftMostOf() {
    auto p = this;
    while (p->firstChild) {
        p = p->firstChild;
    }
    return p;
}

template<class Type>
typename MultiTree<Type>::Node *MultiTree<Type>::Node::rightMostOf() {
    auto p = this;
    while (p->sibling) {
        p = p->sibling;
    }
    return p;
}

#endif //DATASTRUCTURE_MULTITREE_H
