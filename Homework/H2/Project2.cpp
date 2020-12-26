#include <string>
#include <iostream>
#include <functional>
#include <limits>
#include <utility>

///-------------------------- LinkedList ------------------------------------///
template<class Type>
class LinkedList {
public:

    struct LinkNode {
        LinkNode *prev = nullptr;
        LinkNode *next = nullptr;
        Type data;

        LinkNode() = default;
        explicit LinkNode(const Type &x);
    };

    typedef LinkNode *Link;

    class Cursor {
    protected:
        LinkedList *_list;
        Link _current;

    public:
        explicit Cursor(LinkedList *l);
        void reset();
        void next();
        Link current();
        const Type &getElement();

    };

    LinkedList();
    ~LinkedList();

    inline int length() const;
    inline bool isEmpty() const;
    void clear();
    Link locate(int i) const;
    bool insert(int i, const Type &d);
    bool insert(int i, Link link);
    bool remove(int i, Type &d);
    bool remove(int i, Link link = nullptr);
    bool insertBack(const Type &d);
    void forEach(std::function<void(Type &)>);
    void show();
    Cursor getCursor();

protected:
    Link _head;
    Link _tail;
    int _length = 0;

    bool makeNode(Link &) const;
    bool makeNode(Link &, const Type &x) const;
    bool insertTarget(Link tar, Link link);
};


template<class Type>
LinkedList<Type>::LinkedList() {
    if (!(makeNode(_head) && makeNode(_tail))) {
        throw std::runtime_error("Error Creating List");
    }
    _head->next = _tail;
    _head->prev = nullptr;
    _tail->prev = _head;
    _tail->next = nullptr;
    _length = 0;
}

template<class Type>
LinkedList<Type>::~LinkedList() {
    clear();
    delete _head;
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
    return _length;
}

template<class Type>
bool LinkedList<Type>::isEmpty() const {
    return (_length == 0);
}

template<class Type>
typename LinkedList<Type>::Link LinkedList<Type>::locate(int i) const {
    if (i < 0 || i > _length) {
        std::cerr << "Invalid Index" << std::endl;
        return nullptr;
    }
    Link p = _head;
    if (i == 0) { return _head; }
    while (i--) {
        p = p->next;
    }
    return p;
}

template<class Type>
bool
LinkedList<Type>::insertTarget(Link target, Link link) {
    if (target == nullptr) {
        return false;
    }
    link->next = target->next;
    link->next->prev = link;
    target->next = link;
    link->prev = target;
    ++_length;
    return true;
}

template<class Type>
bool LinkedList<Type>::insert(int i, const Type &d) {
    Link newNode;
    if (!makeNode(newNode, d)) {
        std::cerr << "Error Distributing Memory in insert()" << std::endl;
        return false;
    }

    return insert(i, newNode);
}

template<class Type>
bool LinkedList<Type>::insert(int i, Link link) {
    Link target = nullptr;
    if (!(target = locate(i))) {
        return false;
    }
    insertTarget(target, link);
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
        std::cerr << "Error Distributing Memory in insert()" << std::endl;
        return false;
    }
    insertTarget(target, newNode);
    return false;
}

template<class Type>
void LinkedList<Type>::forEach(std::function<void(Type &)> op) {
    if (isEmpty())
        return;
    Link p = _head->next;
    int i = _length;
    while (i--) {
        op(p->data);
        p = p->next;
    }
}

template<class Type>
void LinkedList<Type>::clear() {
    if (isEmpty())
        return;
    Link p = _head->next;
    Link q = p->next;
    while (q != _tail && q != nullptr) {
        delete p;
        p = q;
        q = p->next;
    }
    delete p;
    _length = 0;
    _head->next = _tail;
    _tail->prev = _head;
}

template<class Type>
typename LinkedList<Type>::Cursor LinkedList<Type>::getCursor() {
    return LinkedList::Cursor(this);
}

template<class Type>
void LinkedList<Type>::show() {
    forEach([](Type &d){
        std::cout << d;
        std::cout << ' ';
    });
    std::cout << std::endl;
}

template<class Type>
LinkedList<Type>::Cursor::Cursor(LinkedList *l) : _list(l) {
    _current = _list->_head->next;
}

template<class Type>
void LinkedList<Type>::Cursor::reset() {
    if (_list->_head == nullptr) {
        _current = nullptr;
        return;
    }
    _current = _list->_head->next;
}

template<class Type>
void LinkedList<Type>::Cursor::next() {
    if (_current != nullptr) {
        _current = _current->next;
    } else {
        std::cerr << "Error: Cannot advance null cursor." << std::endl;
    }
}

template<class Type>
typename LinkedList<Type>::Link LinkedList<Type>::Cursor::current() {
    return _current;
}

template<class Type>
const Type &LinkedList<Type>::Cursor::getElement() {
    if (_current == nullptr) {
        std::cerr << "Error: Cannot get null node's element." << std::endl;
    }
    return _current->data;
}


///----------------------- BigInteger ------------------ --------------------///
class NaNException : public std::exception {
public:
    const char *what() const noexcept override {
        return "Not a Number!";
    }
};

class BigInteger {
    friend std::ostream &operator <<(std::ostream &, const BigInteger &);
public:
    explicit BigInteger(const std::string &i = "") :
            integer(check(i)) { }

    BigInteger &operator=(std::string s);
    bool operator>(const BigInteger &other) const;
    bool operator<(const BigInteger &other) const;
    bool operator==(const BigInteger &other) const;

    static std::string check(const std::string &s);

protected:
    std::string integer = "";
};

BigInteger &BigInteger::operator=(std::string s) {
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

std::string BigInteger::check(const std::string &s) {
    if (s == "-1") return s;
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

// function in checking input
bool inputAndCheck(LinkedList<BigInteger> *l) {
    using namespace std;
    string s;

    if (l == nullptr) {
        cerr << "Invalid parameter LinkedList<BigInteger>* l" << endl;
        return false;
    }

    cin >> s;
    BigInteger bigInteger;
    BigInteger prevBI("0");
    while (s != "-1") {
        try {
            bigInteger = BigInteger::check(s);
        } catch (NaNException &e) {
            cerr << "NaN or negative number except for -1 "
                    "in sequence, abandoning...\n" << endl;
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            l->clear();
            return false;
        }
        if (bigInteger < prevBI) {
            cerr << "Not increasing sequence, abandoning..." << endl;
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            l->clear();
            return false;
        }
        prevBI = bigInteger;
        l->insertBack(bigInteger);
        cin >> s;
    }
    return true;
}


///----------------------- Intersect function -------------------------------///
LinkedList<BigInteger> *
        intersect(LinkedList<BigInteger> &a, LinkedList<BigInteger> &b) {
    auto *intersected = new LinkedList<BigInteger>();
    auto aCur = a.getCursor(), bCur = b.getCursor();
    while (aCur.current() != nullptr && bCur.current() != nullptr) {
        if (aCur.getElement() > bCur.getElement()) { bCur.next(); }
        else if (aCur.getElement() < bCur.getElement()) { aCur.next(); }
        else {
            intersected->insertBack(aCur.getElement());
            aCur.next();
            bCur.next();
        }
    }
    aCur.reset(); bCur.reset();
    return intersected;
}

///-------------------------- main function ---------------------------------///
int main() {
    using namespace std;
    LinkedList<BigInteger> a, b;
    LinkedList<BigInteger> *result;
    string goOn = "y";
    while (tolower(goOn[0]) == 'y') {
        cout << "Enter the sorted sequence A end with -1: "  << endl;
        while (!inputAndCheck(&a)) { cout << "Try again: " << endl; }
        cout << "Enter the sorted sequence B end with -1: "  << endl;
        while (!inputAndCheck(&b)) { cout << "Try again: " << endl; }
        result = intersect(a, b);
        if (result->length() == 0) {
            cout << "Empty set." << endl;
        } else {
            cout << "Intersection is: ";
            result->show();
        }

        a.clear(); b.clear(); result->clear();
        cout << "Continue? [Y/n]: ";
        cin >> goOn;
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    }
    return 0;
}

