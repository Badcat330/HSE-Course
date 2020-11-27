
class Rectangle: protected Polygon {
	Rectangle() = delete;
	Rectangle(std::pair<Point, Point> points, double attitude);
	Rectangle(const Rectangle& origin);
	Rectangle& operator=(const Rectangle& origin);
	~Rectangle();

	Point center(); // центр круга
	std::pair<Line, Line> diagonals(); // пара диагоналей
};