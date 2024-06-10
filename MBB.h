#ifndef MBB_H
#define MBB_H

#include "Point.h"

struct MBB {
    Point lowerLeft; 
    Point upperRight; 

    MBB();
    MBB(const MBB& other);
    MBB(const Point& lowerLeft, const Point& upperRight);
    ~MBB();

    bool intersects(const MBB& other) const;
    NType intersectionArea(const MBB& other) const;
    void expand(const MBB& other);
    bool contains(const Point& point) const;
    NType perimeter() const;
    NType area() const;
};


class QueryBox {
    MBB mbb;

public:
    QueryBox(const Point& lowerLeft, const Point& upperRight) : mbb(lowerLeft, upperRight) {}
    ~QueryBox() = default;

    bool intersects(const MBB& other) const {
        return mbb.intersects(other);
    }
    bool contains(const Point& point) const {
        return mbb.contains(point);
    }
    const MBB& getMBB() const {
        return mbb;
    }
};


struct Rect {
    Point lowerLeft;
    Point upperRight;

    Rect(const Point& ll, const Point& ur) : lowerLeft(ll), upperRight(ur) {}
};

#endif // MBB_H