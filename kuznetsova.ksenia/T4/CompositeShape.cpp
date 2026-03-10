#include "CompositeShape.h"
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <cmath>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    shapes_.push_back(std::move(shape));
}

Shape* CompositeShape::getShape(size_t index) const {
    if (index >= shapes_.size()) {
        throw std::out_of_range("Индекс вне диапазона");
    }
    return shapes_[index].get();
}

void CompositeShape::getBoundingBox(Point& min, Point& max) const {
    if (isEmpty()) {
        min = Point(0, 0);
        max = Point(0, 0);
        return;
    }

    min.x = std::numeric_limits<double>::max();
    min.y = std::numeric_limits<double>::max();
    max.x = -std::numeric_limits<double>::max();
    max.y = -std::numeric_limits<double>::max();

    // Проходим по всем фигурам и находим их центры
    for (const auto& shape : shapes_) {
        Point center = shape->getCenter();
        min.x = std::min(min.x, center.x);
        min.y = std::min(min.y, center.y);
        max.x = std::max(max.x, center.x);
        max.y = std::max(max.y, center.y);
    }
}

double CompositeShape::getArea() const {
    double total = 0;
    for (const auto& shape : shapes_) {
        total += shape->getArea();
    }
    return total;
}

Point CompositeShape::getCenter() const {
    //центр ограничивающего прямоугольника по центрам фигур
    Point min, max;
    getBoundingBox(min, max);
    return Point((min.x + max.x) / 2, (min.y + max.y) / 2);
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    if (isEmpty()) return;

    //шаг 1: получаем центр составной фигуры
    Point compositeCenter = getCenter();

    //шаг 2: обрабатываем каждую вложенную фигуру
    for (auto& shape : shapes_) {
        //подшаг 2.1: находим текущий центр фигуры
        Point shapeCenter = shape->getCenter();

        //подшаг 2.2: вычисляем вектор от центра композита до центра фигуры
        double dx = shapeCenter.x - compositeCenter.x;
        double dy = shapeCenter.y - compositeCenter.y;

        //подшаг 2.3: масштабируем вектор
        double newDx = dx * factor;
        double newDy = dy * factor;

        //подшаг 2.4: вычисляем новое положение центра
        Point newShapeCenter(
            compositeCenter.x + newDx,
            compositeCenter.y + newDy
        );

        //подшаг 2.5: смещаем фигуру в новое положение
        double moveX = newShapeCenter.x - shapeCenter.x;
        double moveY = newShapeCenter.y - shapeCenter.y;
        shape->move(moveX, moveY);

        //подшаг 2.6: масштабируем саму фигуру
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}