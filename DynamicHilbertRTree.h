#ifndef DYNAMIC_HILBERT_RTREE_H
#define DYNAMIC_HILBERT_RTREE_H

#include <vector>
#include <set>
#include <iostream>
#include "DataType.h"
#include "Point.h"
#include "MBB.h"
#include "HilbertCurve.h"
#include <set>
#define LEAF true
#define INTERNAL false
struct Node {
    std::vector<Node*> children;    // Solo si es nodo interno
    std::vector<Point>   points;    // Solo si es una hoja
    uint                    LHV;    // Largest Hilbert Value
    MBB                     mbr;    // Minimum Bounding Boxes de los hijos
    Node* parent;
    bool isLeaf;

    Node(bool leaf) : isLeaf(leaf), parent(nullptr), LHV(0) {}
};

class DynamicHilbertRTree {
    Node* root;
    uint maxChildren;
    HilbertCurve hilbertCurve;

    bool search(Node* node, const Point& p) const;
    void insert(Node* node, const Point& p, uint h);

    // Métodos auxiliares para la inserción
    void handle_internal_overflow(Node* node);
    void handle_leaf_overflow(Node* node, const Point& p);
    Node* chooseLeaf(Node* node, const Point& p, uint h);   // Elige la hoja adecuada para insertar un nuevo punto
    void handleOverflow(Node* node, const Point& p);       // Maneja el desbordamiento de un nodo.
    void adjustTree(std::set<Node*>& S);                 // Ajusta el árbol después de actulizar los nodos
    void adjustLeaf(Node* node);
    void adjustInternal(Node* node);
    bool compare_points(const Point& p1, const Point& p2);
    bool compare_nodes(const Node* n1, const Node* n2);
    void print(Node* node) const;
    bool ayuda_hermanos(Node* node, const Point& p);
public:

    DynamicHilbertRTree(uint maxChildren, int order, const Rect& region): maxChildren(maxChildren), hilbertCurve(order, region) {
        root = new Node(LEAF);
    }

    Node* getRoot() const { return root; }
    uint getMaxChildren() const { return maxChildren; }
    HilbertCurve getHilbertCurve() const { return hilbertCurve; }

    void insert(const Point& p);
    bool search(const Point& p) const;
    void print() const;
};

#endif // DYNAMIC_HILBERT_RTREE_H