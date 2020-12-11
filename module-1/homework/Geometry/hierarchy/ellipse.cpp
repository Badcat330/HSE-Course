#include "ellipse.h"

Ellipse::Ellipse(std::pair<Point, Point> _focuses, double _focusSum) : focuses(_focuses), focusSum(_focusSum) {
    a = focusSum / 2;
    c = focuses.first.distance(focuses.second);
    b = std::sqrt(a * a - c * c);
}

Ellipse::Ellipse(const Ellipse &origin) {
    focuses = origin.focuses;
    focusSum = origin.focusSum;
}

Ellipse &Ellipse::operator=(const Ellipse &origin) {
    focuses = origin.focuses;
    focusSum = origin.focusSum;
    return *this;
}

std::pair<Point, Point> Ellipse::getFocuses() {
    return focuses;
}

std::pair<Line, Line> Ellipse::directrices() {
    //TODO
    return std::pair<Line, Line>();
}

double Ellipse::eccentricity() const {
    return c / a;
}

Point Ellipse::center() const {
    return Point{(focuses.first.x + focuses.second.x) / 2, (focuses.first.y + focuses.second.y) / 2};
}

double Ellipse::area() const {
    return Consts::PI * a * b;
}

double Ellipse::perimeter() const {
    return 4 * (Consts::PI * a * b + (a - b)) / (a + b);
}

bool Ellipse::operator==(const Shape &another) const {
    const auto* ellipse_another = dynamic_cast<const Ellipse*>(&another);

    if(ellipse_another) {
        return focuses.first == ellipse_another->focuses.first && focuses.second == ellipse_another->focuses.second &&
               focusSum == ellipse_another->focusSum;
    }

    return false;
}

void Ellipse::rotate(Point center, double angle) {
    focuses.first.x =
            ((focuses.first.x - center.x) * cos(angle) - (focuses.first.y - center.y) * sin(angle)) + center.x;
    focuses.first.y =
            ((focuses.first.x - center.x) * sin(angle) + (focuses.first.y - center.y) * cos(angle)) + center.y;
    focuses.second.x =
            ((focuses.second.x - center.x) * cos(angle) - (focuses.second.y - center.y) * sin(angle)) + center.x;
    focuses.second.y =
            ((focuses.second.x - center.x) * sin(angle) + (focuses.second.y - center.y) * cos(angle)) + center.y;
}

void Ellipse::reflex(Point center) {
    focuses.first.x = 2 * center.x - focuses.first.x;
    focuses.first.y = 2 * center.y - focuses.first.y;
    focuses.second.x = 2 * center.x - focuses.second.x;
    focuses.second.y = 2 * center.y - focuses.second.y;
}

void Ellipse::reflex(Line axis) {
    Point _a = axis.a;
    Point _b = axis.b;
    Point _c = focuses.first;
    double v_x = _b.y - _a.y;
    double v_y = _a.x - _b.x;

    double k = (_a.x * _b.y - _b.x * _a.y + _a.y * _c.x - _b.y * _c.x + _b.x * _c.y - _a.x * _c.y) /
               (v_x * (_b.y - _a.y) + v_y * (_a.x - _b.x));
    double p_x = _c.x + v_x * k;
    double p_y = _c.y + v_y * k;
    focuses.first.x = 2 * p_x - focuses.first.x;
    focuses.first.y = 2 * p_y - focuses.first.y;

    _c = focuses.second;
    k = (_a.x * _b.y - _b.x * _a.y + _a.y * _c.x - _b.y * _c.x + _b.x * _c.y - _a.x * _c.y) /
        (v_x * (_b.y - _a.y) + v_y * (_a.x - _b.x));
    p_x = _c.x + v_x * k;
    p_y = _c.y + v_y * k;
    focuses.second.x = 2 * p_x - focuses.second.x;
    focuses.second.y = 2 * p_y - focuses.second.y;
}

void Ellipse::scale(Point center, double coefficient) {
    focuses.first.x = coefficient * (focuses.first.x - center.x) + center.x;
    focuses.first.y = coefficient * (focuses.first.y - center.y) + center.y;
    focuses.second.x = coefficient * (focuses.second.x - center.x) + center.x;
    focuses.second.y = coefficient * (focuses.second.y - center.y) + center.y;
}

bool Ellipse::containsPoint(Point point) const {
    return pow(point.x, 2) / pow(a, 2) + pow(point.y, 2) / pow(b, 2) <= 1;
}

bool Ellipse::isSimilarTo(const Shape &another) const {
    const auto* ellipse_another = dynamic_cast<const Ellipse*>(&another);

    if (ellipse_another) {
        return a / ellipse_another->a == b / ellipse_another->b;
    }

    return false;
}

bool Ellipse::isCongruentTo(const Shape &another) const {
    const auto* ellipse_another = dynamic_cast<const Ellipse*>(&another);

    if (ellipse_another) {
        double k = a / ellipse_another->a;
        return k == b / ellipse_another->b && k == 1;
    }

    return false;
}

