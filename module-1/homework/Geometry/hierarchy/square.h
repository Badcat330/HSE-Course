#include "rectangle.h"
#include "circle.h"

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