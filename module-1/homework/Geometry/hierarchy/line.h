#include"point.h"

#ifndef GEOMETRY_LINE_H
#define GEOMETRY_LINE_H

struct Line {

    Line(Point _a, Point _b);

    friend bool operator==(const Line &a, const Line &b);

    friend bool operator!=(const Line &a, const Line &b);

    Point a;
    Point b;
};

#endif //GEOMETRY_LINE_H