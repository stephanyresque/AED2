#include "Union.h"
#include <sstream>
#include <iostream>

Element* UnionManager::Find(Element* e) {
    if (e->parent != e)
        e->parent = Find(e->parent);
    return e->parent;
}

bool UnionManager::MakeSet(const std::string& x) {
    if (valueToElement.count(x)) {
        lastAction = "Não foi possível criar conjunto: valor " + x + " já pertence a um conjunto.";
        return false;
    }

    Element* newElem = new Element(x);
    auto newSet = std::make_unique<DisjointSet>(newElem);

    int id = nextSetId++;
    idToSet[id] = std::move(newSet);
    setIdMap[newElem] = id;
    valueToElement[x] = newElem;

    lastAction = "Conjunto S" + std::to_string(id) + " criado.";
    return true;
}

void UnionManager::ShowSet(const std::string& x) {
    if (!valueToElement.count(x)) {
        lastAction = "Elemento " + x + " não encontrado.";
        return;
    }
    Element* rep = Find(valueToElement[x]);
    int id = setIdMap[rep];
    ShowSet(id);
}

void UnionManager::ShowSet(int id) {
    if (!idToSet.count(id)) {
        lastAction = "Conjunto S" + std::to_string(id) + " não existe.";
        return;
    }

    std::ostringstream oss;
    for (Element* e : idToSet[id]->elements)
        oss << e->value << " ";

    lastAction = oss.str();
}

void UnionManager::DestroySet(const std::string& x) {
    if (!valueToElement.count(x)) {
        lastAction = "Conjunto com " + x + " não existe. Nada a fazer.";
        return;
    }

    Element* rep = Find(valueToElement[x]);
    int id = setIdMap[rep];
    DestroySet(id);
}

void UnionManager::DestroySet(int id) {
    if (!idToSet.count(id)) {
        lastAction = "Conjunto S" + std::to_string(id) + " não existe. Nada a fazer.";
        return;
    }

    for (Element* e : idToSet[id]->elements) {
        valueToElement.erase(e->value);
        setIdMap.erase(e);
        delete e;
    }

    idToSet.erase(id);
    lastAction = "Conjunto S" + std::to_string(id) + " destruído.";
}

void UnionManager::Union(const std::string& x, const std::string& y) {
    if (!valueToElement.count(x) || !valueToElement.count(y)) {
        lastAction = "Erro: um ou ambos os elementos não existem.";
        return;
    }

    Element* a = valueToElement[x];
    Element* b = valueToElement[y];

    Element* ra = Find(a);
    Element* rb = Find(b);

    if (ra == rb) {
        lastAction = "Os elementos já estão no mesmo conjunto.";
        return;
    }

    if (ra->rank < rb->rank) std::swap(ra, rb);
    rb->parent = ra;
    if (ra->rank == rb->rank) ra->rank++;

    int idA = setIdMap[ra];
    int idB = setIdMap[rb];
    auto* setA = idToSet[idA].get();
    auto* setB = idToSet[idB].get();

    for (Element* e : setB->elements) {
        setA->elements.insert(e);
        setIdMap[e] = idA;
    }

    idToSet.erase(idB);
    lastAction = "Conjuntos S" + std::to_string(idA) + " e S" + std::to_string(idB) + " unidos em S" + std::to_string(idA);
}

void UnionManager::FindSet(const std::string& x) {
    if (!valueToElement.count(x)) {
        lastAction = "Elemento " + x + " não encontrado.";
        return;
    }
    Element* rep = Find(valueToElement[x]);
    lastAction = "Representante de " + x + ": " + rep->value;
}

void UnionManager::SizeSet(const std::string& x) {
    if (!valueToElement.count(x)) {
        lastAction = "Elemento " + x + " não encontrado.";
        return;
    }
    Element* rep = Find(valueToElement[x]);
    int id = setIdMap[rep];
    lastAction = "Tamanho do conjunto contendo " + x + ": " + std::to_string(idToSet[id]->elements.size());
}

void UnionManager::SizeSetById(int id) {
    if (!idToSet.count(id)) {
        lastAction = "Conjunto S" + std::to_string(id) + " não existe.";
        return;
    }
    lastAction = "Tamanho do conjunto S" + std::to_string(id) + ": " + std::to_string(idToSet[id]->elements.size());
}
