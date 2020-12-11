#include "circle.h"
#include "polygon.h"

class Triangle: protected Polygon {
	Triangle() = delete;
	Triangle(Point x, Point y, Point z);
	Triangle(const Triangle& origin);
	Triangle& operator=(const Triangle& origin);
	~Triangle() = default;


	Circle circumscribedCircle();
	Circle inscribedCircle();
};