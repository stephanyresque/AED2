#ifndef UNION_H
#define UNION_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

struct Element {
    std::string value;
    Element* parent;
    int rank;

    Element(const std::string& val) : value(val), parent(this), rank(0) {}
};

class DisjointSet {
public:
    std::unordered_set<Element*> elements;
    Element* representative;

    DisjointSet(Element* e) : representative(e) {
        elements.insert(e);
    }
};

class UnionManager {
private:
    std::unordered_map<std::string, Element*> valueToElement;
    std::unordered_map<int, std::unique_ptr<DisjointSet>> idToSet;
    std::unordered_map<Element*, int> setIdMap;
    int nextSetId = 1;

    Element* Find(Element* e);
    std::string lastAction = "Nenhuma";

public:
    const std::string& GetLastAction() const { return lastAction; }

    bool MakeSet(const std::string& x);
    void ShowSet(const std::string& x);
    void ShowSet(int id);
    void DestroySet(const std::string& x);
    void DestroySet(int id);
    void Union(const std::string& x, const std::string& y);
    void FindSet(const std::string& x);
    void SizeSet(const std::string& x);
    void SizeSetById(int id);
};

#endif // UNION_H
