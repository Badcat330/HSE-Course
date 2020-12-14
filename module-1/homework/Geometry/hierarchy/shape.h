#include "point.h"
#include "line.h"

#ifndef GEOMETRY_SHAPE_H
#define GEOMETRY_SHAPE_H

class Shape {
public:

    virtual double perimeter() const = 0;

    virtual double area() const = 0;

    // поточечное сравнение фигур
    virtual bool operator==(const Shape& another) const = 0;

    virtual bool isCongruentTo(const Shape& another) const = 0;

    virtual bool isSimilarTo(const Shape& another) const = 0;

    virtual bool containsPoint(Point point) const = 0;

    virtual void rotate(Point center, double angle) = 0;

    virtual void reflex(Point center) = 0;

    virtual void reflex(Line axis) = 0;

    virtual void scale(Point center, double coefficient) = 0;
};

#endif //GEOMETRY_SHAPE_H