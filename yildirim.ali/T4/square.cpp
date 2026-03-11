#include "square.h"

Square::Square(const Point& bl, double side) : bottomLeft(bl), sideLength(side) {}
double Square::getArea() const {
    return sideLength * sideLength;
}
Point Square::getCenter() const {
    double centerX = bottomLeft.x + sideLength / 2.0;
    double centerY = bottomLeft.y + sideLength / 2.0;
    return Point(centerX, centerY);
}
void Square::move(double dx, double dy) {
    bottomLeft.x += dx;
    bottomLeft.y += dy;
}
void Square::scale(double factor) {
    Point center = getCenter();
    bottomLeft.x = center.x + (bottomLeft.x - center.x) * factor;
    bottomLeft.y = center.y + (bottomLeft.y - center.y) * factor;
    sideLength *= factor;
}
std::string Square::getName() const {
    return "SQUARE";
}
bool Square::containsPoint(const Point& p) const {
    return (p.x >= bottomLeft.x && p.x <= bottomLeft.x + sideLength &&
    p.y >= bottomLeft.y && p.y <= bottomLeft.y + sideLength);
}
double Square::getPerimeter() const {
    return 4 * sideLength;
}
