#include "Ring.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Ring::Ring(const Point& center, double outRad, double inRad)
    : center_(center), outR_(outRad), inR_(inRad) {
}

double Ring::getArea() const {
    return M_PI * (outR_ * outR_ - inR_ * inR_);
}

Point Ring::getCenter() const {
    return center_;
}

void Ring::move(double dx, double dy) {
    center_.x += dx;
    center_.y += dy;
}

void Ring::scale(double factor) {
    outR_ *= factor;
    inR_ *= factor;
}

std::string Ring::getName() const {
    return "RING";
}