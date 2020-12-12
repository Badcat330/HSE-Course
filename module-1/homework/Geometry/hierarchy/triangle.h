#include "circle.h"
#include "polygon.h"
#include "point.h"

#ifndef GEOMETRY_TRIANGLE_H
#define GEOMETRY_TRIANGLE_H

class Triangle: public Polygon {
public:
    Triangle() = delete;
	Triangle(Point x, Point y, Point z);
	Triangle(const Triangle& origin);
	Triangle& operator=(const Triangle& origin);
	~Triangle() = default;


	Circle circumscribedCircle();
	Circle inscribedCircle();
};

#endif //GEOMETRY_TRIANGLE_H