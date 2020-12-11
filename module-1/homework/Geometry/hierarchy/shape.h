#include "point.h"
#include "line.h"

#ifndef GEOMETRY_SHAPE_H
#define GEOMETRY_SHAPE_H

class Shape {
 public:
	// периметр фигуры
	virtual double perimeter() const = 0;
	// площадь фигуры
	virtual double area() const = 0;
	// поточечное сравнение фигур
	virtual bool operator==(const Shape& another) const = 0;
	// метод проверяющий "данная фигура изометрична another"
	virtual bool isCongruentTo(const Shape& another) const = 0;
	// метод проверяющий "данная фигура поддобна another"
	virtual bool isSimilarTo(const Shape& another) const = 0;
	// метод проверяющий point находится внутри данной фигуры"
	virtual bool containsPoint(Point point) const = 0;
	// поворот данной фигуры на angle (градусы) относительно точки против часовой
	virtual void rotate(Point center, double angle) = 0;
	// симметричное отражение данной фигуры относительно center"
	virtual void reflex(Point center) = 0;
	// симметричное отражение данной фигуры относительно axis"
	virtual void reflex(Line axis) = 0;
	// гомотетия данной фигуры с центром в center и коэффициентом coefficient
	virtual void scale(Point center, double coefficient) = 0;
};

#endif //GEOMETRY_SHAPE_H