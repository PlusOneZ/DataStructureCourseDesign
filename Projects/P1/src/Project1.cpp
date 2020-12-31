#include <string>
#include <iostream>
#include <functional>
#include <limits>
#include <exception>
#include <cstring>

#ifndef _NOEXCEPT
#define _NOEXCEPT noexcept
#endif

///-------------------------------- Vector ----------------------------------///


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


///------------------------------ HashTable ---------------------------------///

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

    inline unsigned long next_prime(unsigned long n);
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
next_prime(unsigned long n) {
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
    const size_t n_buckets = next_prime(n);
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
        const auto n = next_prime(hint);
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

///--------------------------- StringHashMap --------------------------------///

using std::string;

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
            Equals<string> > ht;
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


///------------------------------ LinkedList --------------------------------///
template <class Type>
class LinkedList {
public:

    struct LinkNode {
        LinkNode *prev = nullptr;
        LinkNode *next = nullptr;
        Type data;

        LinkNode() = default;
        explicit LinkNode(const Type &);
    };

    typedef LinkNode* Link;

    LinkedList();
    LinkedList(LinkedList const &);
    ~LinkedList();

    int length() const;
    bool isEmpty() const;
    Link getHead() const;
    Link getTail() const;
    int search(const Type& x) const;
    int searchByMethod(std::function<bool(Type & )> method);
    Link locate(int i) const;
    Link insert(int i, const Type& d);
    Link insert(int i, Link link);
    Link insertTarget(Link tar, Link link);
    Link insertBack(const Type& d);
    bool remove(int i, Type& d);
    bool remove(int i, Link link = nullptr);
    bool removeTarget(Link link, Type& d);

    void forEach(std::function<void(Type&)> op);


protected:
    Link head;
    Link tail;
    int elemNum = 0;

    bool makeNode(Link &) const;
    bool makeNode(Link &, const Type& x) const;
};

template<class Type>
LinkedList<Type>::LinkedList() {
    if (!(makeNode(head) && makeNode(tail))) {
        throw std::runtime_error("Error Creating List");
    }
    head->next = tail;
    head->prev = nullptr;
    tail->prev = head;
    tail->next = nullptr;
    elemNum = 0;
}

template<class Type>
LinkedList<Type>::LinkedList(LinkedList const &) {
    LinkedList<Type>();

}

template<class Type>
LinkedList<Type>::~LinkedList() {
    Link p = head;
    Link q = p->next;
    while (q != nullptr) {
        delete p;
        p = q;
        q = p->next;
    }
    delete p;
}


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
int LinkedList<Type>::length() const {
    return elemNum;
}

template<class Type>
bool LinkedList<Type>::isEmpty() const {
    return (elemNum == 0);
}

template<class Type>
typename LinkedList<Type>::Link LinkedList<Type>::getHead() const {
    return head;
}

template<class Type>
typename LinkedList<Type>::Link LinkedList<Type>::getTail() const {
    return tail;
}

template<class Type>
int LinkedList<Type>::search(const Type &x) const {
    return searchByMethod([&x](Type &cur) { return (cur == x); });
}

template<class Type>
int
LinkedList<Type>::searchByMethod(std::function<bool(Type & )> method) {
    int i = 1;
    Link p = head->next;
    if (elemNum == 0) {
        std::cerr << "Cannot Search Empty List" << std::endl;
        return -1;
    }
    while (i < elemNum) {
        if (method(p->data)) {
            return i;
        }
        ++p;
        ++i;
    }
    std::cout << "Element not found." << std::endl;
    return -1;
}

template<class Type>
typename LinkedList<Type>::Link LinkedList<Type>::locate(int i) const {
    if (i < 0 || i > elemNum) {
        std::cerr << "Invalid Index" << std::endl;
        return nullptr;
    }
    Link p = head;
    if (i == 0) { return head; }
    while (i--) {
        p = p->next;
    }
    return p;
}

template<class Type>
typename LinkedList<Type>::Link
LinkedList<Type>::insertTarget(LinkedList::Link target, LinkedList::Link link) {
    if (target == nullptr) {
        return nullptr;
    }
    link->next = target->next;
    link->next->prev = link;
    target->next = link;
    link->prev = target;
    ++elemNum;
    return link;
}

template<class Type>
typename LinkedList<Type>::Link
LinkedList<Type>::insert(int i, const Type &d) {
    Link newNode;
    if (!makeNode(newNode, d)) {
        std::cerr << "Error Distributing Memory in insert()" << std::endl;
        return nullptr;
    }

    return insert(i, newNode);
}

template<class Type>
typename LinkedList<Type>::Link
LinkedList<Type>::insert(int i, LinkedList::Link link) {
    Link target = nullptr;
    if (!(target = locate(i))) {
        return nullptr;
    }
    insertTarget(target, link);
    return link;
}

template<class Type>
bool LinkedList<Type>::removeTarget(Link link, Type& d) {
    d = link->data;
    link->prev->next = link->next;
    link->next->prev = link->prev;
    try {
        delete link;
    } catch (std::exception& e) {
        return false;
    }
    --elemNum;
    return true;
}

template<class Type>
bool LinkedList<Type>::remove(int i, Type &d) {
    using namespace std;
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
    --elemNum;
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
typename LinkedList<Type>::Link
LinkedList<Type>::insertBack(const Type &d) {
    Link target = tail->prev;
    Link newNode;
    if (!makeNode(newNode, d)) {
        std::cerr << "Error Distributing Memory in insert()" << std::endl;
        return nullptr;
    }
    return insertTarget(target, newNode);
}

template<class Type>
void LinkedList<Type>::forEach(std::function<void(Type &)> op) {
    if (isEmpty())
        return;
    Link p = head->next;
    int i = elemNum;
    while (i--) {
        op(p->data);
        p = p->next;
    }
}


///----------------------------- Student ------------------------------------///

class Student {
    friend class System;
    enum Sex {Unknown, Female, Male};
protected:
    std::string candidateNum;
    std::string name;
    std::string sex;
    Sex sexAsData;
    static char Sexes[3][8];

    int age = 18;
    std::string category;

public:
    Student() = default;
    Student(std::string &&canNum, std::string &&n, std::string&& s,
            int a, std::string &&cat);
    std::ostream &show(std::ostream &os);
    std::istream &input(std::istream &is);

private:
    static char fillTable(std::string &s);
    static Sex parseSex(const std::string &sex);
};

char Student::Sexes[3][8] = {"unknown", "female", "male"};

Student::Student(std::string &&canNum, std::string &&n,
                 std::string &&s,
                 int a, std::string &&cat) :
        candidateNum(std::move(canNum)),
        name(std::move(n)),
        sex(s),
        age(a),
        category(std::move(cat)) {
//empty
}

std::ostream &Student::show(std::ostream &os) {
    os << candidateNum << fillTable(candidateNum)
       << '\t' << name << fillTable(name)
       << "\t\t" << Sexes[sexAsData]
       << "\t" << age
       << "\t\t" << category;
    os << std::endl;
    return os;
}

Student::Sex Student::parseSex(const std::string &sex) {
    if (sex[0] == 'm')
        return Male;
    if (sex[0] == 'f')
        return Female;
    else {
        return Unknown;
    }
}

char Student::fillTable(std::string &s) {
    if (s.length() < 4) return '\t';
    return '\7';
}

std::istream &Student::input(std::istream &is) {
    is >> candidateNum >> name >> sex >> age >> category;
    sexAsData = parseSex(sex);
    if (sexAsData == Unknown)
        std::cout << "Unidentified sex " << sex << " modified to 'unknown'\n";
    return is;
}


///------------------------------ System ------------------------------------///

class SystemException : public std::exception {
public:
    explicit SystemException(const char* s) : content(s) { }
    const char *what() const _NOEXCEPT override {
        return content;
    };
private:
    const char* content;
};

class System {
    typedef LinkedList<Student>::Link Link;
public:
    enum Command { kQuit, kInsert, kDelete, kFind, kModify, kStatistic, kMenu };
    System();
    void run();

protected:
    static void menu();
    int readCommand();
    void insert();
    void remove();
    void find();
    void modify();
    void statistic();
    void show(int option = 0);
    void statAdd(const Student &s);
    void statSub(const Student &s);

    LinkedList<Student> list;
    StringHashMap<Link> hashMap;
    long long maleCnt = 0;
    long long femaleCnt = 0;
};


System::System() : list(LinkedList<Student>()) {
    using namespace std;
    int i = 0;
    cout << "Constructing Registering System!\n"
         << "Enter amount of candidates: ";
    while (i <= 0) {
        if (cin.bad()) {
            cin.clear();
            cin.ignore(std::numeric_limits<int>::max(), '\n');
        }
        if (cin.eof()) {
            throw SystemException("Initialization Fault");
        }
        cin >> i;
        if (i <= 0) {
            std::cout << "Amount cannot be less than 0! Try again: " << std::endl;
        }
    }
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    cout << "Enter candidate's ID, name, sex, age and "
            "type of examination one by one: " << endl;
    Student s;
    int cnt = 0;
    while (i--) {
        cout << "#" << ++cnt << ": " << flush;
        if (cin.get() == '\n') {
            --cnt;
            break;
        }
        cin.unget();
        s.input(cin);
        while (hashMap.hasKey(s.candidateNum)) {
            cout << "Candidate with same ID not allowed!\n";
            cout << "Existing student: \n";
            hashMap.find(s.candidateNum).second->data.show(cout);
            cout << "Try again: ";
            s.input(cin);
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        }
        statAdd(s);
        auto l = list.insertBack(s);
        hashMap.insert(s.candidateNum, l);
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    }
    cout << "\nTotal " << cnt << (cnt > 1 ? " students" : " student") << " added"
         << endl;
    show(1);
}

void System::show(int option) {
    using namespace std;
    if (option >= 0){
        cout << "No ID\t\tName\t\tSex\t\tAge\t\tType of Examination\n";
        int i = 0;
        list.forEach([&i](Student &s) {
            cout << ++i << ". ";
            s.show(cout);
        });
    }
    if (option > 0) {
        menu();
    }
    cout << "Enter your command, 6 for menu: " << endl;
}

int System::readCommand() {
    int cmd;
    std::cin >> cmd;
    switch (cmd) {
        case kQuit:
            return 0;
        case kInsert:
            insert();
            break;
        case kDelete:
            remove();
            break;
        case kFind:
            find();
            break;
        case kModify:
            modify();
            break;
        case kStatistic:
            statistic();
            break;
        case kMenu:
            menu();
            break;
        default:
            std::cout << "Invalid command, try again? (enter 6 for menu): ";
    }
    return 1;
}

void System::run() {
    while (readCommand()) {
        //empty
    }
}

void System::menu() {
    using namespace std;
    cout << endl;
    cout << "|***** Menu of Commands *****|\n"
         << "|  1: Insert      2: Delete  |\n"
         << "|  3: Find        4: Edit    |\n"
         << "|  5: Statistics  0: Exit    |\n";
    cout << endl;
}

void System::insert() {
    using namespace std;
    int pos;
    Student s;
    cout << "Enter the position of candidate: ";
    cin >> pos;
    while (pos - 1 < 0 || pos - 1 > list.length()) {
        cout << "Position should be between " << (list.isEmpty() ? 0 : 1) <<
                " to " << list.length() + 1 << endl;
        cout << "Try again: ";
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cin >> pos;
    }
    cin.clear();
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    cout << "Enter candidate's ID, name, sex, age and "
            "type of examination: " << endl;
    s.input(cin);
    while (hashMap.hasKey(s.candidateNum)) {
        cout << "Candidate with same ID not allowed!\n";
        cout << "Existing student: \n";
        hashMap.find(s.candidateNum).second->data.show(cout);
        cout << "Try again: ";
        s.input(cin);
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    }
    Link temp;
    if (!(temp = list.insert(pos - 1, s))) {
        cout << "Failed, check your input!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    } else {
        statAdd(s);
        hashMap.insert(s.candidateNum, temp);
    }
    show();
}

void System::remove() {
    using namespace std;
    int i = -1;
    cout << "Choose to remove by [1. Position, 2. Candidate ID]: " << endl;
    cin >> i;
    while (i != 1 && i != 2) {
        cout << "Try again, enter 1 / 2: " << endl;
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cin >> i;
    }
    Student tempStu;
    if (i == 2) {
        string canNum;
        cout << "Enter candidate's ID to remove: ";
        cin >> canNum;
        if (!hashMap.hasKey(canNum)) {
            cout << "Candidate with ID " << canNum << " not found!" << endl;
            show(-1);
            return;
        } else {
            Link tar = hashMap.find(canNum).second;
            list.removeTarget(tar, tempStu);
            hashMap.erase(canNum);
            statSub(tempStu);
        }
    } else if (i == 1) {
        int tar;
        cout << "Enter candidate's position to remove: " << endl;
        cin >> tar;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!list.remove(tar, tempStu)) {
            cout << "Position " << tar << " not found!" << endl;
            show(-1);
            return;
        }
        hashMap.erase(tempStu.candidateNum);
        statSub(tempStu);
    }
    cout << "The candidate removed is: \n";
    tempStu.show(cout);
    show();
}

void System::find() {
    using namespace std;
    cout << "Enter candidate's ID to find: " << endl;
    string canNum;
    cin >> canNum;
    cin.clear();
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    if (!hashMap.hasKey(canNum)) {
        cout << "Candidate with ID " << canNum << " not found" << endl;
    } else {
        hashMap[canNum]->data.show(cout);
    }
    show(-1);
}

void System::modify() {
    using namespace std;

    int i = -1;
    cout << "Choose to edit by [1. Position, 2. Candidate ID]: " << endl;
    cin >> i;
    while (i != 1 && i != 2) {
        cout << "Try again, enter 1 / 2: " << endl;
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cin >> i;
    }
    Student tempStu;
    if (i == 2) {
        string canNum;
        cout << "Enter candidate's ID to edit: ";
        cin >> canNum;
        if (!hashMap.hasKey(canNum)) {
            cout << "Candidate with ID " << canNum << " not found!" << endl;
            show(-1);
            return;
        } else {
            Link tar = hashMap.find(canNum).second;
            statSub(tar->data);
            hashMap.erase(canNum);
            cout << "Enter candidate's ID, name, sex, age and "
                    "type of examination: "  << endl;
            tempStu.input(cin);
            while (hashMap.hasKey(tempStu.candidateNum)) {
                cout << "Candidate with same ID not allowed!\n";
                cout << "Existing student: \n";
                hashMap.find(tempStu.candidateNum).second->data.show(cout);
                cout << "Try again: ";
                tempStu.input(cin);
                cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            }
            tar->data = tempStu;
            hashMap.insert(tempStu.candidateNum, tar);
            statAdd(tempStu);
        }
    } else if (i == 1) {
        int pos = 0;
        cout << "Enter candidate's position to edit: " << endl;
        cin >> pos;
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        LinkedList<Student>::Link link;
        if ((link = list.locate(pos)) == nullptr) {
            cout << "Position " << pos << " not found!" << endl;
            show(-1);
            return;
        }
        statSub(link->data);
        Student s;
        cout << "Enter candidate's ID, name, sex, age and "
                "type of examination: "  << endl;
        s.input(cin);
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        link->data = s;
        statAdd(s);
    }
    show();
}

void System::statistic() {
    std::cout << "Male: " << maleCnt << "   Female: " << femaleCnt
              << "    Unknown: " << list.length() - maleCnt - femaleCnt;
    std::cout << std::endl;
    show();
}

void System::statAdd(const Student &s) {
    if (s.sexAsData == Student::Male)
        maleCnt++;
    if (s.sexAsData == Student::Female)
        femaleCnt++;
}

void System::statSub(const Student &s) {
    if (s.sexAsData == Student::Male)
        maleCnt--;
    if (s.sexAsData == Student::Female)
        femaleCnt--;
}


///----------------------------  main function ------------------------------///

int main() {
    System s;
    s.run();
    std::cout << "Bye~\n";
    return 0;
}


/*
1234 alice female 20 SSE
2345 Bob male 20 CS
3456 zzy m 20 DataStructure
4567 miao pet 5 EatAndSleep
 */
