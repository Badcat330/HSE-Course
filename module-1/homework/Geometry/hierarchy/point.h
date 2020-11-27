struct Point {

	friend bool operator==(const Point& a, const Point& b);
	friend bool operator!=(const Point& a, const Point& b);

	double x;
	double y;
};