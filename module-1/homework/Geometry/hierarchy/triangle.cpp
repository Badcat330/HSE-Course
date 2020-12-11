#include "triangle.h"

Triangle::Triangle(Point x, Point y, Point z) {
    points.push_back(x);
    points.push_back(y);
    points.push_back(z);
}

Triangle::Triangle(const Triangle &origin) {
    points = origin.points;
}

Triangle &Triangle::operator=(const Triangle &origin) {
    points = origin.points;
    return *this;
}

Circle Triangle::circumscribedCircle() {
    return Circle();
}

Circle Triangle::inscribedCircle() {
    return Circle();
}
