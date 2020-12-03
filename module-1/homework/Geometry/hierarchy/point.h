struct Point {

	friend bool operator==(const Point& a, const Point& b);
	friend bool operator!=(const Point& a, const Point& b);

	double distance(Point p) {
		return  std::sqrt(std::pow(p.x - x, 2) +  std::pow(p.y - y, 2));
	}

	double x;
	double y;
};