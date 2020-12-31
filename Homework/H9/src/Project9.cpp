#include <iostream>
#include <string>
#include <exception>
#include <fstream>
#include <sstream>
#include <cstring>
#include <limits>

#ifndef _NOEXCEPT
#define _NOEXCEPT noexcept
#endif

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
    data = new ElemType[vec.capacity];
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
        reallocate((size + 1) * 2); // reduce storage
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

///------------------------------ Queue -------------------------------------///
template <class Type>
class Queue {
public:
    Queue();
    ~Queue();
    bool enqueue(const Type &d);
    bool dequeue(Type &d);
    bool peekFront(Type &d);
    bool isEmpty();
    int length();
    void clear();
    std::ostream &show(std::ostream &os);

private:
    static constexpr int InitSize = 10;
    int size;
    int capacity;
    int front;
    int rear;
    Type * data;

    int resize(int n);
    void tryExtend();
    void tryShrink();
};

template<class Type>
Queue<Type>::Queue() : size(0), capacity(InitSize), front(0), rear(0){
    data = new Type[capacity];
    // Compiler will throw failure message.
}

template<class Type>
Queue<Type>::~Queue() {
    delete[] data;
}

template<class Type>
bool Queue<Type>::enqueue(const Type &d) {
    tryExtend();
    data[rear] = d;
    rear = (rear+1) % capacity;
    ++size;
    return true;
}

template<class Type>
bool Queue<Type>::dequeue(Type &d) {
    if (isEmpty())
        return false;
    d = data[front];
    front = (front+1) % capacity;
    --size;
    tryShrink();
    return true;
}

template<class Type>
bool Queue<Type>::peekFront(Type &d) {
    if (isEmpty())
        return false;
    d = data[front];
    return true;
}

template<class Type>
bool Queue<Type>::isEmpty() {
    return size == 0;
}

template<class Type>
int Queue<Type>::length() {
    return size;
}

template<class Type>
int Queue<Type>::resize(int n) {
    if (n < size) {
        return -1;                  // data loss is not permitted
    }
    if (n == capacity) {
        return 1;                   // not resize
    }
    auto temp = new Type[n]; // Compiler will throw failure message.
    if (!isEmpty()) {
        int j = 0;
        for (int i = front; i != rear; i = (i+1)%capacity) {
            temp[j++] = data[i];
        }
    }
    capacity = n;
    front = 0;
    rear = size;
    delete [] data;
    data = temp;
    return 1;
}

template<class Type>
void Queue<Type>::tryExtend() {
    if (capacity - size <= 1)
        resize(capacity * 2);
}

template<class Type>
void Queue<Type>::tryShrink() {
    if (isEmpty())
        resize(InitSize);
    else if (capacity > InitSize && capacity / 4 > size)
        resize(capacity / 2);
}

template<class Type>
std::ostream &Queue<Type>::show(std::ostream &os) {
    for (int t = front; t != rear; t = (t+1) % capacity) {
        os << data[t] << "\t";
    }
    os << std::endl;
    return os;
}

template<class Type>
void Queue<Type>::clear() {
    front = 0;
    rear = 0;
    size = 0;
    tryShrink();
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

template<> struct Hash<int> {
    size_t operator()(int i) const { return i; }
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
    inline size_t numElements( ) const;
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


///------------------------------- HashMap ----------------------------------///
template<class ValueType, class Key, class HashFunction, class ExtractKey, class EqualKey>
size_t
HashTable<ValueType, Key, HashFunction, ExtractKey, EqualKey>::numElements() const {
    return num_elements;
}

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
    inline size_t numElements() { return ht.numElements(); };

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



///---------------------------------- Graph ---------------------------------///
class GraphException : public std::exception {
public:
    explicit GraphException(const char *s) : content(s) {}
    const char *what() const _NOEXCEPT override {
        return content;
    };
    const char *content;
};


template<class Tag>
class Vertex {
public:
    static constexpr int INF = std::numeric_limits<int>::max();
    Vertex<Tag>() = default;
    explicit Vertex<Tag>(const Tag &t) : tag(t) {};

    Vertex<Tag>& find(const Tag &t) {
        for (int i = 0; i < next.length(); ++i) {
            if (next[i].tag == t) {
                return next[i];
            }
        }
        throw GraphException("Adjacency not found");
    }

    size_t index = 0;
    int inDegree = 0;
    Tag tag;
    Vector<Vertex *> next;
};


template<class VTag>
class Graph {
public:
    Graph() = default;

    inline void addEdge(const VTag &u, const VTag &v);
    inline void addEdge(size_t u, size_t v);
    inline Vertex<VTag> * addVertex(const VTag& tag);
    inline void clear();

    bool hasTag(const VTag& t) { return tagMap.hasKey(t); }
    unsigned nodes() const { return numVertex; }

    Vector<Vertex<VTag> *> vertexes;
private:
    size_t numVertex = 0;
    HashMap<VTag, size_t> tagMap;
};

template<class VTag>
void Graph<VTag>::addEdge(const VTag &u, const VTag &v) {
    if (!(tagMap.hasKey(u) && tagMap.hasKey(v))) {
        throw GraphException("Unknown vertexes");
    }
    addEdge(tagMap[u], tagMap[v]);
}

template<class VTag>
void Graph<VTag>::addEdge(size_t u, size_t v) {
    try {
        vertexes[u]->next.pushBack(vertexes[v]);
        vertexes[v]->next.pushBack(vertexes[u]);
    } catch (IllegalVectorAccessing &e) {
        throw GraphException("Vertexes out of range");
    }
}

template<class VTag>
Vertex<VTag> * Graph<VTag>::addVertex(const VTag &tag) {
    if (tagMap.hasKey(tag)) return nullptr;
    auto vet = new Vertex<VTag>(tag);
    tagMap.insert(tag, vertexes.length());
    vet->index = vertexes.length();
    vertexes.pushBack(vet);
    return vet;
}

template<class VTag>
void Graph<VTag>::clear() {
    tagMap = HashMap<VTag, size_t >();
    for (int i = 0; i < vertexes.length(); ++i) {
        delete vertexes[i];
    }
    vertexes.clear();
}

///------------------------------- Course -----------------------------------///
struct Course {
    Course() = default;
    Course(const string& i, const string& n, int h, int s, bool hasPre) :
            id(i), name(n), hours(h), semester(s), havePre(hasPre) { };
    ~Course() { }

    string id = "0";
    string name = "null";
    int hours = 0;
    int semester = 0;
    int earliestSemester = 0;
    bool havePre = false;
    Vector<string> pres;
};

class CourseTable {
public:
    const int fieldLen = 16;
    int courseNum = 0;
    int maxCourseNum = 0;
    static constexpr int Days = 5;
    static constexpr int CoursePerDay = 10;
    string arrangement[Days][CoursePerDay];

    inline std::ostream &show(std::ostream & os);
};

std::ostream &CourseTable::show(std::ostream &os) {
    using std::endl;
    string result;
    os << string(fieldLen*5 + 5, '-') << endl;
    os << "| Mon            | Tue            "
          "| Wed            | Thur           "
          "| Fri            |\n";
    for (int i = 0; i < CoursePerDay; ++i) {
        for (int j = 0; j < Days; ++j) {
            int len = arrangement[j][i].length();
            len = (len > fieldLen) ? fieldLen : len;
            result += "|";
            result += arrangement[j][i].substr(0, len);
            if (len < fieldLen) {
                result += (string(fieldLen - len, ' '));
            }
        }
        result += "|\n";
    }
    os << result;
    os << string(fieldLen*5 + 5, '-') << endl;
    return os;
}


///------------------------- SchedulingSystem -------------------------------///
class SchedulingSystemException : std::exception {
public:
    explicit SchedulingSystemException(const char* s) : content(s) { }
    const char *what() const _NOEXCEPT override {
        return content;
    };

    const char* content;
};

class SchedulingSystem {
public:
    void arrange();
    void addCourse(Course &course, int &semester);

    bool checkTimeValid(int sem, int d, int h, int span);
    void setCourseHours(int sem, int d, int h, int span, const Course& c);
    void readFile(const string &file);
    void outputToStream(std::ostream & os);

private:
    std::fstream in, out;
    Vector<Course> AllCourse;
    CourseTable semesters[8];
    Graph<string> coursesGraph;
    HashMap<int, Course> coursesMap;
};

void SchedulingSystem::arrange() {
    using std::cout; using std::endl;
    int semester = 0;
    int courseCnt = 0;
    Vector<int> inDegrees;
    Queue<int> validCourses;
    int i;
    for (i = 0; i < coursesGraph.vertexes.length(); ++i)
        inDegrees.pushBack(coursesGraph.vertexes[i]->inDegree);

    for (i = 0; i < inDegrees.length(); ++i) {
        if (coursesMap[i].semester != 0)
            validCourses.enqueue(i);
    }
    for (i = 0; i < inDegrees.length(); ++i) {
        if (inDegrees[i] == 0 && coursesMap[i].semester == 0)
            validCourses.enqueue(i);
    }
    while (!validCourses.isEmpty()) {
        int cur;
        validCourses.dequeue(cur);
        auto CurCourse = coursesMap[cur];
        addCourse(CurCourse, semester);
        courseCnt++;
        for (i = 0; i < coursesGraph.vertexes[cur]->next.length(); ++i) {
            int NextVertexes = coursesGraph.vertexes[cur]->next[i]->index;
            --inDegrees[NextVertexes];
            if (inDegrees[NextVertexes] == 0) {
                validCourses.enqueue(NextVertexes);
                coursesMap[NextVertexes].earliestSemester = (semester + 1);
            }
        }
    }
    if (courseCnt < coursesMap.numElements()) {
        throw SchedulingSystemException("Courses prerequisite form loops.");
    }
}

void SchedulingSystem::addCourse(Course &course, int &semester) {
    using std::cout; using std::endl;
    int tempSmt = semester;
    if (course.earliestSemester > semester)
        semester++; // prerequisite not suited
    if (semester >= 8) {
        throw SchedulingSystemException("Courses prerequisite error");
    }
    if (semesters[semester].courseNum ==
        semesters[semester].maxCourseNum)
        semester++; // current semester full
    bool dealt = false;

    int day, hour;
    int remainHours = 0;
    bool daysOccupied[5] = {false};
    for (day = 0; day < 5; ++day) {
        for (hour = 0; hour < 10; ++hour) {
            if (semesters[semester].arrangement[day][hour].empty())
                ++remainHours;
        }
    }

    if (course.semester != 0) {
        semester = course.semester - 1;
        dealt = true;
    }
    if (semester >= 8) {
        throw SchedulingSystemException("Courses prerequisite error");
    }
    if (remainHours < course.hours) semester++;
    int courseHours = course.hours;
    semesters[semester].courseNum++;
    if (courseHours >= 3) {
        for (day = 0; day < 5; ++day) {
            if (courseHours < 3) break;
            if (daysOccupied[day] ||
                    (day > 0 && daysOccupied[day - 1]) ||
                    (day < 4 && daysOccupied[day + 1]))
                continue;
            for (hour = 0; hour < 8; ++hour) {
                if (courseHours < 3) break;
                if (hour == 3) { ++hour; continue; }
                if (checkTimeValid(semester, day, hour, 3)) {
                    setCourseHours(semester, day, hour, 3, course);
                    courseHours -= 3;
                    daysOccupied[day] = true;
                    break;
                }
            }
        }
    }
    if (courseHours >= 2) {
        for (day = 0; day < 5; ++day) {
            if (courseHours < 2) break;
            if (daysOccupied[day] ||
                    (day > 0 && daysOccupied[day - 1]) ||
                    (day < 4 && daysOccupied[day + 1]) )
                continue;
            for (hour = 0; hour < 9; ++hour) {
                if (courseHours < 2) break;
                if (hour == 4) continue;
                if (checkTimeValid(semester, day, hour, 2)) {
                    setCourseHours(semester, day, hour, 2, course);
                    courseHours -= 2;
                    daysOccupied[day] = true;
                    break;
                }
            }
        }
    }
    if (courseHours >= 1) {
        for (day = 0; day < 5; ++day) {
            if (courseHours < 1) break;
            if (daysOccupied[day] ||
                    (day > 0 && daysOccupied[day - 1]) ||
                    (day < 4 && daysOccupied[day + 1]) )
                continue;
            for (hour = 0; hour < 10; ++hour) {
                if (checkTimeValid(semester, day, hour, 2)) {
                    setCourseHours(semester, day, hour, 2, course);
                    courseHours -= 1;
                    daysOccupied[day] = true;
                    break;
                }
            }
        }
    }
    if (courseHours >= 3) {
        for (day = 0; day < 5; ++day) {
            if (courseHours < 3)break;
            for (hour = 0; hour < 8; ++hour) {
                if (courseHours < 3)break;
                if (hour == 3) {
                    ++hour;
                    continue;
                }
                if (checkTimeValid(semester, day, hour, 3)) {
                    setCourseHours(semester, day, hour, 3, course);
                    courseHours -= 3;
                    daysOccupied[day] = true;
                }
            }
        }
    }
    if (courseHours >= 2) {
        for (day = 0; day < 5; ++day) {
            if (courseHours < 2)break;
            for (hour = 0; hour < 9; ++hour) {
                if (courseHours < 2) break;
                if (hour == 4) continue;
                if (checkTimeValid(semester, day, hour, 2)){
                    setCourseHours(semester, day, hour, 2, course);
                    courseHours -= 2;
                    daysOccupied[day] = true;
                }
            }
        }
    }
    if (courseHours >= 1) {
        for (day = 0; day < 5; ++day) {
            if (courseHours < 1)break;
            for (hour = 0; hour < 10; ++hour) {
                if (checkTimeValid(semester, day, hour, 1)) {
                    setCourseHours(semester, day, hour, 1, course);
                    courseHours -= 1;
                }
            }
        }
    }
    if (dealt)
        semester = tempSmt;
}

bool SchedulingSystem::checkTimeValid(int sem, int d, int h, int span) {
    for (int i = 0; i < span; ++i) {
        if (!semesters[sem].arrangement[d][h + i].empty())
            return false;
    }
    return true;
}

void
SchedulingSystem::setCourseHours(int sem, int d, int h, int span, const Course& c) {
    for (int i = 0; i < span; ++i) {
        semesters[sem].arrangement[d][h + i] = c.name;
    }
}

void SchedulingSystem::readFile(const string &file) {
    using namespace std;
    in.open(file, ios_base::in | ios_base::binary);
    if (!in.is_open()) {
        throw SchedulingSystemException("File error");
    }
    int n, i;
    in >> n;
    int courseCount = 0;
    for (i = 0; i < 8; ++i) {
        in >> semesters[i].maxCourseNum;
        courseCount += semesters[i].maxCourseNum;
    }
    while (in.get() != '\n') continue;
    if (courseCount != n)
        throw SchedulingSystemException("Course num not match!");
    AllCourse.resize(n);
    for (i = 0; i < n; ++i) {
        string s;
        getline(in, s);
        stringstream ss(s);
        ss >> AllCourse[i].id;
        ss >> AllCourse[i].name;
        ss >> AllCourse[i].hours;
        ss >> AllCourse[i].semester;
        while (ss >> s) {
            AllCourse[i].pres.pushBack(s);
        }
        coursesMap.insert(i, AllCourse[i]);
    }
    in.close();
    for (i = 0; i < n; ++i) {
        coursesGraph.addVertex(AllCourse[i].id);
    }
    for (i = 0; i < n; ++i) {
        for (int j = 0; j < AllCourse[i].pres.length(); ++j) {
            coursesGraph.addEdge(AllCourse[i].pres[j], AllCourse[i].id);
        }
    }
}

void SchedulingSystem::outputToStream(std::ostream &os) {
    using std::endl;
    for (int i = 0; i < 8; ++i) {
        os << "Semester " << i + 1 << ": " << endl;
        semesters[i].show(os);
        os << endl << endl;
    }
}

int main() {
    SchedulingSystem ss;
    std::fstream out;
    out.open("out.txt", std::ios_base::out | std::ios_base::binary);
    try {
        ss.readFile("./in.txt");
        ss.arrange();
        ss.outputToStream(std::cout);
        ss.outputToStream(out);
    }
    catch (SchedulingSystemException &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Press any key to quit." << std::endl;
    getchar();
    return 0;
};

