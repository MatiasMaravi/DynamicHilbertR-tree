#ifndef HILBERT_CURVE_H
#define HILBERT_CURVE_H

#include "Point.h"
#include "MBB.h"

class HilbertCurve {
    uint order;
    Rect region;

public:
    HilbertCurve(int order, const Rect& region): order(order), region(region) {};
    uint  point2hIndex(const Point& p) const;
    Point hIndex2Point(uint index) const;

};

#endif // HILBERT_CURVE_H