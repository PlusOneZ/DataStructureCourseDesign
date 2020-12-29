#include <iostream>
#include <string>
#include <exception>
#include <_regex.h>
#include "../../MyDS/Vector.h"
#include "../../MyDS/HashMap.h"
#include "../../MyDS/Queue.h"

#ifndef _NOEXCEPT
#define _NOEXCEPT noexcept
#endif

using std::string;

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
        for (int i = 0; i < adjacents.length(); ++i) {
            if (adjacents[i].tag == t) {
                return adjacents[i];
            }
        }
        throw GraphException("Adjacency not found");
    }

    size_t index = 0;
    int inDegree = 0;
    Tag tag;
    Vector<Vertex *> adjacents;
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

private:
    size_t numVertex = 0;
    Vector<Vertex<VTag> *> vertexes;
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
        vertexes[u]->adjacents.pushBack(vertexes[v]);
        vertexes[v]->adjacents.pushBack(vertexes[u]);
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

struct Course {
    string id = "0";
    string name = "null";
    int hours = 0;
    int semester = 0;
    bool havePre = false;
    Vector<string> pres;

    Course() = default;

    Course(const string& i, const string& n, int h, int s, bool hasPre) :
            id(i), name(n), hours(h), semester(s), havePre(hasPre) { };
};

class CourseTable {
public:
    const int fieldLen = 12;
    int courseNum;
    static constexpr int Days = 5;
    static constexpr int CoursePerDay = 10;
    string arrangement[Days][CoursePerDay];

    inline std::ostream &show(std::ostream & os);
};

std::ostream &CourseTable::show(std::ostream &os) {
    string result;
    os << "| Mon        | Tue        | Wed        | Thur       | Fri        |\n";
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
        result += '\n';
    }
    os << result;
    return os;
}

class ScheduleSystem {
public:

};