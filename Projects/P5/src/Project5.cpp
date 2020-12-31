#include <iostream>
#include <string>
#include <limits>
#include <sstream>

#ifndef _NOEXCEPT
#define _NOEXCEPT noexcept
#endif

using std::string;

/// -------------------------- Cyclic queue -------------------------------- ///
template <class Type>
class Queue {
public:
    Queue();
    ~Queue();

    bool enqueue(const Type &d);
    bool dequeue(Type &d);
    bool peekFront(Type &d) const;
    bool isEmpty() const;
    int length() const;
    void clear();
    std::ostream &show(std::ostream &os) const;

private:
    static constexpr int InitSize = 10;
    int size;
    int capacity;
    int front;
    int rear;
    Type * data;

    bool resize(int n);
    void tryExtend();
    void tryShrink();
};

template<class Type>
Queue<Type>::Queue() : size(0), capacity(InitSize), front(0), rear(0){
    data = new Type[capacity];
    // Compiler will throw failure message if bad allocation happens.
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
bool Queue<Type>::peekFront(Type &d) const {
    if (isEmpty())
        return false;
    d = data[front];
    return true;
}

template<class Type>
bool Queue<Type>::isEmpty() const {
    return size == 0;
}

template<class Type>
int Queue<Type>::length() const {
    return size;
}

template<class Type>
bool Queue<Type>::resize(int n) {
    if (n < size) {
        return false;                  // data loss is not permitted
    }
    if (n == capacity) {
        return true;                   // not resize
    }
    auto temp = new Type[n]; // Compiler will throw failure message.
    if (!isEmpty()) {
        int j = 0;
        for (int i = front; i != rear; i = (i+1)%capacity) {
            // copying data to front part of array
            temp[j++] = data[i];
        }
    }
    capacity = n;
    front = 0;
    rear = size;
    delete [] data;
    data = temp;
    return true;
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
std::ostream &Queue<Type>::show(std::ostream &os) const {
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


/// --------------------------- Bank system -------------------------------- ///

class BankException : public std::exception {
public:
    explicit BankException(const char* s) : content(s) { }
    const char *what() const _NOEXCEPT override {
        return content;
    };

private:
    const char* content;
};

class Bank {
public:
    explicit Bank(std::ostream& os) : ostream(os) { };
    ~Bank() = default;
    void addBusiness(const string& num);
    void handleBusiness();
    void run(int num, std::istream &is);

private:
    Queue<string> windowA;
    Queue<string> windowB;
    std::ostream& ostream;

    enum Window { A, B };
    void handle(Window w);
    static bool isNumeric(const string& num);
};

void Bank::addBusiness(const string& num) {
    if (!isNumeric(num))
        throw BankException("Invalid character!");
    if ((num[num.length()-1] - '0') % 2) {
        windowA.enqueue(num);
    } else {
        windowB.enqueue(num);
    }
}

void Bank::handleBusiness() {
    while (!(windowA.isEmpty() && windowB.isEmpty())) {
        handle(A);
        handle(A);
        handle(B);
    }
    ostream << std::endl;
}

void Bank::handle(Bank::Window w) {
    string m;
    if (w == A) {
        if (windowA.dequeue(m)) {
            ostream << m << ' ';
        }
    } else {
        if (windowB.dequeue(m)) {
            ostream << m << ' ';
        }
    }
}

void Bank::run(int num, std::istream &is) {
    string id;
    for (int i = 0; i < num; ++i) {
        is >> id;
        if (is.eof()) {
            handleBusiness();
            std::cerr << "Unfinished input!\n";
            return;
        }
        try {
            addBusiness(id);
        } catch (BankException &be) {
            std::cerr << be.what() << std::endl;
            is.ignore(std::numeric_limits<int>::max(), '\n');
            return;
        }
        if (is.bad()) {
            std::cerr << "Invalid input!\n";
            is.clear();
            is.ignore(std::numeric_limits<int>::max(), '\n');
            if (is.bad()) throw BankException("Istream jammed.");
        }
    }
    handleBusiness();
}

bool Bank::isNumeric(const string &num) {
    if (num.empty())
        return false;
    for (char d : num) {
        if (d < '0' || d > '9') return false;
    }
    return true;
}

/// ----------------------------- TestCase class --------------------------- ///
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
            "8 2 1 3 9 4 11 12 16",
            "19 1"
    };
};

void TestCases::run() {
    for (int i = 0; i < caseNum; ++i) {
        std::stringstream sin(tests[i]);
        std::cout << "Case " << i+1 << ": " << tests[i] << std::endl;
        int n = 0;
        sin >> n;
        if (n != 0) {
            Bank b(std::cout);
            b.run(n, sin);
        }
        std::cout << std::endl;
    }
    std::cout << "The second and the last test should alert a warning."
    << std::endl;
}


/// ----------------------------- main function ---------------------------- ///

//#define TEST_CASES

int main() {

#ifdef TEST_CASES
    TestCases tc;
    tc.run();
#else
    using std::cout; using std::cin;
    cout << "Enter amount, then the serial number of the clients\n"
            "Enter amount 0 to quit: ";
    int num;
    cin >> num;
    while (num != 0 && !cin.eof()) {
        Bank bank(cout);
        try {
            bank.run(num, cin);
        } catch (std::exception &e) {
            cout << "Exception cannot be handled." << std::endl;
        }
        if (cin.eof())
            break;
        cout << "Enter amount, then the serial numbers of the clients\n"
                "Enter amount 0 to quit: ";
        cin >> num;
    }
    cout << "Bye~" << std::endl;
#endif

    return 0;
}
