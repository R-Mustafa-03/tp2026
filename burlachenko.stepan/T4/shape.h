#ifndef SHAPE_H
#define SHAPE_H

#include "point.h"
#include <string>

struct Bounds
{
    double minX;
    double minY;
    double maxX;
    double maxY;
};


class Shape
{
public:
    virtual ~Shape() = default;

    virtual double getArea() const = 0;
    virtual Point getCenter() const = 0;
    virtual void move(const Point& newCenter) = 0;
    virtual void scale(double ratio) = 0;
    virtual std::string getName() const = 0;
    virtual Bounds getBounds() const = 0;
};

#endif
