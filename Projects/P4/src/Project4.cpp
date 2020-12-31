#include <cstring>
#include <string>
#include <iostream>
#include <exception>
#include <utility>
#include <algorithm>

#ifndef _NOEXCEPT
#define _NOEXCEPT noexcept
#endif

using std::string;

///-------------------------- BigInteger ------------------------------------///
class ParseError : public std::exception {
    const char *what() const _NOEXCEPT override {
        return "Cannot parse invalid integer.";
    }
};

class DivideByZero : public std::exception {
    const char *what() const _NOEXCEPT override {
        return "Cannot divide by zero";
    }
};

class BigInteger {
public:

    BigInteger() : value("0"), negative(false) {}
    explicit BigInteger(int i);
    explicit BigInteger(const string &s);
    BigInteger(const BigInteger &bi);
    ~BigInteger() = default;

    // Unary operator
    BigInteger operator-() const;
    BigInteger operator+() const;
    BigInteger absolute() const;

    std::ostream &show(std::ostream &os) const;
    int compare(const BigInteger &bi) const;

    // Binary operator
    BigInteger &operator=(const BigInteger &bi);
    BigInteger operator+(const BigInteger &bi) const;
    BigInteger operator-(const BigInteger &bi) const;
    BigInteger operator*(const BigInteger &bi) const;
    BigInteger operator/(const BigInteger &bi) const;
    BigInteger operator%(const BigInteger &bi) const;
    BigInteger operator^(const BigInteger &bi) const;
    bool operator==(const BigInteger &bi) const;

protected:
    string value;
    bool negative = false;

    int compareAbs(const BigInteger &bi) const;
};

BigInteger::BigInteger(int i) {
    if (i < 0) {
        value = std::to_string(-i);
        negative = true;
    } else {
        value = std::to_string(i);
        negative = false;
    }
}

BigInteger::BigInteger(const string &s) {
    negative = false;
    unsigned long begPos = 0;
    if (s.empty()) {
        value = "0";
        negative = false;
        return;
    }
    if (s[0] == '-') {
        negative = true;
        begPos = 1;
    }
    for (auto &c : s.substr(begPos)) {
        if (c > '9' || c < '0') {
            throw ParseError();
        }
    }
    value = s.substr(begPos);
}

BigInteger::BigInteger(const BigInteger &bi) {
    negative = bi.negative;
    value = bi.value;
}

BigInteger &BigInteger::operator=(const BigInteger &bi) {
    if (&bi == this)
        return *this;
    negative = bi.negative;
    value = bi.value;
    return *this;
}

int BigInteger::compare(const BigInteger &bi) const {
    if (negative && !bi.negative)
        return -1;
    if (bi.negative && !negative)
        return 1;

    int result = 0;
    if (value.length() < bi.value.length()) {
        result = -1;
    } else if (value.length() > bi.value.length()) {
        result = 1;
    } else {
        result = value.compare(bi.value);
    }
    if (negative)
        result = -result;
    return result;
}

BigInteger BigInteger::absolute() const {
    if (negative)
        return -*this;
    return *this;
}

int BigInteger::compareAbs(const BigInteger &bi) const {
    return this->absolute().compare(bi.absolute());
}

BigInteger BigInteger::operator+(const BigInteger &bi) const {
    using std::reverse;
    BigInteger result;
    string lhs = value, rhs = bi.value;
    reverse(lhs.begin(), lhs.end());
    reverse(rhs.begin(), rhs.end());
    if (this->negative == bi.negative) {
        // Adding
        result.value = "";
        unsigned long length = std::min(lhs.length(), rhs.length());
        int i = 0;
        for (; i < length; i++) {
            // Roughly adding each digit, not caring carrying.
            result.value.push_back(lhs[i] - '0' + rhs[i]);
        }

        if (length < lhs.length()) {
            result.value += lhs.substr(i);
        } else if (length < rhs.length()) {
            result.value += rhs.substr(i);
        }

        int carry = 0;
        for (auto &c : result.value) {
            int temp = c + carry - '0';
            carry = temp / 10;
            temp %= 10;
            c = temp + '0';
        }
        if (carry != 0) {
            result.value.push_back('1');
        }
        result.negative = negative;
    } else {
        // Subtracting

        int flag = compareAbs(bi);
        if (flag == -1) {
            return bi + *this;
        } else if (!flag) {
            return BigInteger("0");
        } else {
            result.value = "";
            unsigned long length = std::min(lhs.length(), rhs.length());
            int i = 0;
            for (; i < length; ++i) {
                // Roughly subtracting each digit, not caring carrying.
                result.value.push_back(lhs[i] - (rhs[i] - '0'));
            }

            if (i < lhs.length()) {
                result.value += lhs.substr(i);
            }
//            std::cout << result.value << std::endl;
            int carry = 0;
            for (auto &c : result.value) {
                int temp = c - carry - '0';
                carry = (temp < 0);
                temp = carry * 10 + temp;
                c = temp + '0';
            }
            result.negative = negative;
        }
    }
    reverse(result.value.begin(), result.value.end());
    auto valid = result.value.find_first_not_of('0');
    if (valid == string::npos) {
        return BigInteger("0");
    }
    result.value = result.value.substr(valid);
    return result;
}

std::ostream &BigInteger::show(std::ostream &os) const {
    if (negative)
        os << '-';
    os << value;
    return os;
}

BigInteger BigInteger::operator-(const BigInteger &bi) const {
    return (*this + (-bi));
}

BigInteger BigInteger::operator-() const {
    auto ret = BigInteger(*this);
    ret.negative = !negative;
    return ret;
}

BigInteger BigInteger::operator+() const {
    return *this;
}

BigInteger BigInteger::operator*(const BigInteger &bi) const {
    using std::reverse;
    if (this->value == "0" || bi.value == "0") {
        return BigInteger("0");
    }
    BigInteger result = BigInteger();
    if (this->negative != bi.negative) {
        result.negative = true;
    }
    string lhs = value, rhs = bi.value;
    reverse(lhs.begin(), lhs.end());
    reverse(rhs.begin(), rhs.end());
    result.value.resize(lhs.length() + rhs.length(), '0');

    // Loop for multiplying
    for (int i = 0; i < lhs.length(); i++) {
        for (int j = 0; j < rhs.length(); j++) {
            int temp = (result.value[i + j] - '0') +
                       (lhs[i] - '0') * (rhs[j] - '0');
            int carry = temp / 10;
            result.value[i + j] = temp % 10 + '0';
            // Adding carrying to every digit
            for (int higherDigits = i + j + 1;
                 carry != 0 && higherDigits < result.value.length();
                 higherDigits++) {
                int renewal = result.value[higherDigits] - '0' + carry;
                carry = renewal / 10;
                result.value[higherDigits] = renewal % 10 + '0';
            }
        }
    }

    reverse(result.value.begin(), result.value.end());
    result.value = result.value.substr(result.value.find_first_not_of('0'));
    return result;
}

// Throws DivideByZero exception
BigInteger BigInteger::operator/(const BigInteger &bi) const {
    if (bi.value == "0") {
        throw DivideByZero();
    }

    // Integer division, return 0 if |*this| < |bi|
    int cmp = this->compareAbs(bi);
    if (cmp == -1) {
        return BigInteger("0");
    }

    BigInteger result("0");
    if (cmp == 0) {
        result = BigInteger("1");
    } else {
        BigInteger absLhs(this->absolute()), absRhs(bi.absolute());
        const BigInteger kTen("10");
        for (int loopCount = int(value.length() - bi.value.length());
             loopCount >= 0;
             --loopCount) {
            BigInteger temp(absRhs);
            for (int i = 0; i < loopCount; ++i) {
                temp = temp * kTen;
            }

            int divider = 0;
            while (absLhs.compareAbs(temp) >= 0) {
                absLhs = absLhs - temp;
                ++divider;
            }
            result = result * kTen + BigInteger(divider);
        }
    }
    result.negative = negative ^ bi.negative;
    return result;
}

// Throws DivideByZero exception
BigInteger BigInteger::operator%(const BigInteger &bi) const {
    BigInteger divider = *this / bi;
    return ((*this - divider * bi).compareAbs(*this - (divider * bi - bi)) < 0) ? (*this - divider * bi) : (*this - (divider * bi - bi));
}

BigInteger BigInteger::operator^(const BigInteger &bi) const {
    BigInteger kZero("0");
    int powZero = bi.compare(kZero);
    int thisIsZero = compare(kZero);
    if (thisIsZero == 0) {
        if (powZero <= 0) {
            throw DivideByZero();
        } else {
            return BigInteger("0");
        }
    } else if (powZero == 0) {
        return BigInteger("1");
    } else if (powZero < 0) {
        std::cerr
                << "Warning: Integer to negative power always evaluate to zero\n";
        return BigInteger("0");
    }

    BigInteger result("1");
    BigInteger lhs = *this, rhs = bi;
    while (rhs.compare(kZero) > 0) {
        bool isOdd = (unsigned) (rhs.value.back() - '0') & 1u;
        if (isOdd) {
            result = result * lhs;
        }
        lhs = lhs * lhs;
        rhs = rhs / BigInteger("2");
    }
    return result;
}

bool BigInteger::operator==(const BigInteger &bi) const {
    return this->compare(bi) == 0;
}

std::ostream &operator<<(std::ostream &os, const BigInteger &bi) {
    bi.show(os);
    return os;
}



///------------------------ ExpressionExceptions ----------------------------///
class UnidentifiedToken : public std::exception {
public:
    explicit UnidentifiedToken(char o) : op(o) {}

    const char *what() const _NOEXCEPT override {
        des[0] = op;
        return des;
    }

private:
    char op;
    mutable char des[30] = {"x is an unidentified token."};
};

class ParenthesesNotMatch : public std::exception {
public:
    const char *what() const _NOEXCEPT override {
        return "Parentheses do not match";
    }
};

class SyntaxError : public std::exception {
public:
    const char *what() const _NOEXCEPT override {
        return "Expression has syntax error, please check.";
    }
};

class EmptyExpression : public std::exception {
public:
    const char *what() const _NOEXCEPT override {
        return "Expression is empty, please check.";
    }
};


///---------------------- ExpressionNode(abstract) --------------------------///
class ExpressionNode {
public:
    ExpressionNode() = default;
    virtual ~ExpressionNode() = default;

    // calculating
    virtual BigInteger evaluate() = 0;
    // visiting
    virtual void prefixVisit(std::ostream &os) = 0;
    virtual void postfixVisit(std::ostream &os) = 0;
};

///------------------------------ NumberNode --------------------------------///
class NumberNode : public ExpressionNode {
    // Tree leaves
public:
    explicit NumberNode(const BigInteger &bi) : num(bi) {}
    ~NumberNode() override = default;

    BigInteger evaluate() override;
    void prefixVisit(std::ostream &os) override;
    void postfixVisit(std::ostream &os) override;

private:
    BigInteger num;
};

BigInteger NumberNode::evaluate() {
    return num;
}

void NumberNode::prefixVisit(std::ostream &os) {
    os << num;
}

void NumberNode::postfixVisit(std::ostream &os) {
    os << num;
}

///------------------------------ OperationNode -----------------------------///
class OperationNode : public ExpressionNode {
    // Tree parents
public:
    OperationNode(char o, ExpressionNode *left, ExpressionNode *right)
            : op(o), leftChild(left), rightChild(right) { }
    ~OperationNode() override;

    BigInteger evaluate() override;
    void prefixVisit(std::ostream &os) override;
    void postfixVisit(std::ostream &os) override;

private:
    char op;
    ExpressionNode *leftChild;
    ExpressionNode *rightChild;
};

OperationNode::~OperationNode() {
    delete leftChild;
    delete rightChild;
}

BigInteger OperationNode::evaluate() {
    if (!leftChild || !rightChild)
        throw SyntaxError();
    switch (op) {
        case '+':
            return leftChild->evaluate() + rightChild->evaluate();
        case '-':
            return leftChild->evaluate() - rightChild->evaluate();
        case '*':
            return leftChild->evaluate() * rightChild->evaluate();
        case '/':
            return leftChild->evaluate() / rightChild->evaluate();
        case '%':
            return leftChild->evaluate() % rightChild->evaluate();
        case '^':
            return leftChild->evaluate() ^ rightChild->evaluate();
        default:
            throw UnidentifiedToken(op);
    }
}

void OperationNode::prefixVisit(std::ostream &os) {
    os << op << ' ';
    leftChild->prefixVisit(os);
    os << ' ';
    rightChild->prefixVisit(os);
}

void OperationNode::postfixVisit(std::ostream &os) {
    leftChild->postfixVisit(os);
    os << ' ';
    rightChild->postfixVisit(os);
    os << ' ' << op << ' ';
}


///--------------------------- ExpressionParser -----------------------------///
class ExpressionParser {
    class StringHandler { // handler for string
    public:
        explicit StringHandler(string expr)
                : expression(std::move(expr)), curPos(0),
                  curOp('\0') { next(); };

        void preprocess();
        void next();
        bool isOp()  { return status == Operator; }
        bool isNum() { return status == Number; }
        bool isEnd() { return status == End; }

    public:
        string expression;
        unsigned int curPos;
        char curOp;
        BigInteger curNum;
        enum Status {
            Number, Operator, End
        } status = Number;

    private:
        unsigned parseInteger();
    };

public:
    explicit ExpressionParser(string expr) : handler(std::move(expr)) {};
    ~ExpressionParser() = default;

    // parse
    ExpressionNode *parse(int precedence);
    ExpressionNode *unaryParse();

    // result
    void calculate();
    const BigInteger &getResult() { return result; }

    // visit
    void prefixTraverse(std::ostream &os);
    void postfixTraverse(std::ostream &os);

private:
    enum Precedence {
        AddPrecedence = 1,          // + -
        MulPrecedence = 2,          // * / %
        PowerPrecedence = 3,        // ^
        UnaryPrecedence = 4         // + - ()
    };
    enum Associativity {
        Left, Right
    };

    StringHandler handler;
    BigInteger result;
    ExpressionNode *tree = nullptr;

    // operation rules
    static Precedence precedenceOf(char op);
    static Associativity associativityOf(char op);
};


void ExpressionParser::StringHandler::next() {
    while (expression[curPos] == ' ') { ++curPos; }
    if (expression[curPos] != '\0') {
        char temp = expression[curPos];
        if ('0' <= temp && temp <= '9') {
            curPos = parseInteger();
            // curNum assigned here
            status = Number;
        } else if (std::strchr("+-*/%^()", temp) != nullptr) {
            curOp = temp;
            ++curPos;
            status = Operator;
        } else {
            throw UnidentifiedToken(temp);
        }
    } else {
        status = End;
    }
}

unsigned ExpressionParser::StringHandler::parseInteger() {
    if (expression[curPos] < '0' || expression[curPos] > '9') {
        throw ParseError();
    }
    unsigned temp = curPos;
    while (expression[temp] >= '0' && expression[temp] <= '9') {
        ++temp;
    }
    curNum = BigInteger(expression.substr(curPos, temp - curPos));
    return temp;
}

void ExpressionParser::StringHandler::preprocess() {
    if (expression[expression.length() - 1] == '=') {
        expression = expression.substr(0, expression.length() - 1);
    }
    if (expression.empty()) {
        throw EmptyExpression();
    }
    enum {
        cDigit, cParenthesis, cWhiteSpace, cOperator, cOpen, cClose
    } lastChar = cDigit, lastNonSpace = cDigit;
    expression = expression.substr(expression.find_first_not_of(' '));

    for (int i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        if (c >= '0' && c <= '9') {
            if (i != 0) {
                if ((lastChar == cWhiteSpace && lastNonSpace == cDigit)
                    || lastNonSpace == cClose) {
                    throw SyntaxError();
                }
                lastChar = cDigit;
                lastNonSpace = cDigit;
            }
        } else if (string("+-*/%^() ").find_first_of(c) == string::npos) {
            throw UnidentifiedToken(c);
        } else {
            if (lastNonSpace == cOperator && c != ' ' && c != '(' && c != '+' && c != '-') {
                std::cerr << c;
                throw SyntaxError();
            }
            if (c == ' ') {
                lastChar = cWhiteSpace;
            } else if (c == '(') {
                if (i != 0 &&
                    (lastNonSpace == cDigit || lastNonSpace == cClose)) {
                    auto len = expression.length();
                    expression = expression.substr(0, i) + "*" +
                                 expression.substr(i, len - i);
                    i++;
                }
                lastNonSpace = cOpen;
                lastChar = cOpen;
            } else if (c == ')') {
                lastNonSpace = cClose;
                lastChar = cClose;
            } else {
                lastChar = cOperator;
                lastNonSpace = cOperator;
            }
        }
    }
}

ExpressionParser::Precedence ExpressionParser::precedenceOf(char op) {
    switch (op) {
        case '+':
        case '-':
            return AddPrecedence;
        case '*':
        case '/':
        case '%':
            return MulPrecedence;
        case '^':
            return PowerPrecedence;
        default:
            return UnaryPrecedence;
    }
}

ExpressionParser::Associativity ExpressionParser::associativityOf(char op) {
    if (op == '^')
        return Right;
    return Left;
}

ExpressionNode *ExpressionParser::parse(int precedence) {
    if (precedence >= UnaryPrecedence) {
        return unaryParse();
    }
    auto leftOperand = parse(precedence + 1);
    if (handler.isEnd())
        return leftOperand;
    char op = handler.curOp;
    if (associativityOf(op) == Right) {
        handler.next();
        return new OperationNode(op, leftOperand, parse(precedence));
    } else {
        while (precedenceOf(op) == precedence && !handler.isEnd()) {
            handler.next();
            auto rightOperand = parse(precedence + 1);
            leftOperand = new OperationNode(op, leftOperand, rightOperand);
            op = handler.curOp;
        }
        return leftOperand;
    }
}

ExpressionNode *ExpressionParser::unaryParse() {
    ExpressionNode *ret = nullptr;
    if (handler.isOp() && handler.curOp == '(') {
        handler.next();
        ret = parse(AddPrecedence);
        if (handler.status != StringHandler::Operator || handler.curOp != ')') {
            throw ParenthesesNotMatch();
        }
        handler.next();
    } else {
        if (handler.isOp() && handler.curOp == '-') {
            handler.next();
            auto temp = unaryParse();
            ret = new NumberNode(-temp->evaluate());
            delete temp;
        } else if (handler.isOp() && handler.curOp == '+') {
            handler.next();
            ret = unaryParse();
        } else if (handler.isNum()) {
            ret = new NumberNode(handler.curNum);
            handler.next();
        }
    }
    return ret;
}

void ExpressionParser::calculate() {
    if (tree == nullptr) {
        handler.preprocess();
        tree = parse(AddPrecedence);
        if (!handler.isEnd()) {
            throw SyntaxError();
        }
        result = tree->evaluate();
    }
}

void ExpressionParser::prefixTraverse(std::ostream &os) {
    tree->prefixVisit(os);
    os << std::endl;
}

void ExpressionParser::postfixTraverse(std::ostream &os) {
    tree->postfixVisit(os);
    os << std::endl;
}


class TestCases {
public:
    typedef BigInteger BI;
    typedef ExpressionParser EP;
    void run();

    void compareResults();
    void exceptionTests();

private:
    static constexpr int testNum = 11;
    static constexpr int exceptionNum = 6;
    EP testCases[testNum] = {
            EP("((((1))))="),     // parentheses
            EP("1+1+1+1-1-1"),    // adding and subtracting
            EP("10*60/89*45"),    // multiply and division
            EP("10293^253"),      // power and big integer
            EP("8764 % 675"),     // modulo
            EP("5+8*8-98/9"),     // precedence - 1
            EP("87*43/3^2+98%3"), // precedence - 2
            EP("(8+6*(5+4)^3)%31"),// precedence -3
            EP("3^3^3"),          // right association power
            EP("8+-5--4++73"),    // unary operation
            EP("847(8374)"),
    };
    BI answers[testNum] = {
            BI("1"),
            BI("2"),
            BI("270"),
            BI(
                    "14897696150986096363009011212162797695162608511819030752"
                    "11806440825057750324616332439485272743494296640314300700"
                    "39448805991603931451937468282797320782058434387736855127"
                    "57073724895137660977961338908731226603289619530097422778"
                    "69512458338938282885141085524854874265230175995627903167"
                    "02652034519726734885953176718417342689740478980651996048"
                    "55570168443956526807671607224422059880751258271238404205"
                    "11464626756725387540836688795005772364466335573451998406"
                    "16828520693327494624108896631868659335338778610410844175"
                    "49613538076893432740552073038400330913467128698547422602"
                    "10764499280344481617042778029214835945963128187811183525"
                    "04678971393318095264089751334567675566169542295325178836"
                    "44430947569949146955649208131626365805901938237329290507"
                    "32490332617193045596880535080533913003970886481418112428"
                    "11936528333574974977676482207108627520293869945065862497"
                    "70324756070506078945425619883307736380063581093413049163"
                    "61897819730020823450123893546714786066727577880460185375"
                    "72724768780106970997730472201113147265497843645627249934"
                    "04807493"),
            BI("664"),
            BI("59"),
            BI("417"),
            BI("11"),
            BI("7625597484987"),
            BI("80"),
            BI("7092778")
    };
    EP exceptionCases[exceptionNum] = {
            EP("2837&4$5=1"),
            EP("4563%(1-1)"),
            EP("(837465+8384^5465"),
            EP("1+"),
            EP(""),
            EP("1 2 3"),
    };
    string expectedException[exceptionNum] = {
            "UnidentifiedToken",
            "DivideByZero",
            "ParenthesesNotMatch",
            "SyntaxError",
            "EmptyExpression",
            "SyntaxError"
    };
};

void TestCases::run() {
    compareResults();
    exceptionTests();
}

void TestCases::compareResults() {
    using std::cout;
    int correctNum = 0;
    for (int i = 0; i < testNum; ++i) {
        try {
            testCases[i].calculate();
            if (testCases[i].getResult() == answers[i]) {
                ++correctNum;
            }
        } catch (std::exception &e) {
            cout << "Bad exceptions appear in legal expressions.";
        }
    }
    cout << correctNum << " / " << testNum << " correctness. ";
    if (correctNum == testNum) {
        cout << "All correct!";
    }
    cout << '\n' << std::endl;
}

void TestCases::exceptionTests() {
    using std::cout;
    for (int i = 0; i < exceptionNum; ++i) {
        try {
            exceptionCases[i].calculate();
        } catch (std::exception &e) {
            cout << "Problem:  " << e.what() << "\n";
            cout << "Expected: " << expectedException[i] << std::endl;
        }
    }
}

//#define CASE_TEST

int main() {
    using std::cout;
    using std::cin;
#ifdef CASE_TEST
    TestCases tc;
    tc.run();
#else
    string expr = "000";
    cout << "Enter your expression with an enter/return marking the end.\n";
    std::getline(cin, expr);
    while (!expr.empty()) {
        try {
            ExpressionParser parser = ExpressionParser(expr);
            parser.calculate();
            cout << "Prefix traverse:  ";
            parser.prefixTraverse(cout);
            cout << "Postfix traverse: ";
            parser.postfixTraverse(cout);
            cout << "Result          : " << parser.getResult() << '\n' << std::endl;
            cout << "Enter your expression with an enter/return marking the end.\n";
        } catch (std::exception &e) {
            cout << "Error occur: " << e.what() << std::endl;
            cout << "Try again: \n";
        }
        std::getline(cin, expr);
    }
    cout << "Bye~\n";
#endif
    return 0;
}
