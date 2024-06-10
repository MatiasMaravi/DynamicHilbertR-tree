#include"MBB.h"

MBB::MBB(){
    lowerLeft = Point();
    upperRight = Point();
}
MBB::MBB(const Point& lowerLeft, const Point& upperRight){
    this->lowerLeft = lowerLeft;
    this->upperRight = upperRight;
}
MBB::MBB(const MBB& other){
    this->lowerLeft = other.lowerLeft;
    this->upperRight = other.upperRight;
}
MBB::~MBB() = default;

NType MBB::intersectionArea(const MBB &other) const{
    NType aux = 0;
    NType lx = max(aux,min(upperRight.getX(), other.upperRight.getX()) - max(lowerLeft.getX(), other.lowerLeft.getX()));
    NType ly = max(aux,min(upperRight.getY(), other.upperRight.getY()) - max(lowerLeft.getY(), other.lowerLeft.getY()));
    return lx*ly;
}

bool MBB::intersects(const MBB &other) const{
    return this->intersectionArea(other) > 0;
}

void MBB::expand(const MBB &other){
    this->lowerLeft = Point(min(lowerLeft.getX(), other.lowerLeft.getX()), min(lowerLeft.getY(), other.lowerLeft.getY()));
    this->upperRight = Point(max(upperRight.getX(), other.upperRight.getX()), max(upperRight.getY(), other.upperRight.getY()));
}

bool MBB::contains(const Point &point) const{
    return lowerLeft.getX() <= point.getX() && point.getX() <= upperRight.getX() &&
           lowerLeft.getY() <= point.getY() && point.getY() <= upperRight.getY();
}

NType MBB::perimeter() const{
    NType x = upperRight.getX() - lowerLeft.getX();
    NType y = upperRight.getY() - lowerLeft.getY();
    return (x + y)*2;
}

NType MBB::area() const {
    return (upperRight.getX() - lowerLeft.getX()) * (upperRight.getY() - lowerLeft.getY());
}