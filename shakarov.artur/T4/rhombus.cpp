#include "rhombus.h"

Rhombus::Rhombus(const Point& center, double vertDiagonal, double horDiagonal)
    : center_(center), vertDiagonal_(vertDiagonal), horDiagonal_(horDiagonal) {}

double Rhombus::getArea() const {
    return (vertDiagonal_ * horDiagonal_) / 2.0;
}

Point Rhombus::getCenter() const {
    return center_;
}

void Rhombus::move(double dx, double dy) {
    center_.x += dx;
    center_.y += dy;
}

void Rhombus::scale(double factor) {
    vertDiagonal_ *= factor;
    horDiagonal_ *= factor;
}

std::string Rhombus::getName() const {
    return "RHOMBUS";
}
