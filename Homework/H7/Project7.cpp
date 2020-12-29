#include <cstring>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>

#ifndef _NOEXCEPT
#define _NOEXCEPT noexcept
#endif

using std::string;

///-------------------------- Vector ----------------------------------------///
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
    static constexpr int LowerBound = 10;

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



///--------------------------- MinHeap --------------------------------------///

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
        if (child + 1 < data.length() &&
            com(data[child + 1], data[child])) {
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



class NaNException : public std::exception {
public:
    const char *what() const noexcept override {
        return "Not a Number!";
    }
};


///--------------------------- BigInteger -----------------------------------///

class BigInteger {
    friend std::ostream &operator <<(std::ostream &, const BigInteger &);
public:
    explicit BigInteger(const string& i = "") :
            integer(check(i)) { }
    BigInteger &operator=(string s);
    ~BigInteger() = default;

    bool operator>(const BigInteger &other) const;
    bool operator<(const BigInteger &other) const;
    BigInteger &operator=(const BigInteger& other);
    bool operator==(const BigInteger &other) const;
    BigInteger operator+(const BigInteger &bi) const;

    static string check(const string &s);

protected:
    string integer = "";
};

BigInteger &BigInteger::operator=(string s) {
    this->integer = std::move(s);
    return *this;
}

bool BigInteger::operator>(const BigInteger &other) const {
    if (integer.length() > other.integer.length()) return true;
    if (integer.length() < other.integer.length()) return false;
    for (int i = 0; i < (int)integer.length(); ++i) {
        if (integer[i] > other.integer[i]) return true;
        if (integer[i] < other.integer[i]) return false;
    }
    return false;
}

bool BigInteger::operator<(const BigInteger &other) const {
    if (integer.length() < other.integer.length()) return true;
    if (integer.length() > other.integer.length()) return false;
    for (int i = 0; i < (int)integer.length(); ++i) {
        if (integer[i] < other.integer[i]) return true;
        if (integer[i] > other.integer[i]) return false;
    }
    return false;
}

bool BigInteger::operator==(const BigInteger &other) const {
    if (integer.length() != other.integer.length()) return false;
    for (int i = 0; i < (int)integer.length(); ++i) {
        if (integer[i] != other.integer[i]) return false;
    }
    return true;
}

string BigInteger::check(const string &s) {
    int headZeros = 0;
    while (headZeros < (int)s.length() && s[headZeros] == '0') {
        ++headZeros;
    }
    for (int i = headZeros; i < (int)s.length(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            throw NaNException();
        }
    }
    return std::string(s.begin() + headZeros, s.end());
}

std::ostream &operator<<(std::ostream &os, const BigInteger &bi) {
    os << bi.integer;
    return os;
}

BigInteger BigInteger::operator+(const BigInteger &bi) const {
    using std::reverse;
    BigInteger result;
    string lhs = integer, rhs = bi.integer;
    reverse(lhs.begin(), lhs.end());
    reverse(rhs.begin(), rhs.end());

    // Adding
    result.integer = "";
    unsigned long length = std::min(lhs.length(), rhs.length());
    int i = 0;
    for (; i < length; i++) {
        // Roughly adding each digit, not caring carrying.
        result.integer.push_back(lhs[i] - '0' + rhs[i]);
    }

    if (length < lhs.length()) {
        result.integer += lhs.substr(i);
    } else if (length < rhs.length()) {
        result.integer += rhs.substr(i);
    }

    int carry = 0;
    for (auto &c : result.integer) {
        int temp = c + carry - '0';
        carry = temp / 10;
        temp %= 10;
        c = temp + '0';
    }
    if (carry != 0) {
        result.integer.push_back('1');
    }

    reverse(result.integer.begin(), result.integer.end());
    auto valid = result.integer.find_first_not_of('0');
    if (valid == string::npos) {
        return BigInteger("0");
    }
    result.integer = result.integer.substr(valid);
    return result;
}

BigInteger &BigInteger::operator=(const BigInteger &other) {
    if (&other == this)
        return *this;
    this->integer = other.integer;
    return *this;
}



///---------------------------- WoodCutter ----------------------------------///

class WoodCutterException : public std::exception {
public:
    explicit WoodCutterException(const char* s) : content(s) { }
    const char *what() const _NOEXCEPT override {
        return content;
    };
    const char* content;
};


class WoodCutter {
public:
    explicit WoodCutter(const Vector<BigInteger> &vec);
    WoodCutter(BigInteger arr[], int n);
    WoodCutter() = default;
    BigInteger calculateCost();
    void readAndRun(std::istream& is, int n);

protected:
    MinHeap<BigInteger> heap;
    BigInteger result;
    bool hasResult = false;
};

WoodCutter::WoodCutter(BigInteger *arr, int n) {
    if (n <= 0)
        throw WoodCutterException("Invalid n for array size.");
    for (int i = 0; i < n; ++i) {
        heap.push(arr[i]);
    }
}

WoodCutter::WoodCutter(const Vector<BigInteger> &vec) {
    if (vec.isEmpty())
        throw WoodCutterException("Empty vector");
    for (int i = 0; i < vec.length(); ++i) {
        heap.push(vec[i]);
    }
}

BigInteger WoodCutter::calculateCost() {
    if (hasResult)
        return result;
    BigInteger cost("0");
    if (heap.size() == 1) {
        cost = heap.peek();
    } else {
        while (heap.size() > 1) {
            BigInteger temp = heap.peek();
            heap.pop();
            temp = temp + heap.peek();
            heap.pop();
            cost = cost + temp;
            heap.push(temp);
        }
    }
    result = cost;
    hasResult = true;
    return cost;
}

void WoodCutter::readAndRun(std::istream& is, int n) {
    if (n <= 0) {
        std::cerr << "n should be positive!\n";
        return;
    }
    if (!heap.isEmpty()){
        heap.clear();
        hasResult = false;
    }
    string temp;
    for (int i = 0; i < n; ++i) {
        if (is.eof()) {
            std::cerr << "Unfinished input!\n";
            break;
        }
        is >> temp;
        try {
            BigInteger bi(temp);
            heap.push(bi);
        } catch (NaNException &e) {
            std::cerr << "Invalid number detected.\n";
            is.ignore(std::numeric_limits<int>::max(), '\n');
            return;
        }
        if (is.bad()) {
            std::cerr << "Invalid input!\n";
            is.clear();
            is.ignore(std::numeric_limits<int>::max(), '\n');
            return;
        }
    }
    std::cout << "Cost is: " << calculateCost() << std::endl;
}


///----------------------------- TestCases ----------------------------------///

class TestCases {
public:
    void run();
private:
    static constexpr int caseNum = 10;
    string tests[caseNum] = {
            "1 9",
            "1 .",
            "10 7 3 5 13 9 11 45 97 57 31",
            "10 12 42 4 6 86 46 80 78 54 2",
            "20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20",
            "3 273646485859594887373722312323 27364648585959488737372231232 7",
            "0 ",
            "8 2 1 3 9 4 11 13 15",
            "8 4 5 1 2 1 3 1 1",
            "19 1"
    };
};

void TestCases::run() {
    WoodCutter factory;
    for (int i = 0; i < caseNum; ++i) {
        std::stringstream sin(tests[i]);
        std::cout << "Case " << i+1 << ": " << tests[i] << std::endl;
        int n = 0;
        sin >> n;
        if (n != 0) {
            factory.readAndRun(sin, n);
        }
        std::cout << std::endl;
    }
    std::cout << "The second and the last test should alert a warning."
              << std::endl;
}



///------------------------- main function ----------------------------------///

#define TEST_CASES

int main() {

#ifdef TEST_CASES
    TestCases tc;
    tc.run();

#else
    using std::cout;
    using std::cin;
    using std::endl;
    int n;
    WoodCutter factory;
    cout << "Enter number(n) of logs to cut, then the length of each log.\n"
            "Number 0 to quit: ";
    cin >> n;
    while (n != 0) {
        factory.readAndRun(cin, n);
        cout << "Enter number(n) of logs to cut, then the length of each log.\n"
                "Number 0 to quit: ";
        cin >> n;
    }
    cout << "Bye~";
#endif
    return 0;
}