#include <utility>

#include "ellipse.h"

#ifndef GEOMETRY_CIRCLE_H
#define GEOMETRY_CIRCLE_H

class Circle : protected Ellipse {
public:
    Circle() = delete;

    Circle(Point center, double radius);

    Circle(const Circle &origin);

    Circle &operator=(const Circle &origin);

    ~Circle() = default;

    double radius();
};

#endif //GEOMETRY_CIRCLE_H