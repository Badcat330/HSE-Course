struct Point {

    Point(double _x, double _y);

	friend bool operator==(const Point& a, const Point& b);
	friend bool operator!=(const Point& a, const Point& b);

	double distance(Point p) const;

	double x;
	double y;
};