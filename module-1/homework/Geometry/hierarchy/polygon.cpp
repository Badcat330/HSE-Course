#include <vector>
#include "polygon.h"

Polygon::Polygon(std::vector<Point> &_points) : points(_points) {
}

Polygon::Polygon(const Polygon &origin) {
    points = origin.points;
}

Polygon &Polygon::operator=(const Polygon &origin) {
    points = origin.points;
    return *this;
}

Polygon::~Polygon() {
    points.clear();
}

double Polygon::area() const {
    double sum = 0;

    for (size_t i = 0; i < verticesCount(); ++i)
        sum += points[i].x * points[(i + 1) % verticesCount()].y - points[(i + 1) % verticesCount()].x * points[i].y;

    std::abs(sum);
    sum /= 2;
    return sum;
}

double Polygon::perimeter() const {
    double perimeter = 0;

    for (size_t i = 0; i < verticesCount() - 1; i++) {
        perimeter += points[i].distance(points[i + 1]);
    }

    perimeter += points[verticesCount() - 1].distance(points[0]);
    return perimeter;
}

bool Polygon::isCongruentTo(const Shape &another) const {
    // TODO: catch exception
    const auto &polygon_another = dynamic_cast<const Polygon &>(another);

    double k = points[0].distance(points[verticesCount() - 1]) /
               polygon_another.points[0].distance(polygon_another.points[verticesCount() - 1]);

    return k == 1 && isSimilarTo(another);
}

size_t Polygon::verticesCount() const {
    return points.size();
}

std::vector<Point> Polygon::getVertices() const {
    return points;
}

bool Polygon::isSimilarTo(const Shape &another) const {
    // TODO: catch exception
    const auto &polygon_another = dynamic_cast<const Polygon &>(another);

    if (verticesCount() != polygon_another.verticesCount())
        return false;

    double k = points[0].distance(points[verticesCount() - 1]) /
               polygon_another.points[0].distance(polygon_another.points[verticesCount() - 1]);

    for (size_t i = 0; i < verticesCount() - 1; ++i) {
        double k_buf = points[i].distance(points[i + 1]) /
                       polygon_another.points[i].distance(polygon_another.points[i + 1]);

        if (k_buf != k)
            return false;
    }

    return true;
}

bool Polygon::operator==(const Shape &another) const {
    // TODO: catch exception
    const auto &polygon_another = dynamic_cast<const Polygon &>(another);

    if (verticesCount() != polygon_another.verticesCount())
        return false;

    for (size_t i = 0; i < verticesCount(); ++i) {
        if (points[i] != polygon_another.points[i])
            return false;
    }

    return true;
}

bool Polygon::isConvex() const {
    Point vector_a_buf{
            points[0].x - points[verticesCount() - 1].x,
            points[0].y - points[verticesCount() - 1].y
    };

    Point vector_b_buf{
            points[0].x - points[1].x,
            points[0].y - points[1].y
    };

    bool sign = vector_a_buf.x * vector_b_buf.y - vector_a_buf.y * vector_b_buf.x >= 0;

    for (size_t i = 1; i < verticesCount(); ++i) {
        Point vector_a{
                points[i].x - points[i - 1].x,
                points[i].y - points[i - 1].y
        };

        Point vector_b{
                points[i].x - points[(i + 1) % verticesCount()].x,
                points[i].y - points[(i + 1) % verticesCount()].y
        };

        if (sign != vector_a.x * vector_b.y - vector_a.y * vector_b.x >= 0)
            return false;
    }

    return true;
}

bool Polygon::containsPoint(Point point) const {
    bool flag;
    double S, S1, S2, S3;

    for (size_t i = 0; i < verticesCount(); ++i) {
        for (size_t j = 0; j < verticesCount() - 2; ++j) {
            size_t i1 = (j + i) % verticesCount();
            size_t i2 = (j + i + 1) % verticesCount();

            S = abs(points[i1].x * (points[i2].y - points[i].y) + points[i2].x * (points[i].y - points[i1].y) +
                     points[i].x * (points[i1].y - points[i2].y));
            S1 = abs(points[i1].x * (points[i2].y - point.y) + points[i2].x * (point.y - points[i1].y) +
                     point.x * (points[i1].y - points[i2].y));
            S2 = abs(points[i].x * (points[i2].y - point.y) + points[i2].x * (point.y - points[i].y) +
                     point.x * (points[i].y - points[i2].y));
            S3 = abs(points[i1].x * (points[i].y - point.y) + points[i].x * (point.y - points[i1].y) +
                     point.x * (points[i1].y - points[i].y));

            if(S == S1 + S2 + S3) {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
            break;
    }

    return flag;
}

void Polygon::rotate(Point center, double angle) {

}




