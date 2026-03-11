#include "composite_shape.h"
#include <algorithm>
#include <limits>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    shapes.push_back(std::move(shape));
}
void CompositeShape::getBoundingBox(Point& min, Point& max) const {
    if (shapes.empty()) {
        min = Point(0, 0);
        max = Point(0, 0);
    return;
    }
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& shape : shapes) {
        Point center = shape->getCenter();
        minX = std::min(minX, center.x);
        minY = std::min(minY, center.y);
        maxX = std::max(maxX, center.x);
        maxY = std::max(maxY, center.y);
    }
    min = Point(minX, minY);
    max = Point(maxX, maxY);
}
double CompositeShape::getArea() const {
    double total = 0.0;
    for (const auto& shape : shapes) {
        total += shape->getArea();
    }
    return total;
}
Point CompositeShape::getCenter() const {
    if (shapes.empty()) {
        return Point(0, 0);
    }
    double sumX = 0.0, sumY = 0.0;
    for (const auto& shape : shapes) {
        Point center = shape->getCenter();
        sumX += center.x;
        sumY += center.y;
    }
    return Point(sumX / shapes.size(), sumY / shapes.size());
}
void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes) {
        shape->move(dx, dy);
    }
}
void CompositeShape::scale(double factor) {
    if (shapes.empty()) return;

Point compositeCenter = getCenter();
    for (auto& shape : shapes) {
        Point shapeCenter = shape->getCenter();

        double dx = shapeCenter.x - compositeCenter.x;
        double dy = shapeCenter.y - compositeCenter.y;

        double newDx = dx * factor;
        double newDy = dy * factor;

        Point newShapeCenter(compositeCenter.x + newDx, compositeCenter.y + newDy);

        double moveX = newShapeCenter.x - shapeCenter.x;
        double moveY = newShapeCenter.y - shapeCenter.y;
        shape->move(moveX, moveY);

        shape->scale(factor);
    }
}
std::string CompositeShape::getName() const {
    return "COMPOSITE";
}
bool CompositeShape::containsPoint(const Point& p) const {
    for (const auto& shape : shapes) {
        if (shape->containsPoint(p)) {
            return true;
        }
    }
    return false;
}
double CompositeShape::getPerimeter() const {
    double total = 0.0;
    for (const auto& shape : shapes) {
        total += shape->getPerimeter();
    }
    return total;
}
//