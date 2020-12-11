#include <cstdlib>
#include <utility>

#include "rectangle.h"

Rectangle::Rectangle(std::pair <Point, Point> point, double attitude){
    points.push_back(point.first);
    points.push_back(Point(point.first.x, point.second.y));
    points.push_back(point.second);
    points.push_back(Point(point.second.x, point.first.y));
}

std::pair<Line, Line> Rectangle::diagonals() {
    return std::pair<Line, Line>(Line(points[0], points[2]), Line(points[1], points[3]));
}

Point Rectangle::center() {
    return Point((points[0].x + points[2].x) / 2, (points[0].y + points[2].y) / 2);
}

