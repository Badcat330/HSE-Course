#include <vector>

#include "shape.h"
#include "const.h"

#ifndef GEOMETRY_POLYGON_H
#define GEOMETRY_POLYGON_H

class Polygon: public Shape {
 public:
	explicit Polygon(std::vector<Point>& points);
	Polygon(const Polygon& origin);
	Polygon& operator=(const Polygon& origin);
	~Polygon();

	double area() const override;
	double perimeter() const override;
	bool operator==(const Shape& another) const override;
	bool isCongruentTo(const Shape &another) const override;
	bool isSimilarTo(const Shape &another) const override;
	bool containsPoint(Point point) const override;
	void rotate(Point center, double angle) override;
	void reflex(Point center) override;
	void reflex(Line axis) override;
	void scale(Point center, double coefficient) override;

	size_t verticesCount() const; // число вершин
	std::vector<Point> getVertices() const; // вершины многоугольника
	bool isConvex() const; // метод проверяющий "являться выпуклым"

 protected:
    Polygon() = default;
	std::vector<Point> points;
};

#endif //GEOMETRY_POLYGON_H