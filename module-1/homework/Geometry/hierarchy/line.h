#include"point.h"

struct Line {

	friend bool operator==(const Line& a, const Line& b);
	friend bool operator!=(const Line& a, const Line& b);

	Point x;
	Point y;
};