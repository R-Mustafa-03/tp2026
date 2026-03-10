#include "Square.h"

Square::Square(const Point& bottomLeft, double sideLength)
    : bottomLeft_(bottomLeft), side_(sideLength) {
}

double Square::getArea() const {
    return side_ * side_;
}

Point Square::getCenter() const {
    return Point(bottomLeft_.x + side_ / 2, bottomLeft_.y + side_ / 2);
}

void Square::move(double dx, double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
}

void Square::scale(double factor) {
    Point center = getCenter();
    side_ *= factor;
    bottomLeft_.x = center.x - side_ / 2;
    bottomLeft_.y = center.y - side_ / 2;
}

std::string Square::getName() const {
    return "SQUARE";
}