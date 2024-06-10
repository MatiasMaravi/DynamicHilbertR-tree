#ifndef POINT_H
#define POINT_H

#include "DataType.h"
#include <iostream>

class Point{
    NType x;
    NType y;

public:
    Point(): x(0), y(0) {}
    Point(NType x, NType y): x(x), y(y) {}
    ~Point() = default;

    NType getX() const { return x; }
    NType getY() const { return y; }

    void setX(NType x) { this->x = x; }
    void setY(NType y) { this->y = y; }

    NType distance(const Point& p) const {
        return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
    }

    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point& p) const {
        return !(*this == p);
    }

    Point operator-(const Point& p) const {
        return Point(x - p.x, y - p.y);
    }
    Point operator+(const Point& p) const {
        return Point(x + p.x, y + p.y);
    }
    bool operator<(const Point& other) const {
        // Aquí puedes definir cómo se deben comparar dos puntos.
        // Por ejemplo, podrías comparar primero por la coordenada x, y luego por la y:
        if (x < other.x) {
            return true;
        }
        if (x == other.x && y < other.y) {
            return true;
        }
        return false;
    }

    // Imprimir
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x.getValue() << "," << p.y.getValue() << ")";
        return os;
    }
};

#endif // POINT_H
