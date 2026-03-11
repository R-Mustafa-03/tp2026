#include "ellipse.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Ellipse::Ellipse(const Point& c, double r_x, double r_y) : center(c), rx(r_x), ry(r_y) {}
double Ellipse::getArea() const {
    return M_PI * rx * ry;
}
Point Ellipse::getCenter() const {
    return center;
}
void Ellipse::move(double dx, double dy) {
    center.x += dx;
    center.y += dy;
}
void Ellipse::scale(double factor) {
    rx *= factor;
    ry *= factor;
}
std::string Ellipse::getName() const {
    return "ELLIPSE";
}
bool Ellipse::containsPoint(const Point& p) const {
    double dx = (p.x - center.x) / rx;
    double dy = (p.y - center.y) / ry;
    return (dx * dx + dy * dy) <= 1.0;
}
double Ellipse::getPerimeter() const {
    double sum = rx + ry;
    double h = ((rx - ry) * (rx - ry)) / (sum * sum);
    return M_PI * sum * (1 + (3 * h) / (10 + std::sqrt(4 - 3 * h)));
}
//