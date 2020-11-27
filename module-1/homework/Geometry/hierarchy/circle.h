#include "ellipse.h"

class Circle: protected Ellipse {
 public:
	Circle() = delete;
	Circle(Point center, double  radius);
	Circle(const Circle& origin);
	Circle& operator=(const Circle& origin );
	~Circle();

	double radius();
};