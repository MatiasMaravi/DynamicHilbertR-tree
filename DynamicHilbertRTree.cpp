#include"DynamicHilbertRTree.h"
#include <algorithm>

void ordenar_puntos(std::vector<Point>& points, HilbertCurve hilbertCurve){
    std::sort(points.begin(), points.end(), [hilbertCurve](const Point& a, const Point& b) {
        return hilbertCurve.point2hIndex(a) < hilbertCurve.point2hIndex(b);
    });
}
void ordenar_nodos(std::vector<Node*>& nodes){
    std::sort(nodes.begin(), nodes.end(), [](const Node* a, const Node* b) {
        return a->LHV < b->LHV;
    });
}
int find_node_index(Node* node, Node* parent){
    for(int i = 0; i < parent->children.size(); i++){
        if(parent->children[i] == node)
            return i;
    }
    return -1;
}
int find_hermanito(Node* parent, int start, int maxChildren){
    for(int i = start-1; i >=0 ; i-- ){
        int tamanio = parent->children[i]->points.size();
        if(tamanio < maxChildren)return i;
    }
    return -1;
}
void split_root(Node* node, Node* newLeaf, Node*& root){
    Node* newRoot = new Node(INTERNAL);
    newRoot->children.push_back(node);
    newRoot->children.push_back(newLeaf);
    node->parent = newRoot;
    newLeaf->parent = newRoot;
    root = newRoot;
}
void actualizar_padre(Node* node, Node* n2){
    node->parent->children.push_back(n2);
    n2->parent = node->parent;
}
void ajustar_LHV_and_MBR_leaf(Node* node, HilbertCurve hilbertCurve){
    node->LHV = hilbertCurve.point2hIndex(node->points[node->points.size()-1]);
    node->mbr = MBB(node->points[0], node->points[0]);
    for(int i = 1; i < node->points.size(); i++){
        node->mbr.expand(MBB(node->points[i], node->points[i]));
    }
}
void ajustar_LHV_and_MBR_internal(Node* node){
    node->LHV = node->children[node->children.size()-1]->LHV;
    node->mbr = node->children[0]->mbr;
    for(int i = 1; i < node->children.size(); i++){
        node->mbr.expand(node->children[i]->mbr);
    }
}
bool DynamicHilbertRTree::compare_points(const Point& p1, const Point& p2){
    uint h1 = hilbertCurve.point2hIndex(p1);
    uint h2 = hilbertCurve.point2hIndex(p2);
    return  h1 < h2;
}
bool DynamicHilbertRTree::compare_nodes(const Node* n1, const Node* n2){
    return n1->LHV < n2->LHV;
}
void DynamicHilbertRTree::insert(const Point& p) {
    uint h = hilbertCurve.point2hIndex(p);
    insert(this->root, p, h);
}
void DynamicHilbertRTree::insert(Node* node, const Point& p, uint h) {
    Node* leaf = chooseLeaf(node, p, h);
    std::set<Node*> S;
    if(leaf->points.size() < maxChildren)
        leaf->points.push_back(p);
    else
        handleOverflow(leaf, p);

    if(leaf->parent){
        for(auto child:leaf->parent->children){
            S.insert(child);
        }
    }else
        S.insert(leaf);

    adjustTree(S);
}

Node* DynamicHilbertRTree::chooseLeaf(Node* node, const Point& p, uint h) {
    Node* N = node;
    if(N->isLeaf) return N;
    for(auto child:N->children){
        if(child->LHV > h) return chooseLeaf(child, p, h);
    }
    return chooseLeaf(N->children[N->children.size()-1], p, h);
}
bool DynamicHilbertRTree::ayuda_hermanos(Node* node, const Point& p){
    if(!node->parent) return false;//No tiene padre, por lo tanto no tiene hermanos

    Node* parent = node->parent;
    int node_index = find_node_index(node, parent);
    if(node_index == 0 || node_index == -1) return false;//No tiene hermanos izquierdos

    int index_hermano = find_hermanito(parent, node_index, maxChildren);
    if(index_hermano == -1) return false;//Hermanos no pueden ayudar
    Point aux = p; 
    while(node_index >= 0){
        parent->children[node_index]->points.push_back(aux);
        ordenar_puntos(parent->children[node_index]->points, hilbertCurve);
        if(node_index == index_hermano) break;

        aux = parent->children[node_index]->points[0];
        parent->children[node_index]->points.erase(parent->children[node_index]->points.begin());
        node_index--;
    }
    return true;//Me ayudaron mis hermanos
}
void DynamicHilbertRTree::handle_leaf_overflow(Node* node, const Point& p){
    Node* newLeaf = new Node(LEAF);
    std::vector<Point> epsilon = node->points;
    node->points.erase(node->points.begin(), node->points.end());
    epsilon.push_back(p);
    //Llenamos con la mitad+1 el primer nodo
    ordenar_puntos(epsilon, hilbertCurve);
    uint m = maxChildren/2;
    for(int i = 0; i < m+1; i++){
        node->points.push_back(epsilon[i]);
    }
    for(int i = m+1; i < epsilon.size(); i++){
        newLeaf->points.push_back(epsilon[i]);
    }

    (node->parent)?actualizar_padre(node, newLeaf):split_root(node, newLeaf, this->root);

    if(node->parent->children.size() > maxChildren){
        std::set<Node*> S = {node, newLeaf};
        adjustTree(S);
        handle_internal_overflow(node->parent);
    }
}
void DynamicHilbertRTree::handleOverflow(Node* node, const Point& p){
    if(!ayuda_hermanos(node, p))
        handle_leaf_overflow(node, p);
}
void DynamicHilbertRTree::handle_internal_overflow(Node* node){
    std::vector<Node*> epsilon = node->children;

    node->children.erase(node->children.begin(), node->children.end());
    ordenar_nodos(epsilon);

    uint m = maxChildren/2;
    Node* new_node = new Node(INTERNAL);
    for(int i = 0; i < m+1; i++){
        node->children.push_back(epsilon[i]);
        epsilon[i]->parent = node;
    }
    for(int i = m+1; i < epsilon.size(); i++){
        new_node->children.push_back(epsilon[i]);
        epsilon[i]->parent = new_node;
    }

    (node->parent)?actualizar_padre(node, new_node):split_root(node, new_node, this->root);

    std::set<Node*> S = {node, new_node};
    adjustTree(S);

    if(node->parent->children.size() > maxChildren)
        handle_internal_overflow(node->parent);
}
void DynamicHilbertRTree::adjustLeaf(Node* node){
    ordenar_puntos(node->points, hilbertCurve);
    ajustar_LHV_and_MBR_leaf(node, hilbertCurve); 
}
void DynamicHilbertRTree::adjustInternal(Node* node){
    ordenar_nodos(node->children);
    ajustar_LHV_and_MBR_internal(node);
}
void DynamicHilbertRTree::adjustTree(std::set<Node*>& S){
    while(!S.empty()){
        Node* node = *S.begin();
        (node->isLeaf)?adjustLeaf(node):adjustInternal(node);
        if(node->parent) S.insert(node->parent);
        S.erase(node);
    }
}
bool DynamicHilbertRTree::search(const Point& p) const {
    return search(root, p);
}
bool DynamicHilbertRTree::search(Node* node, const Point& p) const {
    if(node->isLeaf){
        for(auto point:node->points){
            if(point == p) return true;
        }
    }else{
        uint h = hilbertCurve.point2hIndex(p);
        for(auto child:node->children){
            if(child->LHV >= h) return search(child, p);
        }
    }
    return false;
}
void DynamicHilbertRTree::print(Node* node) const {
    if(node->isLeaf){
        std::cout << "Leaf Node" << std::endl;
        for(auto point:node->points){
            std::cout << point << " " << hilbertCurve.point2hIndex(point) << std::endl;
        }
    }else{
        std::cout << "Internal Node" << std::endl;
        for(auto child:node->children){
            print(child);
        }
    }
}
void DynamicHilbertRTree::print() const {
    print(root);
}