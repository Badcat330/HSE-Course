class Square: protected Rectangle {
	Square() = delete;
	Square(std::pair<Point, Point> points);
	Square(const Square& origin);
	Square& operator=(const Square& origin);
	~Square();

	Circle circumscribedCircle();
	Circle inscribedCircle();
};