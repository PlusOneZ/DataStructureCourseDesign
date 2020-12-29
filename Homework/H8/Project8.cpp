
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#ifndef _NOEXCEPT
#define _NOEXCEPT noexcept
#endif

///--------------------------------- Vector ---------------------------------///

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


///--------------------------------- MinHeap --------------------------------///

class MinHeapException : public std::exception {
public:
    explicit MinHeapException(const char* s) : content(s) { }
    const char *what() const _NOEXCEPT override {
        return content;
    };

    const char* content;
};

template<class T>
struct Less {
    bool operator()(const T &left, const T &right) const {
        return left < right;
    }
};

template<class T>
struct Greater {
    bool operator()(const T &left, const T &right) const {
        return left > right;
    }
};

template<class T>
void swap(T &a, T &b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

template<class ElemType, class Compare = Less<ElemType> >
class MinHeap {
public:
    MinHeap() : data() { }
    explicit MinHeap(const Vector<ElemType> &v);

    void sink(int root);
    void swim(int child);
    void push(const ElemType &x);
    void pop();
    void clear();

    ElemType &peek();
    size_t size();
    bool isEmpty();

private:
    Vector<ElemType> data;
};

template<class ElemType, class Compare>
MinHeap<ElemType, Compare>::MinHeap(const Vector <ElemType> &v) {
    data.reserve(v.length());
    for (size_t i = 0; i < v.length(); ++i) {
        data.pushBack(v[i]);
    }
    for (int i = (data.length() - 2) / 2; i >= 0; --i) {
        sink(i);
    }
}

template<class ElemType, class Compare>
void MinHeap<ElemType, Compare>::sink(int root) {
    Compare com;
    int parent = root;
    size_t child = parent * 2 + 1;
    while (child < data.length()) {
        if (child + 1 < data.length() && com(data[child + 1], data[child])) {
            ++child;
        }

        if (com(data[child], data[parent])) {
            swap(data[child], data[parent]);
            parent = child;
            child = parent * 2 + 1;
        } else {
            break;
        }
    }
}

template<class ElemType, class Compare>
void MinHeap<ElemType, Compare>::swim(int child) {
    Compare com;
    int parent = (child - 1) / 2;
    while (parent >= 0) {
        if (com(data[child], data[parent])) {
            swap(data[parent], data[child]);
            child = parent;
            parent = (child - 1) / 2;
        } else {
            break;
        }
    }
}

template<class ElemType, class Compare>
void MinHeap<ElemType, Compare>::push(const ElemType &x) {
    data.pushBack(x);
    swim(data.length() - 1);
}

template<class ElemType, class Compare>
void MinHeap<ElemType, Compare>::pop() {
    if (data.isEmpty())
        throw MinHeapException("Popping from empty heap.");
    swap(data[0], data[data.length() - 1]);
    data.popBack();
    sink(0);

}

template<class ElemType, class Compare>
ElemType &MinHeap<ElemType, Compare>::peek() {
    if (data.isEmpty())
        throw MinHeapException("Peeking from empty heap.");
    return data[0];
}

template<class ElemType, class Compare>
inline
size_t MinHeap<ElemType, Compare>::size() {
    return data.length();
}

template<class ElemType, class Compare>
inline
bool MinHeap<ElemType, Compare>::isEmpty() {
    return data.isEmpty();
}

template<class ElemType, class Compare>
void MinHeap<ElemType, Compare>::clear() {
    data.clear();
}



///------------------------------- HashTable --------------------------------///

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

///--------------------------------- HashMap --------------------------------///

class HashMapException : public std::exception {
public:
    explicit HashMapException(const char *s) : content(s) {}
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


template <class Key, class Value>
class HashMap {
public:
    typedef Pair<Key, Value> MapPair;
    HashMap() : ht(50) { }
    size_t size() const { return ht.bucketCount(); }
    size_t maxSize() const { return ht.maxBucketNum(); }
    bool empty() const { return ht.empty(); }

    inline Value &operator[](const Key &key);
    MapPair &find(const Key &key) { return ht.find(key); }
    const MapPair &find(const Key &key) const { return ht.find(key); }
    bool hasKey(const Key &key) { return ht.hasKey(key); }
    size_t erase(const Key& key) { return ht.erase(key); }
    void resize(size_t hint) { ht.resize(hint); }
    inline void insert(const Key& s, const Value& val);
    inline void insert(const MapPair& pair);

private:
    HashTable<
            MapPair,
            Key,
            Hash<Key>,
            GetPairKey<MapPair, Key>,
            Equals<Key> > ht;
};

template<class Key, class Value>
Value &HashMap<Key, Value>::operator[](const Key &key) {
    return ht.findOrInsert(MapPair(key, Value())).second;
}

template<class Key, class Value>
void HashMap<Key, Value>::insert(const Key &s, const Value &val) {
    insert(MapPair(s, val));
}

template<class Key, class Value>
void HashMap<Key, Value>::insert(const HashMap::MapPair &pair) {
    try {
        ht.insertUnique(pair);
    } catch (HashTableException &e) {
        throw HashMapException(e.what());
    }
}


///--------------------------------- Graph ----------------------------------///
class PrimException : public std::exception {
public:
    explicit PrimException(const char *s) : content(s) {}
    const char *what() const _NOEXCEPT override {
        return content;
    };
    const char *content;
};

class GraphException : public std::exception {
public:
    explicit GraphException(const char *s) : content(s) {}
    const char *what() const _NOEXCEPT override {
        return content;
    };
    const char *content;
};

template <class T>
const T& min(const T& t1, const T& t2) {
    return (t1 < t2) ? t1 : t2;
}

template <class T>
const T& max(const T& t1, const T& t2) {
    return (t1 > t2) ? t1 : t2;
}

template<class Tag>
class Vertex {
public:
    static constexpr int INF = std::numeric_limits<int>::max();
    Vertex<Tag>() = default;
    explicit Vertex<Tag>(const Tag &t) : tag(t) {};

    Vertex<Tag>& find(const Tag &t) {
        for (int i = 0; i < adjacents.length(); ++i) {
            if (adjacents[i].tag == t) {
                return adjacents[i];
            }
        }
        throw GraphException("Adjacency not found");
    }

    int weightTo(const Tag &t) {
        for (int i = 0; i < adjacents.length(); ++i) {
            if (adjacents[i]->tag == t) {
                return weights[i];
            }
        }
        return INF;
    }

    int weightTo(const Vertex &v) {
        return weightTo(v.tag);
    }

    size_t index = 0;
    Tag tag;
    Vector<Vertex *> adjacents;
    Vector<int> weights;
};

template<class VTag>
class Graph {
    template <class V> friend
    Graph<V> minSpanningTree(Graph<V> &graph, const V& start);
public:
    static constexpr int INF = std::numeric_limits<int>::max();

    Graph() = default;

    inline void addEdge(const VTag &u, const VTag &v, int weight);
    inline void addEdge(size_t u, size_t v, int weight);
    inline void addVertex(const VTag& tag);

    bool hasTag(const VTag& t) { return tagMap.hasKey(t); }
    unsigned nodes() const { return numVertex; }
    void reset() { vertexes.fill(Vertex<VTag>()); }
    inline std::ostream &show(std::ostream &os);
    inline std::ostream &showByVertexes(std::ostream &os);

private:
    size_t numVertex = 0;
    Vector<Vertex<VTag> *> vertexes;
    HashMap<VTag, size_t> tagMap;
};


template<class VTag>
void Graph<VTag>::addEdge(const VTag &u, const VTag &v, int weight) {
    if (!(tagMap.hasKey(u) && tagMap.hasKey(v))) {
        throw GraphException("Unknown vertexes");
    }
    addEdge(tagMap[u], tagMap[v], weight);
}

template<class VTag>
void Graph<VTag>::addEdge(size_t u, size_t v, int weight) {
    if (u == v) // self looping not allowed
        return;
    try {
        for (int i = 0; i < vertexes[u]->adjacents.length(); ++i) {
            if (vertexes[u]->adjacents[i]->index == v)
                std::cout << "Multiple edges between two vertexes, making Multi-graph.\n";
        }
        vertexes[u]->adjacents.pushBack(vertexes[v]);
        vertexes[u]->weights.pushBack(weight);
        vertexes[v]->adjacents.pushBack(vertexes[u]);
        vertexes[v]->weights.pushBack(weight);
    } catch (IllegalVectorAccessing &e) {
        throw GraphException("Vertexes out of range");
    }
}

template<class VTag>
void Graph<VTag>::addVertex(const VTag &tag) {
    if (tagMap.hasKey(tag))
        throw GraphException("Same tag not allowed");
    auto v = new Vertex<VTag>(tag);
    v->index = vertexes.length();
    tagMap.insert(tag, vertexes.length());
    vertexes.pushBack(v);
    numVertex++;
}

template<class VTag>
std::ostream &Graph<VTag>::show(std::ostream &os) {
    int cnt = 0;
    bool hasEdge = false;
    for (int i = 0; i < vertexes.length(); ++i) {
        for (int j = 0; j < vertexes[i]->adjacents.length(); ++j) {
            if (vertexes[i]->adjacents[j]->index <= i) continue;
            hasEdge = true;
            os << "[" <<vertexes[i]->tag << "] -(" << vertexes[i]->weights[j]
               << ")-> [" << vertexes[i]->adjacents[j]->tag << "]\t\t";
            if (cnt == 5) { os << '\n'; cnt = 0;}
            cnt++;
        }
    }
    if (!hasEdge) {
        std::cout << "No edge to show.";
    }
    os << std::endl;
    return os;
}

template<class VTag>
std::ostream &Graph<VTag>::showByVertexes(std::ostream &os) {
    int cnt = 0;
    bool hasEdge = false;
    if (vertexes.isEmpty()) {
        os << "No Vertexes.\n";
        return os;
    }
    for (int i = 0; i < vertexes.length(); ++i) {
        std::cout << "Vertex " << vertexes[i]->tag << ": ";
        for (int j = 0; j < vertexes[i]->adjacents.length(); ++j) {
            hasEdge = true;
            os << "[" <<vertexes[i]->tag << "] -(" << vertexes[i]->weights[j]
               << ")-> [" << vertexes[i]->adjacents[j]->tag << "]\t\t";
            if (cnt == 5) { os << "\n\t"; cnt = 0;}
            cnt++;
        }
        cnt = 0;
        if (!hasEdge) os << "Vertex dangling!";
        hasEdge = false;
        os << '\n';
    }
    os << std::endl;
    return os;
}


///---------------------- MinSpanningTree function --------------------------///
template <class V>
Graph<V> minSpanningTree(Graph<V> &graph, const V& start) {
    struct VertexRecord {
        bool jointed = false;
        int weight = Graph<V>::INF;
        int prevVer = 0;

        VertexRecord() = default;
        VertexRecord(int w, int p, bool j = false) :
                weight(w), prevVer(p), jointed(j) {};

        VertexRecord &operator=(const VertexRecord &other) = default;
    };

    struct Edge {
        Edge(int v1, int v2, int w) : from(v1), to(v2), weight(w) {};
        Edge() = default;

        bool operator<(const Edge &E) const { return weight < E.weight; };
        int from, to;
        int weight;
    };

    if (!graph.tagMap.hasKey(start))
        throw GraphException("Tag not exist");

    Graph<V> result;
    size_t numVertex = graph.nodes();
    size_t startIndex = graph.tagMap[start];
    auto rec = new VertexRecord[numVertex];
    rec[startIndex].jointed = true;
    rec[startIndex].weight = 0;
    MinHeap<Edge> edgeHeap;
    result.addVertex(start);

    const size_t startAdjs = graph.vertexes[startIndex]->adjacents.length();
    for (int i = 0; i < startAdjs; ++i) {
        Vertex<V> *v = graph.vertexes[startIndex]->adjacents[i];
        size_t nextIndex = v->index;
        int nextWeight = graph.vertexes[startIndex]->weights[i];
        edgeHeap.push(Edge(startIndex, nextIndex, nextWeight));
        rec[nextIndex].prevVer = startIndex;
        rec[nextIndex].weight = nextWeight;
    }

    while (!edgeHeap.isEmpty()) {
        int curIndex = edgeHeap.peek().to;
        edgeHeap.pop();
        if (rec[curIndex].jointed)
            continue;
        V curTag = graph.vertexes[curIndex]->tag;
        result.addVertex(curTag);
        result.addEdge(graph.vertexes[rec[curIndex].prevVer]->tag,
                curTag, rec[curIndex].weight);
        rec[curIndex].jointed = true;

        std::cout << graph.vertexes[rec[curIndex].prevVer]->tag << " -> "
                    << curTag << std::endl;

        const size_t adjs = graph.vertexes[curIndex]->adjacents.length();
        for (int i = 0; i < adjs; ++i) {
            size_t nextIndex = graph.vertexes[curIndex]->adjacents[i]->index;
            int nextWeight = graph.vertexes[curIndex]->weights[i];
            if (rec[nextIndex].jointed) continue;
            if (rec[nextIndex].weight > nextIndex) {
                rec[nextIndex].prevVer = curIndex;
                rec[nextIndex].weight = nextWeight;
                edgeHeap.push(Edge(curIndex, nextIndex, nextWeight));
            }
        }
    }
    delete [] rec;
    if (result.vertexes.length() < graph.vertexes.length()) {
        throw GraphException("No minimum spanning tree");
    }
    return result;
}


///---------------------------- PowerSystem ---------------------------------///

using std::string;

class PowerSystem {
    enum Command {
        CreatVertex, CreatEdge, BuildTree, ShowTree, ShowGrid, ShowMenu, Exit
    };
public:
    void run();

private:
    Graph<string> graph;
    Graph<string> minSpanTree;
    bool treeOK = false;

    bool parseCommand(const string &c);

    void createVertex();
    void createEdge();
    void buildTree();
    void showTree();
    void showGrid();
    static void showMenu();

    static void clearIStream(std::istream &is) {
        is.clear();
        is.ignore(std::numeric_limits<int>::max(), '\n');
    }
};

bool PowerSystem::parseCommand(const string &c) {
    if (c.length() > 1) {
        std::cout << "Command " << c << " not valid, try type E to show menu.\n";
        return true;
    } else if (c.empty()) {
        std::cout << "Empty command read.\n";
        return true;
    }
    switch (c[0] - 'A') {
        case CreatVertex:
            createVertex();
            break;
        case CreatEdge:
            createEdge();
            break;
        case BuildTree:
            buildTree();
            break;
        case ShowTree:
            showTree();
            break;
        case ShowGrid:
            showGrid();
            break;
        case ShowMenu:
            showMenu();
            break;
        case Exit:
            return false;
        default:
            std::cout << "Command " << c << " not valid, try again!\n";
    }
    return true;
}

void PowerSystem::createVertex() {
    using std::cin; using std::cout;
    int num = -1;
    cout << "Enter amount of vertexes to add: ";
    cin >> num;
    clearIStream(cin);
    while (num < 0) {
        cout << "Amount should be positive!\n" << "Try again(0 to quit): ";
        cin >> num;
        clearIStream(cin);
    }
    if (num == 0) {
        cout << "No vertexes added." << std::endl;
    }
    cout << "Enter Names of Vertexes separated by LINEBREAKS.\n"
         << "Empty line to quit.\n";
    string name;
    int cnt = 0;
    for (int i = 0; i < num; ++i) {
        cout << "#" << i+1 << ": ";
        std::getline(cin, name);
        if (name.empty())
            break;
        while (name.find_first_of(" \t\n") != string::npos) {
            cout << "Input contains whitespaces, try again: ";
            std::getline(cin, name);
        }
        while (graph.hasTag(name)) {
            cout << "Name " << name << " already exist, try again: \n";
            cout << "#" << i+1 << ": ";
            std::getline(cin, name);
        }
        graph.addVertex(name);
        cnt++;
    }
    treeOK = false;
    cout << cnt << " vertexes added successfully!" << std::endl;
}

void PowerSystem::createEdge() {
    using std::cin; using std::cout;
    cout << "Enter edges in the format below, end with LINEBREAKS,"
            " empty line to quit.\n"
         << "FORMAT: <vertex from> <vertex to> <weight> #LINEBREAK#\n";
    string edge;
    int cnt = 0;
    for (;;) {
        cout << "#" << cnt + 1 << ": ";
        std::getline(cin, edge);
        if (edge.empty())
            break;
        std::stringstream ss(edge);
        string from, to;
        ss >> from;
        if (!graph.hasTag(from)) {
            cout << "Tag " << from << " not in the graph." << std::endl;
            continue;
        }
        ss >> to;
        if (!graph.hasTag(to)) {
            cout << "Tag " << to << " not in the graph." << std::endl;
            continue;
        }
        if (from == to) {
            cout << "Cannot create self link!" << std::endl;
            continue;
        }
        int weight;
        ss >> weight;
        if (weight <= 0) {
            cout << "Not valid weight " << weight << "." << std::endl;
            continue;
        }
        graph.addEdge(from, to, weight);
        cnt++;
    }
    treeOK = false;
    cout << cnt << " edges added successfully!\n" << std::endl;
}

void PowerSystem::buildTree() {
    using std::cin; using std::cout;
    cout << "Enter starting point of tree, end with a LINEBREAK: ";
    string tag;
    std::getline(cin, tag);
    while (!graph.hasTag(tag)) {
        cout << "Name " << tag << " not in the net, the graph looks like: \n";
        graph.show(cout);
        cout << "Try again using an existing name please: ";
        std::getline(cin, tag);
    }
    try {
        minSpanTree = minSpanningTree(graph, tag);
        treeOK = true;
        cout << "Tree successfully built, use command D to show." << std::endl;
    } catch (GraphException &e) {
        cout << "Tree not connected, building failed." << std::endl;;
        treeOK = false;
    }
}

void PowerSystem::showTree() {
    if (treeOK) {
        std::cout << "Tree looks like: \n";
        minSpanTree.show(std::cout);
        std::cout << "\n";
    } else {
        std::cout << "Tree not ready, try build or complete the net first."
                  << std::endl;
    }
}

void PowerSystem::showMenu() {
    std::cout << "***************************************************\n"
                 "**        Power Network Cost Simulating System   **\n"
                 "**-----------------------------------------------**\n"
                 "**         A.   Add vertexes to the net          **\n"
                 "**         B.     Add edge to the net            **\n"
                 "**         C. Generate minimum spanning tree     **\n"
                 "**         D.     Show solution tree             **\n"
                 "**         E.        Show Network                **\n"
                 "**         F.          Show menu                 **\n"
                 "**         G.            EXIT                    **\n"
                 "***************************************************\n";
    std::cout << std::endl;
}

void PowerSystem::run() {
    using std::cout; using std::cin;
    cout << "Welcome to Power Network Cost Simulating System, this is menu:\n";
    showMenu();
    cout << "Enter Your first command: ";
    string cmd;
    std::getline(cin, cmd);
    while (parseCommand(cmd)) {
        cout << "Enter your next command: ";
        std::getline(cin, cmd);
        cout << std::endl;
    }
    cout << "\nBye~" << std::endl;
}

void PowerSystem::showGrid() {
    graph.showByVertexes(std::cout);
}


///-------------------------------- main function ---------------------------///

int main() {
    PowerSystem ps;
    ps.run();
    return 0;
}