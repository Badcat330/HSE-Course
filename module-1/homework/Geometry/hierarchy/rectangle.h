#include "polygon.h"

class Rectangle: protected Polygon {
	Rectangle() = default;
	Rectangle(std::pair<Point, Point> points, double attitude);
	Rectangle(const Rectangle& origin);
	Rectangle& operator=(const Rectangle& origin);
	~Rectangle() = default;

	Point center(); // центр круга
	std::pair<Line, Line> diagonals(); // пара диагоналей
};