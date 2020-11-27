#include "line.h"

bool operator==(const Line& a, const Line& b) {
	return (a.x == b.x) && (a.y == b.y);
}

bool operator!=(const Line& a, const Line& b) {
	return !(a == b);
}
