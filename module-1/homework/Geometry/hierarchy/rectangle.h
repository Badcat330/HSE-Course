#include "polygon.h"

#ifndef GEOMETRY_RECTANGLE_H
#define GEOMETRY_RECTANGLE_H

class Rectangle : public Polygon {
public:
    Rectangle(std::pair<Point, Point> points, double attitude);

    Rectangle(const Rectangle& origin);

    Rectangle& operator=(const Rectangle& origin);

    ~Rectangle() = default;

    Point center();
    std::pair<Line, Line> diagonals();

protected:
    Rectangle() = default;
};

#endif //GEOMETRY_RECTANGLE_H