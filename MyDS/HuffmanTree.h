//
// Created by Zhengyi on 2020/12/15.
//

#ifndef DATASTRUCTURE_HUFFMANTREE_H
#define DATASTRUCTURE_HUFFMANTREE_H

#include "MinHeap.h"

template <class ElemType>
class HuffmanTree {
    struct HuffmanTreeNode {
        typedef HuffmanTreeNode node;
        ElemType data;
        HuffmanTreeNode *leftChild, *rightChild, *parent;
        HuffmanTreeNode() :
            leftChild(nullptr),
            rightChild(nullptr),
            parent(nullptr) { }

        explicit HuffmanTreeNode(const ElemType& d, HuffmanTreeNode* l = nullptr,
                HuffmanTreeNode* r = nullptr, HuffmanTreeNode* p = nullptr) :
                    data(d),
                    leftChild(l),
                    rightChild(r),
                    parent(p) { }

        bool operator <(const node& other) const { return data < other.data; }
        HuffmanTreeNode &operator=(const HuffmanTreeNode& other) {
            if (&other == this)
                return *this;
            leftChild = other.leftChild; rightChild = other.rightChild;
            parent = other.parent;
            data = other.data;
            return *this;
        }
    };

public:
    typedef HuffmanTreeNode Node;

    explicit HuffmanTree(Vector<ElemType> vec);
    HuffmanTree(ElemType arr[], int n);
    ~HuffmanTree() = default;

    ElemType value() const ;
    ElemType cost() const;

private:
    Node *root;
    Vector<ElemType> costReg;
    void merge(Node *n1, Node *n2, Node*& par);
};

template<class ElemType>
void HuffmanTree<ElemType>::merge(Node *n1, Node *n2, Node *&par) {
    par = new Node(n1->data + n2->data, n1, n2);
    n2->parent = par;
    n1->parent = par;
}

template<class ElemType>
HuffmanTree<ElemType>::HuffmanTree(ElemType *arr, int n) {
    MinHeap<Node> mh;
    Node first, second, *parent = nullptr;
    for (int i = 0; i < n; ++i) {
        mh.push(Node(arr[i]));
    }
    for (int j = 0; j < n-1; ++j) {
        first = mh.peek();
        std::cout << "first: " << first.data << "\t";
        mh.pop();
        second = mh.peek();
        std::cout << "second: " << second.data << "\t";
        mh.pop();
        merge(&first, &second, parent);
        mh.push(*parent);
        costReg.pushBack(parent->data);
        std::cout << "parent: " << parent->data << "\n";
        delete parent;
    }

    root = parent;
}

template<class ElemType>
HuffmanTree<ElemType>::HuffmanTree(Vector<ElemType> vec) :
    HuffmanTree(vec.data, vec.length()) {
    // empty
}


template<class ElemType>
ElemType HuffmanTree<ElemType>::value() const {
    return root->data;
}

template<class ElemType>
ElemType HuffmanTree<ElemType>::cost() const {
    ElemType c;
    for (int i = 0; i < costReg.length(); ++i) {
        c = c + costReg[i];
    }
    return c;
}


#endif //DATASTRUCTURE_HUFFMANTREE_H
