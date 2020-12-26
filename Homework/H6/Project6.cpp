//
// Created by Zhengyi on 2020/12/3.
//

#include <limits>
#include <iostream>
#include <string>
#include <exception>
#include <cstring>
using std::string;

///------------------------------ Vector ------------------------------------///

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

    void swap(Vector<ElemType> &another);

    ElemType &operator[](int index);

    const ElemType &operator[](int index) const;

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
ElemType &Vector<ElemType>::operator[](int index) {
    if (index < 0 || index >= size) {
        throw IllegalVectorAccessing();
    }
    return data[index];
}

template<class ElemType>
const ElemType &Vector<ElemType>::operator[](int index) const {
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



///------------------------- MultiTree --------------------------------------///


class MultiTreeException : public std::exception {
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
    int dismissSubTree(Node *&node);
    int dismissSubTree(Node *&node, Vector<Type> &valList);
    Node *getRoot() const;
    void showNode(struct Node* node, int indent);

private:
    Node * root;
    Vector<int> indentVec;
    static bool deleteLeave(Node *&node, Node *&parent);
    static Node *recursiveFind(Node *, const Type &d);
    static void showNode(Node *, std::ostream & os);
};

template<class Type>
int
MultiTree<Type>::dismissSubTree(Node *&node, Vector<Type> &valList) {
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
    } else {
        node->parent->sibling = nullptr;
    }
    valList.pushBack(node->data);
    node->parent = nullptr;
    deleteLeave(node, temp);
    ++count;
    node = nullptr;
    return count;
}

template<class Type>
int MultiTree<Type>::dismissSubTree(Node *&node) {
    Vector<Type> temp;
    return dismissSubTree(node, temp);
}

template<class Type>
bool MultiTree<Type>::deleteLeave(Node *&node, Node *&parent) {
    if (node->firstChild == nullptr && node->sibling == nullptr) {
        parent = node->parent;
        if (parent) {
            if (parent->firstChild == node) parent->firstChild = nullptr;
            else parent->sibling = nullptr;
        }
        delete node;
        node = nullptr;
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
    indentVec.clear();
    showNode(root, 0);
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
void MultiTree<Type>::showNode(MultiTree::Node *node, int indent) {
    if (indent > 0) {
        for (int i = 0; i < indent - 1; ++i) {
            std::cout << (indentVec[i] ? "│   " : "    ");
        }
        std::cout << (indentVec[indent - 1] ? "├── " : "└── ");
    }

    if (!node) {
        std::cout << ("(null)\n");
        return;
    }
    std::cout << node->data << std::endl;
    if (!node->firstChild && !node->sibling)
        return;

    while (indentVec.length() < indent + 1)
        indentVec.pushBack(0);
    indentVec[indent] = 1;
    showNode(node->firstChild, indent + 1);
    indentVec[indent] = 0;
    showNode(node->sibling, indent + 1);
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



///---------------------------- HashTable -----------------------------------///

class HashTableException : public std::exception {
public:
    explicit HashTableException(const char* s) : content(s) { }
    const char *what() const _NOEXCEPT override {
        return content;
    };

    const char* content;
};

size_t hashString(const char *s) {
    size_t h = 0;
    for (; *s; ++s) {
        h = 5*h + *s;
    }
    return h;
}

template <class Key> struct Hash { };

template<> struct Hash<char *> {
    size_t operator()(const char *s) const { return hashString(s); }
};

template<> struct Hash<const char *> {
    size_t operator()(const char *s) const { return hashString(s); }
};

template<> struct Hash<std::string> {
    size_t operator()(const std::string &s) const { return hashString(s.c_str()); }
};

template <class ValueType>
struct HashNode {
    explicit HashNode(const ValueType &v) : next(nullptr), value(v) { }
    HashNode * next;
    ValueType value;
};

template<class ValueType, class Key, class HashFunction,
        class ExtractKey, class EqualKey>
class HashTable {
public:
    explicit HashTable(size_t n);

    inline size_t maxBucketNum() const;
    inline size_t bucketCount() const;
    bool insertUnique(const ValueType& obj);
    void clear();
    ValueType & find(const Key&);
    ValueType & findOrInsert(const ValueType& obj);
    bool hasKey(const Key& k);
    inline bool empty() const;
    size_t erase(const Key& key);
    void resize(size_t hint);

private:
    typedef HashNode<ValueType> node;
    typedef HashFunction hasher;
    typedef EqualKey equal_key;

    hasher hash;
    ExtractKey getKey;
    equal_key equals;

    Vector<node *> buckets;
    size_t num_elements;
    static constexpr int num_primes = 28;
    static const unsigned long prime_list[num_primes];

    inline unsigned long nextPrime(unsigned long n);
    inline size_t findBucketKey(const Key &key, size_t size) const ;
    inline size_t findBucket(const ValueType &obj) const ;
    inline size_t findBucket(const ValueType &obj, size_t size) const ;
};

template<class ValueType, class Key, class HashFunction,
        class ExtractKey, class EqualKey>
const unsigned long
        HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
        prime_list[HashTable::num_primes] = {
        53,         97,           193,         389,       769,
        1543,       3079,         6151,        12289,     24593,
        49157,      98317,        196613,      393241,    786433,
        1572869,    3145739,      6291469,     12582917,  25165843,
        50331653,   100663319,    201326611,   402653189, 805306457,
        1610612741, 3221225473ul, 4294967291ul
};

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
unsigned long
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
nextPrime(unsigned long n) {
    int i;
    for (i = 0; prime_list[i] < n; ++i) { }
    return prime_list[i];
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
maxBucketNum() const {
    return prime_list[num_primes - 1];
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
bucketCount() const {
    return buckets.length();
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
HashTable<ValueType, Key, HashFunction,ExtractKey, EqualKey>::
HashTable(size_t n) : num_elements(0) {
    const size_t n_buckets = nextPrime(n);
    buckets.reserve(n_buckets);
    for (size_t i = 0; i < n_buckets; ++i) {
        buckets.pushBack(nullptr);
    }
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
void
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
resize(size_t hint) {
    const size_t previous = buckets.length();
    if (hint > previous) {
        const auto n = nextPrime(hint);
        Vector<node *> temp(n, nullptr);
        for (int bucket = 0; bucket < previous; ++bucket) {
            node * first = buckets[bucket];
            while (first) {
                size_t new_bucket = findBucket(first->value, n);
                buckets[bucket] = first->next;
                first->next = temp[new_bucket];
                temp[new_bucket] = first;
                first = buckets[bucket];
            }
        }
        buckets.swap(temp);
    }
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
findBucketKey(const Key &key, size_t size) const {
    return hash(key) % size;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
findBucket(const ValueType &obj) const {
    return findBucketKey(getKey(obj), buckets.length());
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
findBucket(const ValueType &obj, size_t size) const  {
    return findBucketKey(getKey(obj), size);
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
bool
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
insertUnique(const ValueType &obj) {
    resize(num_elements + 1);
    const size_t pos = findBucket(obj);
    node *first = buckets[pos];

    for(node *cur = first; cur; cur = cur->next) {
        if (equals(getKey(obj), getKey(cur->value)))
            return false;
    }

    auto temp = new node(obj);
    temp->next = first;
    buckets[pos] = temp;
    ++num_elements;
    return true;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
void
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
clear() {
    for (int i = 0; i < buckets.length(); ++i) {
        node * cur = buckets[i];
        while (cur) {
            auto next = cur->next;
            delete cur;
            cur = next;
        }
        buckets[i] = nullptr;
    }
    num_elements = 0;
    buckets.resize(prime_list[0]);
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
ValueType &
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
find(const Key &k) {
    size_t n = findBucketKey(k, buckets.length());
    node *first = buckets[n];
    if (first == nullptr) throw HashTableException("Key Not Exist");
    for (node *cur = first; cur; cur = cur->next) {
        if (equals(getKey(cur->value), k)) {
            return cur->value;
        }
    }
    throw HashTableException("Key Not Exist");
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
bool
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
hasKey(const Key &k) {
    size_t n = findBucketKey(k, buckets.length());
    node *first = buckets[n];
    if (first == nullptr)
        return false;
    for (node *cur = first; cur; cur = cur->next) {
        if (equals(getKey(cur->value), k)) {
            return true;
        }
    }
    return false;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
bool
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
empty() const {
    return num_elements == 0;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
ValueType &
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
findOrInsert(const ValueType& obj) {
    size_t n = findBucketKey(getKey(obj), buckets.length());
    node *first = buckets[n];

    node *cur;
    for (cur = first; cur; cur = cur->next) {
        if (equals(getKey(cur->value), getKey(obj))) {
            return cur->value;
        }
    }
    // Not found, try inserting
    resize(num_elements + 1);
    // May be updated
    n = findBucketKey(getKey(obj), buckets.length());
    first = buckets[n];
    auto temp = new node(obj);
    temp->next = first;
    buckets[n] = temp;
    ++num_elements;
    return temp->value;
}

template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::
erase(const Key &key) {
    size_t count = 0;
    size_t n = findBucketKey(key, buckets.length());
    node *first = buckets[n];
    for (node *cur = first, *p = cur; cur;) {
        if (equals(getKey(cur->value), key)) {
            ++count;
            if (cur == buckets[n]) {
                buckets[n] = cur->next;
                delete cur;
                cur = buckets[n];
                continue;
            } else {
                p->next = cur->next;
                delete cur;
                cur = p;
            }
            p = cur;
            cur = cur->next;
        }
    }
    return count;
}


///-------------------------- StringHashMap ---------------------------------///

class SHMApException : public std::exception {
public:
    explicit SHMApException(const char *s) : content(s) {}

    const char *what() const _NOEXCEPT override {
        return content;
    };

    const char *content;
};

template<class First, class Second>
struct Pair {
    typedef First firstType;
    typedef Second SecondType;

    First first;
    Second second;

    Pair() : first(First()), second(Second()) {}
    Pair(const First &t1, const Second &t2) : first(t1), second(t2) {}
    Pair(const Pair &p) : first(p.first), second(p.second) { }

    Pair &operator=(const Pair& p) {
        if (&p == this)
            return *this;
        first = p.first; second = p.second;
        return *this;
    }
};

template<class P, class KeyType>
struct GetPairKey {
    const KeyType &operator()(const P &pair) const {
        return pair.first;
    }
};

template <class T>
struct Equals {
    bool operator()(const T& t1, const T& t2) {
        return t1 == t2;
    }
};

// class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey

template<class Value>
class StringHashMap {
public:
    typedef Pair<string, Value> MapPair;

    StringHashMap() : ht(50) { }

    size_t size() const { return ht.bucketCount(); }
    size_t maxSize() const { return ht.maxBucketNum(); }
    bool empty() const { return ht.empty(); }

    inline Value &operator[](const string &s);
    MapPair &find(const string &s) { return ht.find(s); }
    const MapPair &find(const string &s) const { return ht.find(s); }
    bool hasKey(const string &s) { return ht.hasKey(s); }
    size_t erase(const string& key) { return ht.erase(key); }
    void resize(size_t hint) { ht.resize(hint); }
    inline void insert(const string& s, const Value& val);
    inline void insert(const MapPair& pair);


private:
    HashTable<
            MapPair,
            string,
            Hash<string>,
            GetPairKey<MapPair, string>,
            Equals<string>
            > ht;
};

template<class Value>
Value &StringHashMap<Value>::operator[](const string &s) {
    return ht.findOrInsert(MapPair(s, Value())).second;
}

template<class Value>
void StringHashMap<Value>::insert(const StringHashMap::MapPair &pair) {
    try {
        ht.insertUnique(pair);
    } catch (HashTableException &e) {
        throw SHMApException(e.what());
    }
}

template<class Value>
void StringHashMap<Value>::insert(const string &s, const Value &val) {
    insert(MapPair(s, val));
}

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;


///------------------------- GenealogyTreeSystem ----------------------------///

class GenealogyTreeSystem {
public:

    GenealogyTreeSystem() : tree("") { }

    void run();
    bool parseCommand(char c);
    void addAncestor();
    void addChild();
    void dismissSubFamily();
    void rename();
    static void showMenu();

private:
    typedef MultiTree<string> TreeType;
    typedef TreeType::Node NodeType;
    TreeType tree;
    StringHashMap<NodeType *> regMap;
    StringHashMap<int> collisionCount;

private:
    static std::istream &clearInput(std::istream &is);
    static void notValidCommand(const string &s);
    string collisionHandle(const string &name);
};

bool GenealogyTreeSystem::parseCommand(char c) {
    switch (c - '0') {
        case 0:
            return false;
        case 1:
            addAncestor();
            break;
        case 2:
            addChild();
            break;
        case 3:
            dismissSubFamily();
            break;
        case 4:
            rename();
            break;
        case 5:
            tree.show(cout);
            break;
        default:
            notValidCommand(std::string());
    }
    return true;
}

void GenealogyTreeSystem::addAncestor() {
    cout << "Enter the name of the ancestor, empty string to back-out: " << endl;
    string name;
    getline(cin, name);
    clearInput(cin);
    if (name.empty())
        return;
    if (regMap.hasKey(name)) {
        name = collisionHandle(name);
    }
    auto node = TreeType::insertSibling(tree.getRoot(), name);
    regMap.insert(name, node);
    cout << name << " added!" << endl;
}

void GenealogyTreeSystem::addChild() {
    cout << "Enter the name of the parent, end with Return/Enter.\n"
            "Empty string to back-out: ";
    string name;
    getline(cin, name);
    clearInput(cin);
    if (name.empty()) {
        cout << "No children added" << endl;
        return;
    }
    if (regMap.hasKey(name)) {
        string childName;
        NodeType* node = regMap.find(name).second;
        cout << "Input the amount of children to be added: ";
        int n, cnt = 0;
        cin >> n;
        while (cin.bad() || n <= 0) {
            clearInput(cin);
            cout << "Invalid input, please input a positive integer: ";
            cin >> n;
        }
        cin.ignore(std::numeric_limits<int>::max(), '\n');
        for (int i = 0; i < n; ++i) {
            cout << "Enter the name of the child #" << i+1 <<
                    " end with Return/Enter, empty to quit: ";
            getline(cin, childName);
            clearInput(cin);
            if (childName.empty()) {
                cout << "Exiting..." << endl;
                break;
            }
            if (regMap.hasKey(childName)) {
                childName = collisionHandle(childName);
            }
            auto child = TreeType::insertChild(node, childName);
            regMap.insert(childName, child);
            cout << childName << " added!" << endl;
            cnt++;
        }
        cout << cnt << (cnt > 1 ? " children" : " child") << " added successfully!"
             << endl;
    } else {
        cout << "Person not found, cannot add children!" << endl;
    }
}

void GenealogyTreeSystem::dismissSubFamily() {
    cout << "Enter the name of the ancestor of tree, end with Return/Enter.\n"
            "Empty string to back-out: ";
    string name;
    getline(cin, name);
    clearInput(cin);
    if (name.empty()) {
        cout << "No family dismissed" << endl;
        return;
    }
    if (regMap.hasKey(name)) {
        Vector<string> member;
        NodeType* ancestor = regMap[name];
        try {
            tree.dismissSubTree(ancestor, member);
        } catch (MultiTreeException& e) {
            cout << "Genealogy tree will be empty after remove, action stopped!" << endl;
            return;
        }
        int n = member.length();
        for (int i = 0; i < n; ++i) {
            cout << member[i] << endl;
            regMap.erase(member[i]);
        }
        cout << n << (n>1 ? " members" : " member") << " removed." << endl;
    } else {
        cout << "Person not found, cannot dismiss subtree!" << endl;
    }
}

void GenealogyTreeSystem::rename() {
    cout << "Enter the name of the person to rename, end with Return/Enter.\n"
            "Empty string to back-out: ";
    string name;
    getline(cin, name);
    clearInput(cin);
    if (name.empty()) {
        cout << "No person inputted" << endl;
        return;
    }
    if (regMap.hasKey(name)) {
        cout << "Enter " << name << "'s new name, end with Return/Enter: ";
        string nName;
        getline(cin, nName);
        clearInput(cin);
        while (name.empty() || cin.bad()) {
            cout << "Invalid input, please input non-empty characters: ";
            getline(cin, nName);
            clearInput(cin);
        }
        regMap[name]->data = nName;
    } else {
        cout << "Person not found." << endl;
    }
}

std::istream &GenealogyTreeSystem::clearInput(std::istream &is) {
    if (!is.good()) {
        is.clear();
        cout << "Clearing input stream!" << endl;
    }
//    is.ignore(std::numeric_limits<int>::max(), '\n');
    return is;
}

void GenealogyTreeSystem::notValidCommand(const string &s) {
    cout << s << " is not a valid command.";
}

string GenealogyTreeSystem::collisionHandle(const string &name) {
    int n;
    if (collisionCount.hasKey(name)) {
        n = ++collisionCount[name];
        while (collisionCount.hasKey(name + std::to_string(n))) {
            n = ++collisionCount[name];
        }
    } else {
        collisionCount.insert(name, 1);
        n = 1;
    }
    string newName = name + std::to_string(n);
    cout << "Same name! " << name << " renamed to " << newName << endl;
    return newName;
}

void GenealogyTreeSystem::run() {
    string ancestor;
    cout << "Welcome to GenealogyTreeSystem!\n"
            "To start, first input one ancestor's name, end with Return/Enter: ";
    getline(cin, ancestor);
    clearInput(cin);
    while (ancestor.empty()) {
        cout << "Cannot start with empty name! try again: ";
        getline(cin, ancestor);
        clearInput(cin);
    }
    tree.getRoot()->data = ancestor;
    regMap.insert(ancestor, tree.getRoot());
    string command;
    while (true) {
        showMenu();
        getline(cin, command);
        clearInput(cin);
        if (command.length() != 1) {
            notValidCommand(command);
            getline(cin, command);
            clearInput(cin);
        }
        if (!parseCommand(command[0]))
            break;
    }
}

void GenealogyTreeSystem::showMenu() {
    cout << "\t\t------------Commands-----------\n"
            "\t\t* 1. Add another ancestor     *\n"
            "\t\t* 2. Add children to a member *\n"
            "\t\t* 3. Dismiss someone's family *\n"
            "\t\t* 4. Rename a person          *\n"
            "\t\t* 5. Display Tree             *\n"
            "\t\t* 0. Quit                     *\n"
            "\t\t-------------------------------\n";
    cout << "To use these commands, type the (single) "
            "character and press Return/Enter." << endl;
}

int main() {
    GenealogyTreeSystem sys;
    sys.run();
}