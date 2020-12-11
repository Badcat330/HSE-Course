#include "rectangle.h"
#include "circle.h"

#ifndef GEOMETRY_SQUARE_H
#define GEOMETRY_SQUARE_H

class Square: protected Rectangle {
public:
	Square() = delete;
	Square(std::pair<Point, Point> points);
	Square(const Square& origin);
	Square& operator=(const Square& origin);
	~Square() = default;

	Circle circumscribedCircle();
	Circle inscribedCircle();
};

#endif