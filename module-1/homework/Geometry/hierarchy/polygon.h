#include "shape.h"

class Polygon: public Shape {
 public:
	Polygon() = delete;
	Polygon(std::vector<Point>& points);
	Polygon(const Polygon& origin);
	Polygon& operator=(const Polygon& origin);
	~Polygon();

	double area() override;
	double perimeter() override;
	bool operator==(const Shape& another) override; // TODO проверить корректность работы
	bool isCongruentTo(const Shape &another) override;
	bool isSimilarTo(const Shape &another) override;
	bool containsPoint(Point point) override;
	void rotate(Point center, double angle) override;
	void reflex(Point center) override;
	void reflex(Line axis) override;
	void scale(Point center, double coefficient) override;

	size_t verticesCount(); // число вершин
	std::vector<Point> getVertices(); // вершины многоугольника
	bool isConvex(); // метод проверяющий "являться выпуклым"

 protected:
	std::vector<Point> points;
};