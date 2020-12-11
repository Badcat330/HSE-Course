

class Ellipse: public Shape {
 public:
	Ellipse() = delete;
	Ellipse(std::pair<Point,Point> focuses, double focusSum);
	Ellipse(const Ellipse& origin);
	Ellipse& operator=(const Ellipse& origin);
	~Ellipse();

	double area() const override;
	double perimeter() const override;
	bool operator==(const Shape& another) const override; // TODO проверить корректность работы
	bool isCongruentTo(const Shape &another) const override;
	bool isSimilarTo(const Shape &another) const override;
	bool containsPoint(Point point) const override;
	void rotate(Point center, double angle) override;
	void reflex(Point center) override;
	void reflex(Line axis) override;
	void scale(Point center, double coefficient) override;

	std::pair<Point,Point> getFocuses(); // фокусы эллипса
	std::pair<Line, Line> directrices(); // пара директрис эллипса
	double eccentricity(); // эксцентриситет
	Point center(); // центр эллипса

 protected:
	std::pair<Point,Point> focuses;
	double focusSum;
};