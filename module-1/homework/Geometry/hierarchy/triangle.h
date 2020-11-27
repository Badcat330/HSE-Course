#include "circle.h"

class Triangle: protected Polygon {
	Triangle() = delete;
	Triangle(Point x, Point y, Point z);
	Triangle(const Triangle& origin);
	Triangle& operator=(const Triangle& origin);
	~Triangle();


	Circle circumscribedCircle();
	Circle inscribedCircle();
};