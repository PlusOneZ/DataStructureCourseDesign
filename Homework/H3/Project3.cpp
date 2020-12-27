#include <iostream>
#include <exception>


///------------------------------- Stack ------------------------------------///
class PeekingEmptyError : public std::exception {
    const char *what() const _NOEXCEPT override {
        return "Cannot peek empty stack";
    };
};

template<class Type>
class Stack {
    struct Node {
        Node *next = nullptr;
        Type data;

        explicit Node(const Type &d) : data(d) {}
        Node() = default;
    };

public:
    Stack();
    ~Stack();

    const Type &peek() const;
    bool pop();
    void push(const Type &d);

    void clear();
    bool isEmpty() const;

    bool show(std::ostream &os, const std::string &sep) const;

protected:
    Node *head = nullptr;
};

template<class Type>
Stack<Type>::Stack() {
    head = new Node();
    head->next = nullptr;
}

template<class Type>
Stack<Type>::~Stack() {
    Node *p = head;
    Node *q;
    while (p != nullptr) {
        q = p->next;
        delete p;
        p = q;
    }
}

template<class Type>
void Stack<Type>::clear() {
    if (isEmpty()) return;
    Node *p = head->next;
    Node *q;
    while (p->next != nullptr) {
        q = p->next;
        delete p;
        p = q;
    }
    head = p;
}

template<class Type>
bool Stack<Type>::isEmpty() const {
    return head->next == nullptr;
}

template<class Type>
const Type &Stack<Type>::peek() const {
    if (isEmpty()) {
        throw PeekingEmptyError();
    }
    return head->next->data;
}

template<class Type>
bool Stack<Type>::pop() {
    if (isEmpty()) {
        return false;
    }
    Node *p = head->next;
    head->next = p->next;
    delete p;
    return true;
}

template<class Type>
void Stack<Type>::push(const Type &d) {
    auto nNode = new Node(d);
    nNode->next = head->next;
    head->next = nNode;
}

template<class Type>
bool Stack<Type>::show(std::ostream &os, const std::string &sep) const {
    auto p = head;
    if (p->next == nullptr)
        return false;
    p = p->next;
    int counter = 1;
    while (p->next != nullptr) {
        os << p->data << " " << sep << " ";
        p = p->next;
        if (counter++ % 5 == 0) {
            os << '\n';
        }
    }
    os << p->data << std::endl;
    return true;
}


///---------------------------------- Maze ----------------------------------///

class MazeOutOfRangeError : public std::exception {
    const char *what() const _NOEXCEPT override {
        return "Index exceeding maze's range";
    };
};

class Maze {
public:
    class Position {
        int coordinate[2] = {0, 0};
    public:
        Position(int x, int y) {
            coordinate[0] = x;
            coordinate[1] = y;
        }
        Position() = default;

        std::ostream &show(std::ostream &os) {
            os << "<" << coordinate[0] << "," << coordinate[1] << ">";
            return os;
        }
        int x() const { return coordinate[0]; }
        int y() const { return coordinate[1]; }
    };

    Maze() : start(1, 16) { }

    void run() {
        clearAllTraversed();
        showMaze();
        route.clear();
        bool gotOut = DeepFirstSearch(start);
        if (gotOut) {
            std::cout << "The route is: \n";
            route.show(std::cout, "-->");
            modifyMaze();
            showMaze();
        } else {
            std::cout << "No way out!\n";
        }
    }

protected:
    static constexpr int width = 100;
    static constexpr int height = 100;

    char maze[height][width] = {
            "####################",
            "#1#..#.#..#.#..#0..#",
            "#.#.#...#...#.####.#",
            "#...###.##.#..##.#.#",
            "###...#....##....#.#",
            "#...######..#...#..#",
            "#.###......#..##..##",
            "#..##.#####..##..###",
            "##..#..####.##..####",
            "###...####.....##..#",
            "#.##.####..###..##.#",
            "#.##....##..##.##..#",
            "#..####....###..#..#",
            "##......#######...##",
            "####################",
    };
    enum Token { Unvisited, ToBeMarked, BadWay };
    Token traversed[height][width] = { Unvisited };

    Position start;
    Stack<Position> route;
    Stack<int> routeSequence;
    int directions[4][2] = {{-1, 0},
                            {1,  0},
                            {0,  1},
                            {0,  -1}
    };

    void showMaze() {
        using namespace std;
        cout << endl;
        for (auto & i : maze) {
            if (i[0] != '\0')
                cout << i << endl;
        }
        cout << endl;
    }


    void modifyMaze() {
        while (!route.isEmpty()) {
            getPos(route.peek()) = ' ';
            route.pop();
        }
    }

    char &getPos(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw MazeOutOfRangeError();
        }
        return maze[x][y];
    }

    char &getPos(const Position &pos) {
        return getPos(pos.x(), pos.y());
    }

    void clearAllTraversed() {
        for (auto &i : traversed) {
            for (auto &j : i) {
                j = Unvisited;
            }
        }
    }

    bool checkValid(const Position &pos) {
        char c;
        try {
            c = getPos(pos);
        } catch (MazeOutOfRangeError &e) {
            return false;
        }
        return !(c == '#' || traversed[pos.x()][pos.y()] != Unvisited);
    }

    bool isDestination(const Position &pos) {
        return (getPos(pos) == '1');
    }

    bool DeepFirstSearch(const Position &position) {
        Position curPos = start;
        if (checkValid(curPos)) {
            route.push(curPos);
        } else {
            return false;
        }
        bool canForward;
        while (true) {
            if (isDestination(curPos)) {
                break;
            }
            canForward = false;
            for (const auto &d : directions) {
                int x_new = curPos.x() + d[0];
                int y_new = curPos.y() + d[1];

                if (checkValid(Position(x_new, y_new))) {
                    canForward = true;
                    traversed[curPos.x()][curPos.y()] = ToBeMarked;
                    curPos = Position(x_new, y_new);
                    route.push(curPos);
                    break;
                }
            }
            if (!canForward) {
                traversed[curPos.x()][curPos.y()] = BadWay;
                route.pop();
                if (route.isEmpty()) {
                    return false;
                }
                curPos = route.peek();
            }
        }
        return !route.isEmpty();
    }
};

std::ostream &operator<<(std::ostream &os, Maze::Position &p) {
    p.show(os);
    return os;
}

int main() {
    Maze m;
    m.run();
}