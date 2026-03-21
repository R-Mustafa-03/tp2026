#include "circle.h"
#include <cmath>

const double PI = 3.14159265358979323846;

Circle::Circle(const Point& center, double radius)
    : center_(center), radius_(radius) {}

double Circle::getArea() const {
    return PI * radius_ * radius_;
}

Point Circle::getCenter() const {
    return center_;
}

void Circle::move(double dx, double dy) {
    center_.x += dx;
    center_.y += dy;
}

void Circle::scale(double factor) {
    radius_ *= factor;
}

std::string Circle::getName() const {
    return "CIRCLE";
}
