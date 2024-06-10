#include "HilbertCurve.h"

void HilbertRotate(uint n, uint& x, uint& y, uint rx, uint ry){
    if (ry == 0){
        if (rx == 1){
            x = (n - 1) - x;
            y = (n - 1) - y;
        }
        std::swap(x, y);
    }
}

//Order 1 = 2^1 = 2x2 grid
//Order 2 = 2^2 = 4x4 grid
//Order 3 = 2^3 = 8x8 grid
uint HilbertCurve::point2hIndex(const Point& p)const {
    uint x = p.getX().getValue();
    uint y = p.getY().getValue();
    uint n = 1 << order;
    uint rx, ry, s, h=0;
    for (s = n/2; s > 0; s /= 2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        h += s * s * ((3 * rx) ^ ry);
        HilbertRotate(s, x, y, rx, ry);
    }
    return h;
}

Point HilbertCurve::hIndex2Point(uint index) const{
    uint n = 1 << order;
    uint rx, ry, s, t = index;
    uint x,y;
    x = y = 0;
    for (s = 1; s < n; s *= 2){
        rx = 1 & (t / 2);
        ry = 1 & (t ^ rx);
        HilbertRotate(s, x, y, rx, ry);
        x += s * rx;
        y += s * ry;
        t /= 4;
    }
    return Point(x, y);
}