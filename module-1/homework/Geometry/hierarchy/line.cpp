#include "line.h"

bool operator==(const Line &a, const Line &b) {
    return (a.a == b.a) && (a.b == b.b);
}

bool operator!=(const Line &a, const Line &b) {
    return !(a == b);
}

Line::Line(Point _a, Point _b) : a(_a), b(_b) {}
