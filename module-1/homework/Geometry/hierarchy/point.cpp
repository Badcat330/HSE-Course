#include <cmath>
#include "point.h"

bool operator==(const Point &a, const Point &b) {
    return fabs(a.x - b.x) < Consts::EPS && fabs(a.y - b.y) < Consts::EPS;
}

bool operator!=(const Point &a, const Point &b) {
    return !(a == b);
}

double Point::distance(Point p) const {
    return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
}

Point::Point(double _x, double _y) : x(_x), y(_y) {

}
