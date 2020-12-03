#include <vector>
#include "polygon.h"

Polygon::Polygon(std::vector<Point>& _points): points(_points) {
}

Polygon::Polygon(const Polygon& origin) {
	points = origin.points;
}

Polygon& Polygon::operator=(const Polygon& origin) {
	points = origin.points;
	return *this;
}

Polygon::~Polygon() {
	points.clear();
}

double Polygon::perimeter() {
	double perimeter = 0;

	for(size_t i = 0; i < points.size() - 1; i++){
		perimeter += points[i].distance(points[i+1]);
	}

	perimeter += points[points.size() - 1].distance(points[0]);
	return perimeter;
}

size_t Polygon::verticesCount() {
	return points.size();
}

std::vector<Point> Polygon::getVertices() {
	return points;
}


