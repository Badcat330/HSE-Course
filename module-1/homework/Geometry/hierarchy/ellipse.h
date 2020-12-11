#include <utility>
#include <cmath>

#include "const.h"
#include "shape.h"

#ifndef GEOMETRY_ELLIPSE_H
#define GEOMETRY_ELLIPSE_H

class Ellipse: public Shape {
 public:
    Ellipse() = delete;
	Ellipse(std::pair<Point,Point> _focuses, double _focusSum);
	Ellipse(const Ellipse& origin);
	Ellipse& operator=(const Ellipse& origin);
	~Ellipse()=default;

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

	std::pair<Point,Point> getFocuses(); // фокусы эллипса
	std::pair<Line, Line> directrices(); // пара директрис эллипса
	double eccentricity() const; // эксцентриситет
	Point center() const; // центр эллипса

 protected:
	std::pair<Point,Point> focuses;
	double focusSum;
	double a;
	double b;
	double c;
};

#endif //GEOMETRY_ELLIPSE_H